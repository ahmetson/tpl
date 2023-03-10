/**
 * Konstanta maglumatlar, dört sany.
**/
#ifndef TOKEN_TOKEN_TYPES_CONST_DATA_H
#define TOKEN_TOKEN_TYPES_CONST_DATA_H
#include "../token_structs.h"

// 1-nji konstanta maglumat tipi - san.
extern wchar_t CHAR_MINUS;
extern int INT_CONST_DATA_TOK_NUM;

// 2-nji konstanta maglumat tipi - drob.
extern int FLOAT_CONST_DATA_TOK_NUM;
extern wchar_t FLOAT_CONST_DATA_SEPARATOR;
extern wchar_t FLOAT_CONST_DATA_C_CODE_SEPARATOR;

// 3-nji konstanta maglumat tipi - harp
extern int CHAR_CONST_DATA_TOK_NUM;
extern wchar_t CHAR_CONST_DATA_OPENER;

// 4-nji konstanta maglumat tipi - harplar
extern int STRING_CONST_DATA_TOK_NUM;

// Debag üçin
extern wchar_t *CONST_DATA_TOK_NUM_WORDS[4];

int set_def_type_alias_const_data(int *left_class, int *left_type);


// TRANSLATOR TO C
wchar_t *get_const_data_string(token *tok);

int get_const_data_type(token *tok, int *tok_class, int *tok_type);


void tok_int_c_code(token *tok, wchar_t **l, int *llen);
void tok_float_c_code(token *tok, wchar_t **l, int *llen);
void tok_wchar_t_c_code(token *tok, wchar_t **l, int *llen);
void tok_string_c_code(token *tok, wchar_t **l, int *llen);

int is_token_int_const_data     (token *tok, wchar_t *tok_val);
int is_token_float_const_data   (token *tok, wchar_t *tok_val);
int is_token_wchar_t_const_data    (token *tok, wchar_t *tok_val);


#endif // TOKEN_TOKEN_TYPES_CONST_DATA_H
