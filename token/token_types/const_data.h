/**
 * Konstanta maglumatlar, dört sany.
**/
#ifndef TOKEN_TOKEN_TYPES_CONST_DATA_H
#define TOKEN_TOKEN_TYPES_CONST_DATA_H
#include "../token_structs.h"

// 1-nji konstanta maglumat tipi - sanlar.
extern int INT_CONST_DATA_TOK_NUM;

// 2-nji konstanta maglumat tipi - droblar.
extern int FLOAT_CONST_DATA_TOK_NUM;
extern char FLOAT_CONST_DATA_SEPARATOR;
extern char FLOAT_CONST_DATA_C_CODE_SEPARATOR;

// 3-nji konstanta maglumat tipi - harplar
extern int CHAR_CONST_DATA_TOK_NUM;

// Debag üçin
extern char *CONST_DATA_TOK_NUM_WORDS[3];

void set_def_type_alias_const_data(int *left_class, int *left_type);


// TRANSLATOR TO C
char *get_const_data_string(token *tok);

#endif // TOKEN_TOKEN_TYPES_CONST_DATA_H
