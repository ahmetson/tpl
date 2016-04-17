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
    int i, j;

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
    write_code_line(f, &l, &llen , line1);
}
void write_to_csource_call_another_files_fn_close(  )
{
    file_item *fi = get_file_by_name( MAIN_FILE_NAME );
    FILE *s = _wfopen(fi->c_source, L"r+, ccs=UTF-8");

    fseek(s, 0, SEEK_END);
    fputws( L"}; \n", s );
    fclose( s );
}
