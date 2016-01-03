#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conv_basic_types.h"
#include "../translator_to_c.h"
#include "../error.h"
#include "../pragma/pragma.h"
#include "../token/harpl.h"

char             *CONV_DEF_TYPES_SOURCE_NAME_NO_H = "_tpl_conv_def_types";
char             *CONV_DEF_TYPES_SOURCE_NAME = "_tpl_conv_def_types.h";
char             *CONV_DEF_TYPES_C_SOURCE_NAME = "_tpl_conv_def_types.c";


void add_conv_basic_type_c_code_file()
{
    char h_path[MAX_FILE_LEN] = {0};
	strncat(h_path, C_SOURCE_FOLDER, strlen(C_SOURCE_FOLDER));
	strncat(h_path, "\\", strlen("\\"));
	strncat(h_path, CONV_DEF_TYPES_SOURCE_NAME, strlen(CONV_DEF_TYPES_SOURCE_NAME));

	FILE *h_source = fopen(h_path, "w");

	prepare_h_source(h_source, CONV_DEF_TYPES_SOURCE_NAME_NO_H);

    char *h_code = "\
\n\
#ifndef INT_DECIMAL_STRING_SIZE  \n\
#define INT_DECIMAL_STRING_SIZE(int_type_size) ((CHAR_BIT*int_type_size-1)*10/33+3)\n\
#endif\n\
#include <stdio.h>\n\
#include <limits.h>\n\
\n\
extern char _tpl_tmp_str[INT_DECIMAL_STRING_SIZE(sizeof(int))];\n\
\n\
extern char **_tpl_str;\n\
extern int _tpl_str_num;\n\
\n\
float _tpl_conv_if(int s);\n\
char _tpl_conv_ic(int s);\n\
char * _tpl_conv_ia(int s);\n\
\n\
int _tpl_conv_fi(float d);\n\
char _tpl_conv_fc(float d);\n\
char * _tpl_conv_fa(float d);\n\
\n\
int _tpl_conv_ci(char c);\n\
char _tpl_conv_cf(char h);\n\
char * _tpl_conv_ca(char h);\n\
\n\
int _tpl_conv_ai(char *hl);\n\
float _tpl_conv_af(char *hl);\n\
char _tpl_conv_ac(char *hl);\n\
\n\
void _tpl_free_tmp_str();\n";
    fputs(h_code, h_source);

	finishize_h_source(h_source);

	fclose(h_source);


    char c_path[MAX_FILE_LEN] = {0};
	strncat(c_path, C_SOURCE_FOLDER, strlen(C_SOURCE_FOLDER));
	strncat(c_path, "\\", strlen("\\"));
	strncat(c_path, CONV_DEF_TYPES_C_SOURCE_NAME, strlen(CONV_DEF_TYPES_C_SOURCE_NAME));

    FILE *c_source = fopen(c_path, "w");


    char *c_code = "#include <stdio.h>\n\
#include <stdlib.h>\n\
#include <string.h>\n\
#include \"_tpl_conv_def_types.h\"\n\
\n\
\n\
char _tpl_tmp_str[INT_DECIMAL_STRING_SIZE(sizeof(int))] = {0} ;\n\
\n\
char **_tpl_str = NULL;\n\
int _tpl_str_num = 0;\n\
\n\
\n\
float _tpl_conv_if(int s)\n\
{\n\
    return (float)s;\n\
}\n\
char _tpl_conv_ic(int s)\n\
{\n\
    /// Diňe iň soňky bir baýty gerek\n\
    int delete_bits = (sizeof(s)-1)*8;\n\
    int s2 = s<<delete_bits; // Iň kiçi baýtdan öňküleri nollaýas\n\
        s2 = s>>delete_bits; // Iň kiçi baýty yza geçirýäs\n\
    return s2;\n\
}\n\
char * _tpl_conv_ia(int s)\n\
{\n\
    char hl[INT_DECIMAL_STRING_SIZE(sizeof(s))] = {0};\n\
    sprintf(hl, \"%d\", s);\n\
\n\
    ++_tpl_str_num;\n\
    _tpl_str = realloc(_tpl_str, sizeof(*_tpl_str)*_tpl_str_num);\n\
    _tpl_str[_tpl_str_num-1] = malloc(sizeof(**_tpl_str)*INT_DECIMAL_STRING_SIZE(sizeof(s)));\n\
    strncpy(_tpl_str[_tpl_str_num-1], hl, strlen(hl)+1);\n\
    return _tpl_str[_tpl_str_num-1];\n\
}\n\
void _tpl_free_tmp_str()\n\
{\n\
    if (!_tpl_str_num)\n\
    {\n\
        return;\n\
    }\n\
\n\
    int i;\n\
    for (i=0; i<_tpl_str_num; ++i)\n\
    {\n\
        free(_tpl_str[i]);\n\
    }\n\
    free(_tpl_str);\n\
}\n\
\n\
int _tpl_conv_fi(float d)\n\
{\n\
    return (double)d;\n\
}\n\
char _tpl_conv_fc(float d)\n\
{\n\
    int s;\n\
    s = _tpl_conv_fi(d);\n\
    return _tpl_conv_ic(s);\n\
}\n\
\n\
char * _tpl_conv_fa(float d)\n\
{\n\
    char hl[100] = {0};\n\
    sprintf(hl, \"%f\", d);\n\
\n\
    ++_tpl_str_num;\n\
    _tpl_str = realloc(_tpl_str, sizeof(*_tpl_str)*_tpl_str_num);\n\
    _tpl_str[_tpl_str_num-1] = malloc(sizeof(**_tpl_str)*100);\n\
    strncpy(_tpl_str[_tpl_str_num-1], hl, strlen(hl)+1);\n\
    return _tpl_str[_tpl_str_num-1];\n\
}\n\
\n\
\n\
\n\
int _tpl_conv_ci(char c)\n\
{\n\
    return (int)c;\n\
}\n\
char _tpl_conv_cf(char h)\n\
{\n\
    int s = _tpl_conv_ci(h);\n\
    return  _tpl_conv_if(s);\n\
}\n\
\n\
char * _tpl_conv_ca(char h)\n\
{\n\
    #define L sizeof(h)*2\n\
    char hl[L] = {0};\n\
    hl[0]=h;\n\
\n\
    ++_tpl_str_num;\n\
    _tpl_str = realloc(_tpl_str, sizeof(*_tpl_str)*_tpl_str_num);\n\
    _tpl_str[_tpl_str_num-1] = malloc(sizeof(**_tpl_str)*L);\n\
    strncpy(_tpl_str[_tpl_str_num-1], hl, strlen(hl)+1);\n\
    return _tpl_str[_tpl_str_num-1];\n\
}\n\
\n\
\n\
int _tpl_conv_ai(char *hl)\n\
{\n\
    return atoi(hl);\n\
}\n\
float _tpl_conv_af(char *hl)\n\
{\n\
    return  (float)atof(hl);\n\
}\n\
\n\
char _tpl_conv_ac(char *hl)\n\
{\n\
    return hl[0];\n\
}\n\
\n\
\n";

    fputs(c_code, c_source);

    fclose(c_source);
}


void add_conv_basic_free_atexit(FILE *f)
{
    char *line = "atexit(_tpl_free_tmp_str);";
    char *l = NULL;
    int llen = 0;
    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line);
}

void add_conv_basic_prepere_atexit(FILE *f)
{
    char *line = "/**Sada tipleri konwertlemek uchin*/\n#include <stdlib.h>\n";
    char *l = NULL;
    int llen = 0;
    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line);
}

