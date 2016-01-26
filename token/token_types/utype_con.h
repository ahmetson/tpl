#ifndef TOKEN_TOKEN_TYPES_UTYPE_CON_H
#define TOKEN_TOKEN_TYPES_UTYPE_CON_H
#include "../token_structs.h"

void tok_utype_con_c_code(token *tok, wchar_t **l, int *llen);

int is_token_utype_con          (token *tok, wchar_t *tok_val);


#endif // TOKEN_TOKEN_TYPES_UTYPE_CON_H
