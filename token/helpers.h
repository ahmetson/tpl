#ifndef TOKEN_HELPERS_H
#define TOKEN_HELPERS_H
#include "token_structs.h"

token get_empty_tok();

void empty_tok_c_code(token *tok, wchar_t **l, int *llen);

int empty_tok_return_type(token *tok, int *tok_class, int *tok_num);

void check_tok_len( wchar_t *tokval, token *tok );

int  is_token_empty(token *tok);

int get_potentional_token_type_class( token *tok, int potentional_type_num );
int get_potentional_token_type( token *tok, int potentional_type_num );
int get_token_type_class( token *tok );
int get_token_type( token *tok );

int is_complete_token( token *tok );

wchar_t *get_token_value ( token *tok );

#endif // TOKEN_HELPERS_H
