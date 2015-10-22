/**
 * Konstanta maglumatlar, dört sany.
**/
#include "const_data.h"
#include "../token_types.h"


/**
 *  1-nji konstanta maglumat tipi - sanlar.
 *
 *  Bu token tipi sanlar bilen işleýän komandalarda ulanylyp bilinýär.
**/
int INT_CONST_DATA_TOK_NUM = 1;

// DEBUG UCHIN
char *CONST_DATA_TOK_NUM_WORDS[1] = {
    "integer"
};


void set_def_type_alias_const_data(int *left_class, int *left_type)
{
    if (*left_class==TOK_CLASS_DEF_TYPE)
    {
        *left_class = TOK_CLASS_CONST_DATA;

        if (*left_type==1)  // San
            *left_type=INT_CONST_DATA_TOK_NUM;
    }
    else
    {
        *left_class = 0;
        *left_type=0;   // Natanysh
    }
}


char *get_const_data_string(token *tok)
{
    if (tok->potentional_types[0].type_num==INT_CONST_DATA_TOK_NUM)
        return tok->potentional_types[0].value;
}
