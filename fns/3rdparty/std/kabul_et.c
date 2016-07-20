/** 3rdParty/STD funksiýa
    Çap etmek
**/
#include <stdlib.h>
#include <string.h>

#include "../../fn.h"
#include "../../../main/glob.h"
#include "kabul_et.h"
#include "../../../token/token_types.h"
#include "../../../token/token_types/const_data.h"
#include "../../../translator_to_c.h"

wchar_t *c_kabul_et_lib    = L"\"_tpl_std_kabul_et.h\"";
wchar_t *c_kabul_et_name    = L"_tpl_std_kabul_et";
wchar_t *h_kabul_et_source = L"_tpl_std_kabul_et.h";
wchar_t *c_kabul_et_source = L"_tpl_std_kabul_et.c";

int add_std_kabul_et()
{
    wchar_t *f_name = L"tpl_std.c",
            *fn_c_kabul_et_name = L"kabul_et";
    func std_kabul_et;
    std_kabul_et.file_num = -1;
    wcsncpy(std_kabul_et.file_name, f_name, wcslen(f_name)+1);
    wcsncpy(std_kabul_et.name, fn_c_kabul_et_name, wcslen(fn_c_kabul_et_name)+1);
    std_kabul_et.type_class = FUNC_CLASS_3RD_PARTY;
    std_kabul_et.type_num = FUNC_TYPE_STD;
    std_kabul_et.args_num = 0;     // * ýaly argumentleri kabul edýär
    std_kabul_et.inf_wchar_t = 0;
    std_kabul_et.inf_wchar_t_pos = 0;
    std_kabul_et.inf_line_pos = 0;
    std_kabul_et.return_class = TOK_CLASS_CONST_DATA;
    std_kabul_et.return_type = STRING_CONST_DATA_TOK_NUM;
    wcsncpy(std_kabul_et.c_name, c_kabul_et_name, wcslen(c_kabul_et_name)+1);
    wcsncpy(std_kabul_et.c_lib, c_kabul_et_lib, wcslen(c_kabul_et_lib)+1);
    std_kabul_et.make_args_string = make_user_def_fn_args;
    add_func_to_list(std_kabul_et);

    // Argumentler
    //func_arg std_chap_et_arg;
    //std_chap_et_arg.type_class = TOK_CLASS_CONST_DATA;
    //std_chap_et_arg.type_num   = STRING_CONST_DATA_TOK_NUM;
    //add_arg_to_last_func(std_chap_et_arg, 0);

    return 1;
}


void write_source_std_kabul_et_source_code()
{
    wchar_t *bslash = L"\\";
    wchar_t h_path[MAX_FILE_LEN] = {0};
	wcsncat(h_path, DIR_C_FOLDER, wcslen(DIR_C_FOLDER));
	wcsncat(h_path, bslash, wcslen(bslash));
	wcsncat(h_path, h_kabul_et_source, wcslen(h_kabul_et_source));

    wchar_t c_path[MAX_FILE_LEN] = {0};
	wcsncat(c_path, DIR_C_FOLDER, wcslen(DIR_C_FOLDER));
	wcsncat(c_path, bslash, wcslen(bslash));
	wcsncat(c_path, c_kabul_et_source, wcslen(c_kabul_et_source));

	FILE *h = _wfopen(h_path, L"w, ccs=UTF-8");
	FILE *c = _wfopen(c_path, L"w, ccs=UTF-8");

    wchar_t *h_code = L"#ifndef _tpl_std_kabul_et_h\n\
#define _tpl_std_kabul_et_h\n\
\n\
wchar_t *_tpl_std_kabul_et();\n\
\n\
#endif\n";

    fputws(h_code, h);
    fclose(h);

    wchar_t *c_code = L"#include <windows.h>\n#include <stdio.h>\n\
#include <stdlib.h>\n\
#include <string.h>\n\
#include \"_tpl_std_kabul_et.h\"\n\n\
wchar_t *_tpl_std_kabul_et()\n\
{\n\
	int str_len = sizeof(wchar_t);\n\
    wchar_t *str = malloc( str_len );\n\
    wcsncpy( str, L\"\\0\", str_len/sizeof(wchar_t) );\n\
\n\
    DWORD tmp;\n\
    HANDLE input = GetStdHandle(STD_INPUT_HANDLE);\n\
\n\
    int str_full_len = sizeof(wchar_t);\n\
    wchar_t *str_full = malloc( str_full_len );\n\
    wcsncpy( str_full, L\"\\0\", str_full_len/sizeof(wchar_t) );\n\
\n\
    SetConsoleMode(input, 0);\n\
\n\
    while ( 1 ) {\n\
        ReadConsoleW( input, str, str_len, &tmp, NULL );\n\
        wprintf( L\"%S\", str );\n\
        str_full_len += sizeof(wchar_t);\n\
        str_full = realloc( str_full, str_full_len );\n\
        wcsncat( str_full, str, str_len/sizeof(wchar_t) );\n\
\n\
        if ( str[0]==L'\\n' || str[0]==L'\\r' || str[0]==L'\\f'  ) {\n\
            str_full[ str_full_len/sizeof(wchar_t)-2 ] = L'\\n';\n\
            break;\n\
        }\n\
    }\n\
    printf(\"\\n\");\n\
    str_full_len += sizeof(wchar_t);\n\
    str_full = realloc( str_full, str_full_len );\n\
    str_full[str_full_len/sizeof(wchar_t)-1] = L'\\0';\n\
\n\
    free( str );\n\
\n\
    return str_full;\n\
}\n";
    fputws(c_code, c);


	fclose(c);
}

