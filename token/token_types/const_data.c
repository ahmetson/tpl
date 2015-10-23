/**
 * Konstanta maglumatlar, dört sany.
**/
#include "const_data.h"
#include "../token_types.h"


// DEBUG UCHIN
// Eger täze element goşulsa, onda token/token_types/const_data.h faýlda, bu sanawyň prototipinde-de
// Elementleriň sanyny bire ulaltmaly
char *CONST_DATA_TOK_NUM_WORDS[3] = {
    "integer",
    "float",
    "char"
};


/**
 *  1-nji konstanta maglumat tipi - sanlar.
 *
 *  Bu token tipi sanlar bilen işleýän komandalarda ulanylyp bilinýär.
**/
int INT_CONST_DATA_TOK_NUM = 1;


/**
 *  2-nji konstanta maglumat tipi - droblar.
 *
 *  Bu token tipi droblar bilen işleýän komandalarda ulanylyp bilinýär.
**/
int FLOAT_CONST_DATA_TOK_NUM = 2;
char FLOAT_CONST_DATA_SEPARATOR = '_';              // Sanlary bölüji
char FLOAT_CONST_DATA_C_CODE_SEPARATOR = '.';       // Ýasaljak kodda sanlary bölüji

/**
 *  3-nji konstanta maglumat tipi - harplar.
 *
 *  Bu token tipi harplar bilen işleýän komandalarda ulanylyp bilinýär.
 *  Deňdir belgisi indiki harpyň ýörite manyny berýändigini aňladýar
**/
int CHAR_CONST_DATA_TOK_NUM = 3;

/**
 * Umumy token tiplerine degişli
**/
void set_def_type_alias_const_data(int *left_class, int *left_type)
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
    }
    else
    {
        *left_class = 0;
        *left_type=0;   // Natanysh
    }
}


char *get_const_data_string(token *tok)
{
    if (tok->potentional_types[0].type_num==INT_CONST_DATA_TOK_NUM ||
        tok->potentional_types[0].type_num==FLOAT_CONST_DATA_TOK_NUM ||
        tok->potentional_types[0].type_num==CHAR_CONST_DATA_TOK_NUM)
        return tok->potentional_types[0].value;
}
