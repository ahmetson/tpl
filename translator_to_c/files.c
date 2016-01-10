/**
 * Ýasalan kodlar bilen işleýän funksiýalar hem-de
 * Ýasalan kodlara degişli global ülňilerdäki magtlumatlar bilen işleýän funksiýalar
**/
#include <stdlib.h>
#include <string.h>

#include "files.h"
#include "../fns.h"
#include "../main/glob.h"


/**
 * Faýllaryň sanawyndan, berlen faýlyň adyna görä, faýlyň ýasalan headery gaýtarylýar
**/
wchar_t *get_header_source_by_source(wchar_t *s)
{
    int i, len;
    for (i=0; i<CUR_FILE_NUM; i++)
    {
        len = (wcslen(s)<wcslen(FILES[i].source))?wcslen(FILES[i].source):wcslen(s);
        if (wcsncmp(FILES[i].source, s, len)==0)
            return remove_dirnames(FILES[i].h_source);
    }
    return L"";
}

/**
 * Faýllaryň sanawyndan, berlen faýlyň adyna görä, faýlyň ýasalan headery gaýtarylýar
**/
wchar_t *get_c_source_by_source(wchar_t *s)
{
    int i, len;
    for (i=0; i<CUR_FILE_NUM; i++)
    {
        len = (wcslen(s)<wcslen(FILES[i].source))?wcslen(FILES[i].source):wcslen(s);
        if (wcsncmp(FILES[i].source, s, len)==0)
            return FILES[i].c_source;
    }
    return L"";
}


/**
 * Ýasalan kodyň içine: #include "" goýýar.
 *
 * @s               - Içine goýmaly ýasalan kod.
 * @included_files  - #include komanda-da ulanylmaly faýl atlary
 * @files_num       - #include komanda-da ulanylýan faýl atlarynyň möçberi
**/
int add_includes_to_source(FILE *s, wchar_t (*included_files)[MAX_FILE_LEN], int files_num)
{
	int i, len = sizeof( wchar_t ), cmode = 0;

	wchar_t c, prev_c = L'\0', prev_c2 = L'\0',
            *l = malloc( sizeof( wchar_t ) ),
            *tmp= NULL,
            *include = L"#include ",
            *nl = L"\n";
    l[0] = L'\0';

    fseek(s, SEEK_SET, 0);

	while ( ( c=fgetwc( s ) )!=WEOF )
	{
	    /// Includleriň gutarandygyny aňladýan zolaga geç.
	    if ( prev_c==L'/' && prev_c2==L'/' && c==L'.' )
        {
            len -= 2*sizeof( *l );
            l[ ( wcslen( l )-2 ) ] = L'\0';
            cmode = 1;
            continue;
        }
        /// Includlamagy başlaýan teswir.
		if (cmode==1)
		{
		    if (c==L'\n')
			{
			    cmode=2;
				continue;
			}
		}
		/// Inklud ýa inkludlamagy aňladýan teswir däl zolak
		else if(!cmode)
		{
            len += sizeof(*l);
			tmp = realloc(l, len);
            if (tmp!=NULL)
                l=tmp;

			l[ ( len/sizeof( *l ) )-2 ] = c;
			l[ ( len/sizeof( *l ) )-1 ] = L'\0';
			prev_c2 = prev_c;
			prev_c = c;
		}
		/// Inkludlamaly zolag
		else
		{
            // Fayllar goshulyar
		    for(i=0; i<files_num; ++i)
            {
                //printf(L"Goshmaly:%s %d\n", included_files[i], files_num);
                wchar_t putme[MAX_FILE_LEN+20] = {0};
                wcsncpy(putme, include, wcslen(include)+1);
                wcsncat(putme, included_files[i], wcslen(included_files[i]));
                wcsncat(putme, nl, wcslen(nl));
                wcsadd_on_heap( &l, &len, putme );
            }
            //printf("TEST:<%ls>\n", l);

            // Header goşmalary gutarandygy bellenilýär.
            wchar_t *end_of_headers = L"//.\n\n";
            wcsadd_on_heap( &l, &len, end_of_headers );

			cmode = 0;
		}
	}

	fseek(s, 0, SEEK_SET);
	int answ2 = fputws(l, s);
	//int answ = fwrite(l, sizeof(wchar_t), wcslen(l)+1, s);
	//printf("%ls\n", l);

	free(l);

    return 1;
}


