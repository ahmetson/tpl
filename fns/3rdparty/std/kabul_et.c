/** 3rdParty/STD funksiýa
    Çap etmek
**/
#include <stdlib.h>
#include <string.h>

#include "../../fn.h"
#include "chap_et.h"
#include "../../../token/token_types.h"
#include "../../../token/token_types/const_data.h"
#include "../../../translator_to_c.h"

wchar_t *c_lib    = L"\"_tpl_std_kabul_et.h\"";
wchar_t *c_name    = L"_tpl_std_kabul_et";
wchar_t *h_source = L"_tpl_std_kabul_et.h";
wchar_t *c_source = L"_tpl_std_kabul_et.c";

int add_std_kabul_et()
{
    wchar_t *f_name = L"tpl_std.c",
            *fn_c_name = L"kabul_et";
    func std_kabul_et;
    std_kabul_et.file_num = -1;
    wcsncpy(std_kabul_et.file_name, f_name, wcslen(f_name)+1);
    wcsncpy(std_kabul_et.name, fn_c_name, wcslen(fn_c_name)+1);
    std_kabul_et.type_class = FUNC_CLASS_3RD_PARTY;
    std_kabul_et.type_num = FUNC_TYPE_STD;
    std_kabul_et.args_num = 0;     // * ýaly argumentleri kabul edýär
    std_kabul_et.inf_wchar_t = 0;
    std_kabul_et.inf_wchar_t_pos = 0;
    std_kabul_et.inf_line_pos = 0;
    std_kabul_et.return_class = TOK_CLASS_CONST_DATA;
    std_kabul_et.return_type = STRING_CONST_DATA_TOK_NUM;
    wcsncpy(std_kabul_et.c_name, c_name, wcslen(c_name)+1);
    wcsncpy(std_kabul_et.c_lib, c_lib, wcslen(c_lib)+1);
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
	wcsncat(h_path, C_SOURCE_FOLDER, wcslen(C_SOURCE_FOLDER));
	wcsncat(h_path, bslash, wcslen(bslash));
	wcsncat(h_path, h_source, wcslen(h_source));

    wchar_t c_path[MAX_FILE_LEN] = {0};
	wcsncat(c_path, C_SOURCE_FOLDER, wcslen(C_SOURCE_FOLDER));
	wcsncat(c_path, bslash, wcslen(bslash));
	wcsncat(c_path, c_source, wcslen(c_source));

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

    wchar_t *c_code = L"#include <stdio.h>\n\
#include <stdlib.h>\n\
#include <string.h>\n\
#include \"_tpl_std_kabul_et.h\"\n\n\
wchar_t *_tpl_std_kabul_et()\n\
{\n\
	size_t size = 0;\n\
    wchar_t c = L'\\0';\n\
    wchar_t *buf = NULL;\n\
\n\
	while ((c=fgetwc(stdin)) != L'\\n')\n\
    {\n\
    	/* BUFSIZ is defined as \"the optimal read size for this platform\" */\n\
        size += sizeof(c);\n\
        buf = realloc(buf, size); /* realloc(NULL,n) is the same as malloc(n) */\n\
		buf[size/sizeof(c)-1] = c;\n\
        //if (buf == NULL) return NULL;\n\
    }\n\
    size += +sizeof(c);\n\
    buf = realloc(buf, size); /* realloc(NULL,n) is the same as malloc(n) */\n\
	buf[size/sizeof(c)-1] = L'\\0';\n\
    return buf;\n\
}\n";
    fputws(c_code, c);


	fclose(c);
}

