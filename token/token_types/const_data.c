/**
 * Konstanta maglumatlar, dört sany.
**/
#include <string.h>
#include <stdlib.h>
#include "const_data.h"
#include "../../main/glob.h"
#include "../token_types.h"
#include "../harpl.h"
#include "../../fns.h"


// DEBUG UCHIN
// Eger täze element goşulsa, onda token/token_types/const_data.h faýlda, bu sanawyň prototipinde-de
// Elementleriň sanyny bire ulaltmaly
wchar_t *CONST_DATA_TOK_NUM_WORDS[4] = {
    L"integer",
    L"float",
    L"wchar_t",
    L"string"
};


/**
 *  1-nji konstanta maglumat tipi - san.
 *
 *  Bu token tipi sanlar bilen işleýän komandalarda ulanylyp bilinýär.
**/
wchar_t CHAR_MINUS = L'-';
int INT_CONST_DATA_TOK_NUM = 0;


/** 2-nji konstanta maglumat tipi - drob.
 *
 *  Bu token tipi droblar bilen işleýän komandalarda ulanylyp bilinýär.
**/
int FLOAT_CONST_DATA_TOK_NUM = 1;
wchar_t FLOAT_CONST_DATA_SEPARATOR = L'_';              // Sanlary bölüji
wchar_t FLOAT_CONST_DATA_C_CODE_SEPARATOR = L'.';       // Ýasaljak kodda sanlary bölüji

/** 3-nji konstanta maglumat tipi - harp.
 *
 *  Bu token tipi harplar bilen işleýän komandalarda ulanylyp bilinýär.
 *  Deňdir belgisi indiki harpyň ýörite manyny berýändigini aňladýar*/
wchar_t CHAR_CONST_DATA_OPENER = L'\'';
int CHAR_CONST_DATA_TOK_NUM = 2;

/** 4-nji konstanta maglumat tipi - harplar.
 *
 *  Bu token tipi harplar bilen işleýän komandalarda ulanylyp bilinýär.
 *  Deňdir belgisi indiki harpyň ýörite manyny berýändigini aňladýar*/
int STRING_CONST_DATA_TOK_NUM = 3;

/** Umumy token tiplerine degişli*/
int set_def_type_alias_const_data(int *left_class, int *left_type)
{
    if (*left_class==TOK_CLASS_DEF_TYPE)
    {
        *left_class = TOK_CLASS_CONST_DATA;

        if (*left_type==1)  // San
            *left_type=INT_CONST_DATA_TOK_NUM;
        else if (*left_type==2)
            *left_type=FLOAT_CONST_DATA_TOK_NUM;
        else if (*left_type==0)
            *left_type=CHAR_CONST_DATA_TOK_NUM;
        else if (*left_type==3)
            *left_type=STRING_CONST_DATA_TOK_NUM;
    }
    return 1;
}


wchar_t *get_const_data_string(token *tok)
{
    if (tok->potentional_types[0].type_num==INT_CONST_DATA_TOK_NUM ||
        tok->potentional_types[0].type_num==FLOAT_CONST_DATA_TOK_NUM)
        return tok->potentional_types[0].value;
    else if (tok->potentional_types[0].type_num==CHAR_CONST_DATA_TOK_NUM)
    {
        if (tok->potentional_types[0].value[1]==L'\\')
        {
            wchar_t *backslash = L"'\\\\'";
            return backslash;
        }
        return tok->potentional_types[0].value;
    }
    else if (tok->potentional_types[0].type_num==STRING_CONST_DATA_TOK_NUM)
    {
        return (wchar_t *)get_string_item(tok->potentional_types[0].string_value);
    }
    return L"";
}


int get_tok_type_const_data_int_val_type(token *tok, int *tok_class, int *tok_type)
{
    *tok_class = TOK_CLASS_CONST_DATA;
    *tok_type  = INT_CONST_DATA_TOK_NUM;
    return 1;
}

int get_tok_type_const_data_float_val_type(token *tok, int *tok_class, int *tok_type)
{
    *tok_class = TOK_CLASS_CONST_DATA;
    *tok_type  = FLOAT_CONST_DATA_TOK_NUM;
    return 1;
}

int get_tok_type_const_data_wchar_t_val_type(token *tok, int *tok_class, int *tok_type)
{
    *tok_class = TOK_CLASS_CONST_DATA;
    *tok_type  = CHAR_CONST_DATA_TOK_NUM;
    return 1;
}

int get_tok_type_const_data_string_val_type(token *tok, int *tok_class, int *tok_type)
{
    *tok_class = TOK_CLASS_CONST_DATA;
    *tok_type  = STRING_CONST_DATA_TOK_NUM;
    return 1;
}


void tok_int_c_code(token *tok, wchar_t **l, int *llen)
{
    wchar_t *val = tok->potentional_types[0].value;
    wcsadd_on_heap( l, llen, val );
}
void tok_float_c_code(token *tok, wchar_t **l, int *llen)
{
    wchar_t *val = tok->potentional_types[0].value;
    wcsadd_on_heap( l, llen, val );
}
void tok_wchar_t_c_code(token *tok, wchar_t **l, int *llen)
{
    wchar_t prev_l = L'L',
            *val = tok->potentional_types[0].value,
            *wchar_val = NULL;
    int len = sizeof(prev_l)+get_null_size();
    wchar_val = malloc( len );
    wchar_val[0] = prev_l;
    wchar_val[1] = L'\0';

    wcsadd_on_heap( &wchar_val, &len, val );

    wcsadd_on_heap( l, llen, wchar_val );

    free( wchar_val );
}
void tok_string_c_code(token *tok, wchar_t **l, int *llen)
{
    wchar_t *str = (wchar_t *)get_string_item( tok->potentional_types[0].string_value );
    wcsadd_on_heap( l, llen, str );
}
