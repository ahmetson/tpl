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

char *c_lib    = "\"_tpl_std_kabul_et.h\"";
char *c_name    = "_tpl_std_kabul_et";
char *h_source = "_tpl_std_kabul_et.h";
char *c_source = "_tpl_std_kabul_et.c";

int add_std_kabul_et()
{
    func std_kabul_et;
    std_kabul_et.file_num = -1;
    strncpy(std_kabul_et.file_name, "tpl_std.c", strlen("tpl_std.c")+1);
    strncpy(std_kabul_et.name, "kabul_et", strlen("kabul_et")+1);
    std_kabul_et.type_class = FUNC_CLASS_3RD_PARTY;
    std_kabul_et.type_num = FUNC_TYPE_STD;
    std_kabul_et.args_num = 0;     // * ýaly argumentleri kabul edýär
    std_kabul_et.inf_char = 0;
    std_kabul_et.inf_char_pos = 0;
    std_kabul_et.inf_line_pos = 0;
    std_kabul_et.return_class = TOK_CLASS_CONST_DATA;
    std_kabul_et.return_type = STRING_CONST_DATA_TOK_NUM;
    strncpy(std_kabul_et.c_name, c_name, strlen(c_name)+1);
    strncpy(std_kabul_et.c_lib, c_lib, strlen(c_lib)+1);
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
    char h_path[MAX_FILE_LEN] = {0};
	strncat(h_path, C_SOURCE_FOLDER, strlen(C_SOURCE_FOLDER));
	strncat(h_path, "\\", strlen("\\"));
	strncat(h_path, h_source, strlen(h_source));

    char c_path[MAX_FILE_LEN] = {0};
	strncat(c_path, C_SOURCE_FOLDER, strlen(C_SOURCE_FOLDER));
	strncat(c_path, "\\", strlen("\\"));
	strncat(c_path, c_source, strlen(c_source));

	FILE *h = fopen(h_path, "w");
	FILE *c = fopen(c_path, "w");

    char *h_code = "#ifndef _tpl_std_kabul_et_h\n\
#define _tpl_std_kabul_et_h\n\
\n\
char *_tpl_std_kabul_et();\n\
\n\
#endif\n";

    fputs(h_code, h);
    fclose(h);

    char *c_code = "#include <stdio.h>\n\
#include <stdlib.h>\n\
#include <string.h>\n\
#include \"_tpl_std_kabul_et.h\"\n\n\
char *_tpl_std_kabul_et()\n\
{\n\
	size_t size = 0;\n\
    char c = '\\0';\n\
    char *buf = NULL;\n\
\n\
	while ((c=fgetc(stdin)) != '\\n')\n\
    {\n\
    	/* BUFSIZ is defined as \"the optimal read size for this platform\" */\n\
        size += 1;\n\
        buf = realloc(buf, size); /* realloc(NULL,n) is the same as malloc(n) */\n\
		buf[size-1] = c;\n\
        //if (buf == NULL) return NULL;\n\
\n\
		//printf(\"%c\", c);\n\
    }\n\
    buf = realloc(buf, size+1); /* realloc(NULL,n) is the same as malloc(n) */\n\
	buf[size] = '\\0';\n\
    return buf;\n\
}\n";
    fputs(c_code, c);


	fclose(c);
}

