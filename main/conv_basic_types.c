#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conv_basic_types.h"
#include "../translator_to_c.h"
#include "../error.h"
#include "../pragma/pragma.h"
#include "../token/harpl.h"

wchar_t             *CONV_DEF_TYPES_SOURCE_NAME_NO_H = L"_tpl_conv_def_types";
wchar_t             *CONV_DEF_TYPES_SOURCE_NAME      = L"_tpl_conv_def_types.h";
wchar_t             *CONV_DEF_TYPES_C_SOURCE_NAME    = L"_tpl_conv_def_types.c";


void add_conv_basic_type_c_code_file()
{
    wchar_t *dquote = L"\\";
    wchar_t h_path[MAX_FILE_LEN] = {0};
	wcsncat(h_path, C_SOURCE_FOLDER, wcslen(C_SOURCE_FOLDER));
	wcsncat(h_path, dquote, wcslen(dquote));
	wcsncat(h_path, CONV_DEF_TYPES_SOURCE_NAME, wcslen(CONV_DEF_TYPES_SOURCE_NAME));

	FILE *h_source = _wfopen(h_path, L"w, ccs=UTF-8");

	prepare_h_source(h_source, CONV_DEF_TYPES_SOURCE_NAME_NO_H);

    wchar_t *h_code = L"\
\n\
#ifndef INT_DECIMAL_STRING_SIZE  \n\
#define INT_DECIMAL_STRING_SIZE(int_type_size) ((CHAR_BIT*int_type_size-1)*10/33+3)\n\
#endif\n\
#include <stdio.h>\n\
#include <limits.h>\n\
\n\
extern wchar_t _tpl_tmp_str[INT_DECIMAL_STRING_SIZE(sizeof(int))];\n\
\n\
extern wchar_t **_tpl_str;\n\
extern int _tpl_str_num;\n\
\n\
float _tpl_conv_if(int s);\n\
wchar_t _tpl_conv_ic(int s);\n\
wchar_t * _tpl_conv_ia(int s);\n\
\n\
int _tpl_conv_fi(float d);\n\
wchar_t _tpl_conv_fc(float d);\n\
wchar_t * _tpl_conv_fa(float d);\n\
\n\
int _tpl_conv_ci(wchar_t c);\n\
wchar_t _tpl_conv_cf(wchar_t h);\n\
wchar_t * _tpl_conv_ca(wchar_t h);\n\
\n\
int _tpl_conv_ai(wchar_t *hl);\n\
float _tpl_conv_af(wchar_t *hl);\n\
wchar_t _tpl_conv_ac(wchar_t *hl);\n\
\n\
void _tpl_free_tmp_str();\n";
    fputws(h_code, h_source);

	finishize_h_source(h_source);

	fclose(h_source);


    wchar_t c_path[MAX_FILE_LEN] = {0};
	wcsncat(c_path, C_SOURCE_FOLDER, wcslen(C_SOURCE_FOLDER));
	wcsncat(c_path, dquote, wcslen(dquote));
	wcsncat(c_path, CONV_DEF_TYPES_C_SOURCE_NAME, wcslen(CONV_DEF_TYPES_C_SOURCE_NAME));

    FILE *c_source = _wfopen(c_path, L"w, ccs=UTF-8");


    wchar_t *c_code = L"#include <stdio.h>\n\
#include <stdlib.h>\n\
#include <string.h>\n\
#include <wchar.h>\n\
#include \"_tpl_conv_def_types.h\"\n\
\n\
\n\
wchar_t _tpl_tmp_str[INT_DECIMAL_STRING_SIZE(sizeof(int))] = {0} ;\n\
\n\
wchar_t **_tpl_str = NULL;\n\
int _tpl_str_num = 0;\n\
\n\
\n\
float _tpl_conv_if(int s)\n\
{\n\
    return (float)s;\n\
}\n\
wchar_t _tpl_conv_ic(int s)\n\
{\n\
    int delete_bits = (sizeof(s)-1)*8;\n\
    int s2 = s<<delete_bits; // Iň kiçi baýtdan öňküleri nollaýas\n\
        s2 = s>>delete_bits; // Iň kiçi baýty yza geçirýäs\n\
    return s2;\n\
}\n\
wchar_t * _tpl_conv_ia(int s)\n\
{\n\
    wchar_t hl[INT_DECIMAL_STRING_SIZE(sizeof(s))] = {0};\n\
    swprintf(hl, \"%d\", s);\n\
\n\
    ++_tpl_str_num;\n\
    _tpl_str = realloc(_tpl_str, sizeof(*_tpl_str)*_tpl_str_num);\n\
    _tpl_str[_tpl_str_num-1] = malloc(sizeof(**_tpl_str)*INT_DECIMAL_STRING_SIZE(sizeof(s)));\n\
    wcsncpy(_tpl_str[_tpl_str_num-1], hl, wcslen(hl)+1);\n\
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
wchar_t _tpl_conv_fc(float d)\n\
{\n\
    int s;\n\
    s = _tpl_conv_fi(d);\n\
    return _tpl_conv_ic(s);\n\
}\n\
\n\
wchar_t * _tpl_conv_fa(float d)\n\
{\n\
    wchar_t hl[100] = {0};\n\
    swprintf(hl, \"%f\", d);\n\
\n\
    ++_tpl_str_num;\n\
    _tpl_str = realloc(_tpl_str, sizeof(*_tpl_str)*_tpl_str_num);\n\
    _tpl_str[_tpl_str_num-1] = malloc(sizeof(**_tpl_str)*100);\n\
    wcsncpy(_tpl_str[_tpl_str_num-1], hl, wcslen(hl)+1);\n\
    return _tpl_str[_tpl_str_num-1];\n\
}\n\
\n\
\n\
\n\
int _tpl_conv_ci(wchar_t c)\n\
{\n\
    return (int)c;\n\
}\n\
wchar_t _tpl_conv_cf(wchar_t h)\n\
{\n\
    int s = _tpl_conv_ci(h);\n\
    return  _tpl_conv_if(s);\n\
}\n\
\n\
wchar_t * _tpl_conv_ca(wchar_t h)\n\
{\n\
    #define L sizeof(h)*2\n\
    wchar_t hl[L] = {0};\n\
    hl[0]=h;\n\
\n\
    ++_tpl_str_num;\n\
    _tpl_str = realloc(_tpl_str, sizeof(*_tpl_str)*_tpl_str_num);\n\
    _tpl_str[_tpl_str_num-1] = malloc(sizeof(**_tpl_str)*L);\n\
    wcsncpy(_tpl_str[_tpl_str_num-1], hl, wcslen(hl)+1);\n\
    return _tpl_str[_tpl_str_num-1];\n\
}\n\
\n\
\n\
int _tpl_conv_ai(wchar_t *hl)\n\
{\n\
\twchar_t *stop_str;\n\
    return (int)wcstol(hl, &stop_str, 10);\n\
}\n\
float _tpl_conv_af(wchar_t *hl)\n\
{\n\
\twchar_t *stop_str;\n\
    return  wcstof(hl, &stop_str);\n\
}\n\
\n\
wchar_t _tpl_conv_ac(wchar_t *hl)\n\
{\n\
    return hl[0];\n\
}\n\
\n\
\n";

    fputws(c_code, c_source);

    fclose(c_source);
}


void add_conv_basic_free_atexit(FILE *f)
{
    wchar_t *line = L"atexit(_tpl_free_tmp_str);";
    wchar_t *l = NULL;
    int llen = 0;
    write_code_line(f, &l, &llen , line);
}

void add_conv_basic_prepere_atexit(FILE *f)
{
    wchar_t *line = L"#include <stdlib.h>\n";
    wchar_t *l = NULL;
    int llen = 0;
    write_code_line(f, &l, &llen , line);
}

void add_turkmen_locale_support_lib(FILE *f)
{
    wchar_t *line = L"#include <locale.h>\n";
    wchar_t *l = NULL;
    int llen = 0;
    write_code_line(f, &l, &llen , line);
}

