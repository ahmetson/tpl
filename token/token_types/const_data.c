/**
 * Konstanta maglumatlar, dört sany.
**/
#include <string.h>
#include <stdlib.h>
#include "const_data.h"
#include "../../main/glob.h"
#include "../token_types.h"


// DEBUG UCHIN
// Eger täze element goşulsa, onda token/token_types/const_data.h faýlda, bu sanawyň prototipinde-de
// Elementleriň sanyny bire ulaltmaly
char *CONST_DATA_TOK_NUM_WORDS[4] = {
    "integer",
    "float",
    "char",
    "string"
};


/**
 *  1-nji konstanta maglumat tipi - san.
 *
 *  Bu token tipi sanlar bilen işleýän komandalarda ulanylyp bilinýär.
**/
char CHAR_MINUS = '-';
int INT_CONST_DATA_TOK_NUM = 0;


/**
 *  2-nji konstanta maglumat tipi - drob.
 *
 *  Bu token tipi droblar bilen işleýän komandalarda ulanylyp bilinýär.
**/
int FLOAT_CONST_DATA_TOK_NUM = 1;
char FLOAT_CONST_DATA_SEPARATOR = '_';              // Sanlary bölüji
char FLOAT_CONST_DATA_C_CODE_SEPARATOR = '.';       // Ýasaljak kodda sanlary bölüji

/**
 *  3-nji konstanta maglumat tipi - harp.
 *
 *  Bu token tipi harplar bilen işleýän komandalarda ulanylyp bilinýär.
 *  Deňdir belgisi indiki harpyň ýörite manyny berýändigini aňladýar
**/
char CHAR_CONST_DATA_OPENER = '\'';
int CHAR_CONST_DATA_TOK_NUM = 2;

/**
 *  4-nji konstanta maglumat tipi - harplar.
 *
 *  Bu token tipi harplar bilen işleýän komandalarda ulanylyp bilinýär.
 *  Deňdir belgisi indiki harpyň ýörite manyny berýändigini aňladýar
**/
int STRING_CONST_DATA_TOK_NUM = 3;

/**
 * Umumy token tiplerine degişli
**/
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
    else
    {
        *left_class = 0;
        *left_type=0;   // Natanysh
    }
    return 1;
}


char *get_const_data_string(token *tok)
{
    if (tok->potentional_types[0].type_num==INT_CONST_DATA_TOK_NUM ||
        tok->potentional_types[0].type_num==FLOAT_CONST_DATA_TOK_NUM)
        return tok->potentional_types[0].value;
    else if (tok->potentional_types[0].type_num==CHAR_CONST_DATA_TOK_NUM)
    {
        if (tok->potentional_types[0].value[1]=='\\')
        {
            char *backslash = "'\\\\'";
            return backslash;
        }
        return tok->potentional_types[0].value;
    }
    else if (tok->potentional_types[0].type_num==STRING_CONST_DATA_TOK_NUM)
    {
        return *tok->potentional_types[0].string_value;
    }
    return "";
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

int get_tok_type_const_data_char_val_type(token *tok, int *tok_class, int *tok_type)
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


void tok_int_c_code(token *tok, char **l, int *llen)
{
    *llen += strlen(tok->potentional_types[0].value);
    *l = realloc(*l, *llen);
    strncat(*l, tok->potentional_types[0].value, strlen(tok->potentional_types[0].value));
}
void tok_float_c_code(token *tok, char **l, int *llen)
{
    *llen += strlen(tok->potentional_types[0].value);
    *l = realloc(*l, *llen);
    strncat(*l, tok->potentional_types[0].value, strlen(tok->potentional_types[0].value));
}
void tok_char_c_code(token *tok, char **l, int *llen)
{
    *llen += strlen(tok->potentional_types[0].value);
   *l = realloc(*l, *llen);
    strncat(*l, tok->potentional_types[0].value, strlen(tok->potentional_types[0].value));
}
void tok_string_c_code(token *tok, char **l, int *llen)
{
    *llen += strlen(*tok->potentional_types[0].string_value);
    *l = realloc(*l, *llen);
    strncat(*l, *tok->potentional_types[0].string_value, strlen(*tok->potentional_types[0].string_value));
}
