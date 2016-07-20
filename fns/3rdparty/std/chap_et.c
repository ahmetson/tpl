/** 3rdParty/STD funksiýa
    Çap etmek
**/
#include <stdlib.h>
#include <string.h>

#include "../../fn.h"
#include "../../../fns.h"
#include "../../../translator_to_c.h"
#include "../../../main/glob.h"
#include "chap_et.h"
#include "../../../token/token_types.h"
#include "../../../token/token_types/const_data.h"
#include "../../../translator_to_c.h"

wchar_t *c_chap_et_lib    = L"\"_tpl_std_chap_et.h\"";
wchar_t *c_chap_et_name    = L"_tpl_std_chap_et";
wchar_t *h_chap_et_source = L"_tpl_std_chap_et.h";
wchar_t *c_chap_et_source = L"_tpl_std_chap_et.c";

int add_std_chap_et()
{
    wchar_t *file_name = L"tpl_std.c",
            *fn_tpl_name   = L"chap_et",
            *fn_c_chap_et_name = L"_tpl_std_chap_et",
            *c_chap_et_lib_name = L"\"_tpl_std_chap_et.h\"";
    func std_chap_et;
    std_chap_et.file_num = -1;
    wcsncpy(std_chap_et.file_name, file_name, wcslen(file_name)+1);
    wcsncpy(std_chap_et.name, fn_tpl_name, wcslen(fn_tpl_name)+1);
    std_chap_et.type_class = FUNC_CLASS_3RD_PARTY;
    std_chap_et.type_num = FUNC_TYPE_STD;
    std_chap_et.args_num = -1;     // * ýaly argumentleri kabul edýär
    std_chap_et.inf_wchar_t = 0;
    std_chap_et.inf_wchar_t_pos = 0;
    std_chap_et.inf_line_pos = 0;
    std_chap_et.return_class = TOK_CLASS_CONST_DATA;
    std_chap_et.return_type = INT_CONST_DATA_TOK_NUM;
    wcsncpy(std_chap_et.c_name, fn_c_chap_et_name, wcslen(fn_c_chap_et_name)+1);
    wcsncpy(std_chap_et.c_lib, c_chap_et_lib_name, wcslen(c_chap_et_lib_name)+1);
    std_chap_et.make_args_string = make_std_chap_et_args;
    add_func_to_list(std_chap_et);

    // Argumentler
    func_arg std_chap_et_arg;
    std_chap_et_arg.type_class = TOK_CLASS_CONST_DATA;
    std_chap_et_arg.type_num   = STRING_CONST_DATA_TOK_NUM;
    add_arg_to_last_func(std_chap_et_arg, 0);

    return 1;
}


void make_std_chap_et_args(parenthesis *paren, wchar_t **line, int *llen)
{
    wchar_t *first = NULL,
            *second = NULL,
            *dquote = L"\"",
            *empty = L"",
            *fplace = L"s",
            *item_separator = L", ";
    int flen = 0, slen = 0;

    wcsadd_on_heap( &first, &flen, dquote );
    wcsadd_on_heap( &second, &slen, empty );

    int i;
    parenthesis_elem *p_es = get_paren_elems(paren->elems);
    for(i=0; i<paren->elems_num; ++i)
    {
        wcsadd_on_heap( &first, &flen, fplace );
        wcsadd_on_heap( &second, &slen, item_separator );
        write_paren_item_c_code( &p_es[i], &second, &slen );
    }
    wcsadd_on_heap( &first, &flen, dquote );

    wcsadd_on_heap( line, llen, first );
    wcsadd_on_heap( line, llen, second );

    free(first);
    free(second);
}


void write_source_std_chap_et_source_code()
{
    wchar_t *bslash = L"\\";
    wchar_t h_path[MAX_FILE_LEN] = {0};
	wcsncat(h_path, DIR_C_FOLDER, wcslen(DIR_C_FOLDER));
	wcsncat(h_path, bslash, wcslen(bslash));
	wcsncat(h_path, h_chap_et_source, wcslen(h_chap_et_source));

    wchar_t c_path[MAX_FILE_LEN] = {0};
	wcsncat(c_path, DIR_C_FOLDER, wcslen(DIR_C_FOLDER));
	wcsncat(c_path, bslash, wcslen(bslash));
	wcsncat(c_path, c_chap_et_source, wcslen(c_chap_et_source));

	FILE *h = _wfopen(h_path, L"w, ccs=UTF-8");
	FILE *c = _wfopen(c_path, L"w, ccs=UTF-8");

    wchar_t *h_code = L"#ifndef _tpl_std_chap_et_h\n\
#define _tpl_std_chap_et_h\n\
\n\
void _tpl_std_chap_et(char *, ...);\n\
\n\
#endif\n";

    fputws(h_code, h);
    fclose(h);

    wchar_t *c_code = L"#include <string.h>\n\
#include <stdio.h>\n\
#include <windows.h>\n\
#include <stdlib.h>\n\
#include \"_tpl_std_chap_et.h\"\n\n\
char *_tpl_wc2mb( wchar_t *str ) {\n\
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);\n\
    char* m = malloc(sizeof(char)*bufferSize);\n\
    WideCharToMultiByte(CP_UTF8, 0, str, -1, m, bufferSize, NULL, NULL);\n\
    return m;\n\
}\n\
\n\
void _tpl_std_chap_et(char *format, ...)\n\
{\n\
va_list argptr;\
\n\
   va_start( argptr, format );\n\
\n\
   int i = 0;\n\
   while( format[i] != '\\0' ) {\n\
     // string\n\
     if( *format == 's' ) {\n\
       wchar_t *ws = va_arg( argptr, wchar_t * );\n\
       char *tmp_s = _tpl_wc2mb( ws );\n\
       printf( \"%s\", tmp_s );\n\
       free( tmp_s );\n\
     }\n\
     // character\n\
     else if( *format == 'c' ) {\n\
       wchar_t c = (int) va_arg( argptr, int );\n\
       wchar_t wcc[] = L\"a\";\n\
       wcc[0] = c;\n\
       char *tmp_s = _tpl_wc2mb( wcc );\n\
       printf( \"%s\", tmp_s );\n\
       free( tmp_s );\n\
     }\n\
     // integer\n\
     else if( *format == 'd' ) {\n\
       int d = va_arg( argptr, int );\n\
       printf( \"%d\", d );\n\
     }\n\
\n\
     i += 1;\n\
   }\n\
\n\
   va_end( argptr );\n\
}\n";
    fputws(c_code, c);


	fclose(c);
}

