/**
 * Baş ýasaljak kodly faýl bilen bagly ähli maglumatlar saklanýar
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tpl.h"
#include "main_file.h"
#include "files.h"
#include "../translator_to_c/includes.h"
#include "../translator_to_c/files.h"
#include "../translator_to_c.h"

// Ýasaljak programmanyň baş faýlynyň ady
wchar_t MAIN_FILE_NAME[];
// Baş faýlda başga faýllaryň algoritmlerini saklaýan funksiýalaryň we faýllaryň sanawy
int  MAIN_FILE_INCLUDES_NUM;
wchar_t (*MAIN_FILE_INCLUDES)[2][MAX_INCLUDE_LEN];


/** Baş ýasaljak kodyň soňuna, başga ýasaljak kodlaryň algoritmlerini çagyrjak funksiýalar goýulýar.*/
int add_addtn_file_fns()
{
    file_item *f = get_file_by_name( MAIN_FILE_NAME );
    FILE *s = _wfopen(f->c_source, L"r+, ccs=UTF-8");
    int i;

    /// Begin at the end
    fseek(s, 0, SEEK_END);

	/// Baş faýla inklud etmeli faýllaryň sanawy
    file_incs *fi = NULL;
    if (( f->num+1 ) > INCLUDES_NUM )
    {
       fi = includes_add_new();
    }
    else
    {
       fi = &INCLUDES[ f->num ];
    }

    // Prototipler baş faýlda çagyrylýar, olaryň yglan edilen faýly baş faýla inklud edilmeli faýllaryň
    // sanawyna goşulýar
	wchar_t *tab = L"\t",
            *space_nl  = L" \n",
            *dquote = L"\"";
	for( i=0; i<MAIN_FILE_INCLUDES_NUM; ++i )
    {
       //printf(L"Goshmaly:%s %d\n", included_files[i], files_num);
        wchar_t putme[ MAX_FILE_LEN+20 ] = {0};
        wcsncpy(putme, tab, wcslen( tab )+1);
        wcsncat(putme, MAIN_FILE_INCLUDES[i][1], wcslen(MAIN_FILE_INCLUDES[i][1]));

        wcsncat(putme, space_nl, wcslen(space_nl));

        fputws( putme, s );
        // 3)
        wchar_t var_def_f[MAX_FILE_LEN] = {0};
        wcsncpy(var_def_f, dquote, wcslen(dquote)+1);
        wcsncat(var_def_f, MAIN_FILE_INCLUDES[i][0], wcslen(MAIN_FILE_INCLUDES[i][0]));
        wcsncat(var_def_f, dquote, wcslen(dquote));
        //printf(L"%s\n", var_def_f);

        includes_file_add_include(fi, var_def_f);
    }
	//printf(L"%s\n", l);

	fclose(s);

    return 1;
}


void write_to_csource_call_another_files_fn_open( FILE *f )
{
    wchar_t *line1 = L"\nvoid _tpl_call_another_files(){ ";
    wchar_t *l = NULL;
    int llen = 0;
    write_strings_to_file(f, &l, &llen , line1);
}
void trans_to_c_write_addtn_fn_multi_free( FILE *f )
{
    wchar_t *line1 = L"\n#include <stdarg.h>\nvoid _tpl_multi_free( int addr_num, ... ) {\n\
    int answer = 0;\n\
    va_list argptr;   \n\
\n\
    va_start( argptr, addr_num );\n\
\n\
    for( ; addr_num > 0; addr_num-- ) {\n\
      wchar_t *ws = va_arg( argptr, wchar_t * );\n\
      if ( ws!=NULL )\n\
        free( ws );\n\
    }\n\
\n\
    va_end( argptr );\n\
\n\
  }      ";
    wchar_t *l = NULL;
    int llen = 0;
    write_strings_to_file(f, &l, &llen , line1);
}
void trans_to_c_write_addtn_fn_glob_vars_free( )
{
    file_item *fi = get_file_by_name( MAIN_FILE_NAME );
    FILE *s = _wfopen(fi->c_source, L"r+, ccs=UTF-8");
    fseek(s, 0, SEEK_END);

    wchar_t *line1 = L"\nvoid _tpl_glob_vars_free( ) { _tpl_multi_free( ";
    fputws( line1, s );

    if ( C_CODE_GLOB_FREE_VARS_NUM!=0 )
    {
        wchar_t var_num[10] = { 0 };
        wsprintfW(var_num, L"%d", C_CODE_GLOB_FREE_VARS_NUM );
        fputws( var_num, s );
        fputws( L", ", s );

        int i;
        for ( i=0; i<C_CODE_GLOB_FREE_VARS_NUM; ++i )
        {
            fputws( C_CODE_GLOB_FREE_VARS[ i ], s );
            if ( i+1<C_CODE_GLOB_FREE_VARS_NUM )
                fputws( L", ", s );
        }

        fputws( L"    );\n", s );
    }
    fputws( L" }\n", s );

    fclose( s );
}
void write_to_csource_call_another_files_fn_close(  )
{
    file_item *fi = get_file_by_name( MAIN_FILE_NAME );
    FILE *s = _wfopen(fi->c_source, L"r+, ccs=UTF-8");

    fseek(s, 0, SEEK_END);
    fputws( L"}; \n", s );
    fclose( s );
}

void write_to_csource_free_glob_strings()
{
    if ( C_CODE_GLOB_FREE_VARS_NUM==0 )
        return;

    file_item *fi = get_file_by_name( MAIN_FILE_NAME );
    FILE *s = _wfopen(fi->c_source, L"r+, ccs=UTF-8");

    fseek(s, 0, SEEK_END);
    fputws( L" atexit(  _tpl_glob_vars_free ); \n", s );

    fclose( s );
}

