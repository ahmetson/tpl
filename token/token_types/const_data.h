/**
 * Konstanta maglumatlar, dört sany.
**/
#ifndef TOKEN_TOKEN_TYPES_CONST_DATA_H
#define TOKEN_TOKEN_TYPES_CONST_DATA_H
#include "../token_structs.h"

// 1-nji konstanta maglumat tipi - san.
extern char CHAR_MINUS;
extern int INT_CONST_DATA_TOK_NUM;

// 2-nji konstanta maglumat tipi - drob.
extern int FLOAT_CONST_DATA_TOK_NUM;
extern char FLOAT_CONST_DATA_SEPARATOR;
extern char FLOAT_CONST_DATA_C_CODE_SEPARATOR;

// 3-nji konstanta maglumat tipi - harp
extern int CHAR_CONST_DATA_TOK_NUM;
extern char CHAR_CONST_DATA_OPENER;

// 4-nji konstanta maglumat tipi - harplar
extern int STRING_CONST_DATA_TOK_NUM;

// Debag üçin
extern char *CONST_DATA_TOK_NUM_WORDS[4];

int set_def_type_alias_const_data(int *left_class, int *left_type);


// TRANSLATOR TO C
char *get_const_data_string(token *tok);

int get_tok_type_const_data_int_val_type(token *tok, int *tok_class, int *tok_type);
int get_tok_type_const_data_float_val_type(token *tok, int *tok_class, int *tok_type);
int get_tok_type_const_data_char_val_type(token *tok, int *tok_class, int *tok_type);
int get_tok_type_const_data_string_val_type(token *tok, int *tok_class, int *tok_type);


void tok_int_c_code(token *tok, char **l, int *llen);
void tok_float_c_code(token *tok, char **l, int *llen);
void tok_char_c_code(token *tok, char **l, int *llen);
void tok_string_c_code(token *tok, char **l, int *llen);

#endif // TOKEN_TOKEN_TYPES_CONST_DATA_H
