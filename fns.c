/** Additonal functions*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "tpl.h"
#include "tokens.h"
#include "fns.h"
#include "translator_to_c.h"


// Folder name = 255, command name = 45
#define MAX_SYS_CMD_LEN 300

int count_mb_bytes( char *str )
{
    return MultiByteToWideChar( CP_UTF8, 0, str, -1, NULL, 0 );
}


/** Short variant of MultiByteWideCharacter */
void mb2wc( char *str, wchar_t *out, int out_size )
{
    MultiByteToWideChar( CP_UTF8, 0, str, -1, out, out_size );
}


/**2 sozi we bir harpy goshyar
 *
 * @to        - final string
 * @from      - first  adding string
 * @C         - second adding wchar_t
 * @from_len  - number of wchar_tacters of first string
 * @to_len    - number of wchar_tacters of final string
**/
void wcsstrchcat( wchar_t *to, wchar_t *from, wchar_t c )
{
    wcsncpys( to, from );

    str_add_char( to, c );
}


/** Söziň soňuna harpy goşýar*/
void str_add_char( wchar_t *to, wchar_t c )
{
    to[ wcslen( to )+1 ] = L'\0';	        		// Sonky \0 bire suyshirilyar
	to[ wcslen( to ) ] = c;
}

/* Checks first occurance of substring in string started from offset
 *
 * @string      - search in
 * @sub         - search for
 * @offset      - start searching from this position
 *
 * Return 0  if strings are equal
 * Return -1 if substring is longer than string
 * Return -2 if string is not found
 * Return 1  when substring found in string
**/
int wcsstr_by_offset(const wchar_t *string, const wchar_t*sub, unsigned int offset)
{
	int i;
	int len     = wcslen(string);
	int sub_len = wcslen(sub);

	// Can not search long string in smaller one;
	if (len<sub_len)
		return -1;

	// The strings are equal
	if (wcsncmp(string, sub, len)==0)
		return 0;

	for(i=0; offset<sub_len; offset++, i++)
	{
		if (string[offset]!=sub[i])
			return -2;
	}

	return 1;
}


// Taze papka yasayar
int sys_mkdir(wchar_t *folder_name, wchar_t rm_prev_dir)
{
	if (rm_prev_dir)
		// Onki faylyn yzlary pozulyar
		sys_rmdir(folder_name);
	// Max file length + command name length
	wchar_t sys_cmd[MAX_SYS_CMD_LEN] = L"mkdir >nul 2>nul ";		// >nul hides result, and
	wcsncat(sys_cmd, folder_name, wcslen(folder_name));			// 2>nul hides errors
	return !_wsystem(sys_cmd);
}

int sys_rmdir(wchar_t *folder_name)
{
	char sys_cmd[MAX_SYS_CMD_LEN] = "rmdir /S /Q >nul 2>nul ";
	char folder_name_norm[MAX_FILE_LEN] = {0};
	int len = wcstombs(NULL, folder_name, 0);
    wcstombs(folder_name_norm, folder_name, len);
	strncat(sys_cmd, folder_name_norm, strlen(folder_name_norm));
	return !system(sys_cmd);
}

int sys_is_dir_exists( wchar_t folder_name[ MAX_FILE_LEN ] )
{
    if ( _waccess( folder_name, 0 )!=-1 ) {
      return 1;
    }
    return 0;
}

// Berlen fayl adynyn onundaki papkalaryn atlaryny pozyar.
wchar_t *remove_dirnames( wchar_t *f )
{
	int i;
	for (i=0; i<wcslen(f); ++i)
	{
		if (f[i]==L'/')
		    f[i]=L'\\';
	}
	i = 0;

	if (wcsrchr(f, L'\\'))
	{
	    wchar_t *t = wcsrchr(f, L'\\');

	    if (wcslen(t)>1)
		{
			int i;
		    for (i=1; i<wcslen(t); ++i)
		    {
			    f[i-1] = t[i];
		    }
		    f[i-1] = L'\0';
		}
	}

	return f;
}

// Berlen fayl adynyn gornushini pozyar.
wchar_t *remove_ext(wchar_t *f, wchar_t *e)
{
	//if (wcslen(e)>=wcslen(f))
	//	return f;

	if (wcsstr(f,e)!=NULL)
	{
		f[wcslen(f)-wcslen(e)] = L'\0';
	}

	return f;
}

/*
 * Berlen sozun bolmaly uzynlygyndaky,
 * ichindaki hemme harplaryn deregine \0 bilen dolduryar.
 *
 * Eger-de bolmaly uzynlygy tanalmasy, yagny argument -1 bolsa,
 * ol wcslen() funksiyasy arkaly tanalyar.
 *   Shonda dine sozde birinji \0 dushyancha hemme harplaryn deregine \0 goyular
**/
wchar_t *empty_string(wchar_t *f, int len)
{
	int i, last;
	if (len<0)
		last = wcslen(f)-1;
	else
		last = len-1;

	for (i=0; i<=last; ++i)
	{
		f[i] = L'\0';
	}
	return f;
}


