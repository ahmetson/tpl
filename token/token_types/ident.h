/** Identifikatoryň maglumatlary
**/
#ifndef TOKEN_TOKEN_TYPES_IDENT_H
#define TOKEN_TOKEN_TYPES_IDENT_H

#include "../token_structs.h"

int get_tok_type_ident_ident_val_type(token *tok, int *tok_class, int *tok_type);

void tok_ident_c_code(token *tok, wchar_t **l, int *llen);

#endif // TOKEN_TOKEN_TYPES_IDENT_H
