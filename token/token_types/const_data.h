/**
 * Konstanta maglumatlar, dört sany.
**/
#ifndef TOKEN_TOKEN_TYPES_CONST_DATA_H
#define TOKEN_TOKEN_TYPES_CONST_DATA_H
#include "../token_structs.h"

// 1-nji konstanta maglumat tipi - san.
extern int INT_CONST_DATA_TOK_NUM;

// 2-nji konstanta maglumat tipi - drob.
extern int FLOAT_CONST_DATA_TOK_NUM;
extern char FLOAT_CONST_DATA_SEPARATOR;
extern char FLOAT_CONST_DATA_C_CODE_SEPARATOR;

// 3-nji konstanta maglumat tipi - harp
extern int CHAR_CONST_DATA_TOK_NUM;

// 4-nji konstanta maglumat tipi - harplar
extern int STRING_CONST_DATA_TOK_NUM;

// Debag üçin
extern char *CONST_DATA_TOK_NUM_WORDS[4];

void set_def_type_alias_const_data(int *left_class, int *left_type);


// TRANSLATOR TO C
char *get_const_data_string(token *tok);

#endif // TOKEN_TOKEN_TYPES_CONST_DATA_H