void divide_string(wchar_t *source, wchar_t d, wchar_t ***out, int *its)
{
    int i;
    wchar_t *tmp = malloc(sizeof(wchar_t)*10);

    tmp[0] = L'\0';
    for (i=0; i<wcslen(source); ++i)
    {
        if (source[i]==d)
        {
            if (wcslen(tmp))
            {
                *its += 1;
                *out = realloc(*out, sizeof(**out)*(*its));
                (*out)[*its-1] = NULL;
                (*out)[*its-1] = realloc((*out)[*its-1], sizeof(***out)*(wcslen(tmp)+1));
                wcsncpy((*out)[*its-1], tmp, wcslen(tmp)+1);

                free(tmp);

                tmp = malloc(sizeof(wchar_t)*10);
                tmp[0] = L'\0';
            }
        }
        else
        {
            if (wcslen(tmp))
            {
                int last = wcslen(tmp)-1;
                tmp[last+1] = source[i];
                tmp[last+2]   = L'\0';
            }
            else
            {
                tmp[0] = source[i];
                tmp[1] = L'\0';
            }
        }
    }
    if (wcslen(tmp))
    {
        *its += 1;
        //free((*out)[*items-2]);
        *out = realloc(*out, sizeof(*out)*(*its));
        (*out)[*its-1] = NULL;
        (*out)[*its-1] = realloc((*out)[*its-1], sizeof(***out)*(wcslen(tmp)+1));
        wcsncpy((*out)[*its-1], tmp, wcslen(tmp)+1);
    }
    free(tmp);
}


/** Harplyň daşynda duran goşa dyrnaklary aýyryp, yzyna gaýtarýar.

    @withKuotes - daşy goşa dyrnakly tekst,
    @unKutoed   - goşa dyrnaklar aýrylan görnüşi (Harplaryň sanawynyň uzynlygy @withKuotes sanawynyňky ýaly bolmaly)*/
void string_helper_remove_dquotes(wchar_t *unquoted, wchar_t *with_quotes)
{
    int u, q; // unquoted position, quoted position
    for(u=0, q=1;q<(wcslen(with_quotes)-1); ++q, ++u)
    {
        unquoted[u] = with_quotes[q];
    }
    unquoted[u] = L'\0';
}

/** TURKMEN LETTERS UNICODE:
    Ü - 00DC  LATIN CAPITAL LETTER U WITH DIAERESIS
    ü - 00FC  LATIN SMALL LETTER U WITH DIAERESIS
    ç - 00E7  LATIN SMALL LETTER C WITH CEDILLA
    ş - 015F  LATIN SMALL LETTER S WITH CEDILLA

*/
#define TK_U 0x00DC
#define TK_u 0x00FC
#define TK_c 0x00E7
#define TK_s 0x015f

int count_bytes( wchar_t *wstr )
{
    if ( wstr==NULL )
        return 0;
    if ( !wcslen( wstr ) )
    {
        return sizeof( *wstr );
    }
    return ( wcslen( wstr )*sizeof( *wstr ) );
}

int get_null_size()
{
    return sizeof( wchar_t );
}

int realloc_wchar_heap( wchar_t **mem, int *memlen, wchar_t *add )
{
    *memlen += count_bytes( add );

    *mem = realloc( *mem, *memlen );

    return ( *mem==NULL ) ? 0 : 1;
}

int realloc_wchar_null_heap( wchar_t **mem, int *memlen )
{
    *memlen += get_null_size();
    *mem = realloc( *mem, *memlen );

    return ( *mem==NULL ) ? 0 : 1;
}

int wcsncat_on_heap( wchar_t **mem, int *memlen, wchar_t *add )
{
    if ( !realloc_wchar_heap( mem, memlen, add) )
        return 0;

    wcsncat( *mem, add, wcslen( add ) );

    return 1;
}

int wcsncpy_on_heap( wchar_t **mem, int *memlen, wchar_t *add )
{
    if ( !realloc_wchar_heap( mem, memlen, add) )
        return 0;
    realloc_wchar_null_heap( mem, memlen );

    wcsncpy( *mem, add, wcslen( add )+1 );

    return 1;
}

int wcsadd_on_heap( wchar_t **mem, int *memlen, wchar_t *add )
{
    if ( *memlen==0 )
    {
        return wcsncpy_on_heap( mem, memlen, add );
    }

    return wcsncat_on_heap( mem, memlen, add );
}


/** wcsncpy funksiýanyň has gysgaldyp ýazyljak görnüşi. Hem-de goraglyrak görnüşi
    d - ýazylmaly ýeri
    s - ýazylmaly faýlyň özi
*/
int wcsncpys( wchar_t *d, wchar_t *s )
{
    /// Öz özüňe maglumat ýazmak nämä gerek.
    if ( d==s )
        return 1;

    return ( int )wcsncpy( d, s, wcslen( s )+1 );
}


/** Iki sany sözleriň biri birine dürs gabat gelýänini barlaýar.*/
int is_wcseq( wchar_t *s1, wchar_t *s2 )
{
    return  wcslen( s1 )==wcslen( s2 ) &&
            wcsncmp( s1, s2, wcslen( s1 ) )==0;
}

void    str_change_char( wchar_t *s, wchar_t c, wchar_t with )
{
    int i;
    for ( i=0; i<wcslen( s ); ++i )
    {
        if ( s[ i ]==c )
            s[ i ] = with;
    }
}
