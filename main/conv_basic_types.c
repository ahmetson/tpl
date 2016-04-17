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


void add_conv_basic_type_c_code_file( FILE *c_file, FILE *h_file )
{
    wchar_t *dquote = L"\\";
    wchar_t *h_code = L"#ifndef INT_DECIMAL_STRING_SIZE  \n\
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
double _tpl_conv_if(int s);\n\
wchar_t _tpl_conv_ic(int s);\n\
wchar_t * _tpl_conv_ia(int s);\n\
\n\
int _tpl_conv_fi(double d);\n\
wchar_t _tpl_conv_fc(double d);\n\
wchar_t * _tpl_conv_fa(double d);\n\
\n\
int _tpl_conv_ci(wchar_t c);\n\
wchar_t _tpl_conv_cf(wchar_t h);\n\
wchar_t * _tpl_conv_ca(wchar_t h);\n\
\n\
int _tpl_conv_ai(wchar_t *hl);\n\
double _tpl_conv_af(wchar_t *hl);\n\
wchar_t _tpl_conv_ac(wchar_t *hl);\n\
\n\
void _tpl_free_tmp_str();\n\
void _tpl_call_another_files();\n";
    fputws( h_code, h_file );

    wchar_t *c_code = L"#include <stdio.h>\n\
#include <stdlib.h>\n\
#include <string.h>\n\
#include <wchar.h>\n\
wchar_t _tpl_tmp_str[INT_DECIMAL_STRING_SIZE(sizeof(int))] = {0} ;\n\
wchar_t **_tpl_str = NULL;\n\
int     _tpl_str_num = 0;\n\
double  _tpl_conv_if(int s)\n\ { return (double)s; }\n\
wchar_t _tpl_conv_ic( int s ) {\n\
    int delete_bits = (sizeof(s)-1)*8;\n\
    int s2 = s<<delete_bits; // Iň kiçi baýtdan öňküleri nollaýas\n\
        s2 = s>>delete_bits; // Iň kiçi baýty yza geçirýäs\n\
    return s2;\n\
}\n\
wchar_t * _tpl_conv_ia(int s) {\n\
    wchar_t hl[INT_DECIMAL_STRING_SIZE(sizeof(s))] = {0};\n\
    swprintf(hl, INT_DECIMAL_STRING_SIZE(sizeof(s))/sizeof(s), L\"%i\", s);\n\
\n\
    ++_tpl_str_num;\n\
    _tpl_str = realloc(_tpl_str, sizeof(*_tpl_str)*_tpl_str_num);\n\
    _tpl_str[_tpl_str_num-1] = malloc(sizeof(**_tpl_str)*INT_DECIMAL_STRING_SIZE(sizeof(s)));\n\
    wcsncpy(_tpl_str[_tpl_str_num-1], hl, wcslen(hl)+1);\n\
    return _tpl_str[_tpl_str_num-1]; }\n\
void _tpl_free_tmp_str() {\n\
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
int     _tpl_conv_fi(double d)  { return (int)d; }\n\
wchar_t _tpl_conv_fc(double d)  {  int s = _tpl_conv_fi(d); return _tpl_conv_ic(s); }\n\
wchar_t *_tpl_conv_fa(double d) {\n\
    wchar_t hl[100] = {0};\n\
    ++_tpl_str_num;\n\
    _tpl_str = realloc(_tpl_str, sizeof(*_tpl_str)*_tpl_str_num);\n\
    _tpl_str[_tpl_str_num-1] = malloc(sizeof(**_tpl_str)*100);\n\
    wcsncpy(_tpl_str[_tpl_str_num-1], hl, wcslen(hl)+1);\n\
    return _tpl_str[_tpl_str_num-1];}\n\
int     _tpl_conv_ci(wchar_t c) {\n\ return (int)c;}\n\
wchar_t _tpl_conv_cf(wchar_t h) {int s = _tpl_conv_ci(h); return  _tpl_conv_if(s); }\n\
wchar_t *_tpl_conv_ca(wchar_t h){\n\
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
int     _tpl_conv_ai(wchar_t *hl)   { wchar_t *stop_str; return (int)wcstol(hl, &stop_str, 10); }\n\
double  _tpl_conv_af(wchar_t *hl)   { wchar_t *stop_str; return  (double)wcstof(hl, &stop_str); }\n\
wchar_t _tpl_conv_ac(wchar_t *hl)   { return hl[0]; }\n";

    fputws( c_code, c_file );
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

