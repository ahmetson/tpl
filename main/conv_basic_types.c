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
    //wchar_t *dquote = L"\\";
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
void _tpl_call_another_files();\n\
void _tpl_multi_free( int addr_num, ... );\n\
void _tpl_glob_vars_free(  );\n";
    fputws( h_code, h_file );

    wchar_t *c_code = L"/** Integer to float */\n\
double  _tpl_conv_if( int s ) {\n\
   return (double)s;\n\
}\n\
/** Integer to char  */\n\
wchar_t _tpl_conv_ic( int s ) {\n\
    int delete_bits = (sizeof(s)-1)*8;\n\
    int s2 = s<<delete_bits; // Set zero to variable `s`, except least single byte\n\
        s2 = s>>delete_bits; // For setting zero, shift least byte to left corner.\n\
                             // Then move least byte to original position. \n\
    return s2;\n\
}\n\
/** Integer to String */\n\
wchar_t * _tpl_conv_ia( int s ) {\n\
    wchar_t hl[INT_DECIMAL_STRING_SIZE(sizeof(s))] = {0};\n\
    //swprintf(hl, INT_DECIMAL_STRING_SIZE(sizeof(s)), L\"%i\", s);\n\
    swprintf(hl, L\"%i\", s);\n\
\n\
    wchar_t *str = malloc( sizeof( wchar_t )*INT_DECIMAL_STRING_SIZE( sizeof( s ) ) );\n\
    wcsncpy( str, hl, wcslen( hl )+1 );\n\
    return str; \n\
}\n\
\n\
/** Float to integer  */\n\
int     _tpl_conv_fi( double d )  {\n\
    return (int)d; \n\
}\n\
\n\
/** Float to char  */\n\
wchar_t _tpl_conv_fc(double d)  {\n\
  int s = _tpl_conv_fi( d );\n\
  return _tpl_conv_ic( s );\n\
}\n\
/** Float to string  */\n\
wchar_t *_tpl_conv_fa( double d ) {\n\
    wchar_t hl[100] = {0};\n\
    //swprintf(hl, 100, L\"%f\", d);\n\
    swprintf(hl, L\"%f\", d);\n\
    wchar_t * str = malloc(sizeof( wchar_t )*100);\n\
    wcsncpy( str, hl, wcslen( hl )+1 );\n\
    return str;\n\
}\n\
/** Char to integer */\n\
int     _tpl_conv_ci( wchar_t c ) {\n\
    return (int)c;\n\
}\n\
/** Char to float */\n\
wchar_t _tpl_conv_cf( wchar_t h ) {\n\
    int s = _tpl_conv_ci(h); \n\
    return  _tpl_conv_if(s);\n\
}\n\
/** Char to string */\n\
wchar_t *_tpl_conv_ca(wchar_t h){\n\
    #define L sizeof(h)*2\n\
    wchar_t hl[ L ] = {0};\n\
    hl[0]=h;\n\
\n\
    wchar_t *str = malloc( sizeof( wchar_t )*2 );\n\
    str[0] = h;\n\
    str[1] = L'\\0';\n\
    return str;\n\
}\n\
/** String to integer */\n\
int     _tpl_conv_ai(wchar_t *hl)   {\n\
    wchar_t *stop_str; \n\
    return (int)wcstol( hl, &stop_str, 10 ); \n\
}\n\
/** String to float */\n\
double  _tpl_conv_af(wchar_t *hl)   {\n\
    wchar_t *stop_str; \n\
    return  (double)wcstof( hl, &stop_str ); \n\
}\n\
/** String to char */\n\
wchar_t _tpl_conv_ac(wchar_t *hl)   {\n\
 return hl[0]; \n\
}";

    fputws( c_code, c_file );
}


void add_conv_basic_free_atexit(FILE *f)
{
    wchar_t *line = L"//atexit();\n";
    wchar_t *l = NULL;
    int llen = 0;
    write_strings_to_file(f, &l, &llen , line);
}

void add_conv_basic_prepere_atexit(FILE *f)
{
    wchar_t *line = L"\n#include <stdlib.h>\n";
    wchar_t *l = NULL;
    int llen = 0;
    write_strings_to_file(f, &l, &llen , line);
}

