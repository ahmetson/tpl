#ifndef TOKEN_TOKEN_TYPES_BLOCK_INSIDE_H
#define TOKEN_TOKEN_TYPES_BLOCK_INSIDE_H

#ifndef TOK_CLASS_BLOCK_INSIDE_NUM
#define TOK_CLASS_BLOCK_INSIDE_NUM 1
#endif // TOK_CLASS_BLOCK_NUM

#ifndef TOK_CLASS_BLOCK_INSIDE_FN_RETURN_CHAR
#define TOK_CLASS_BLOCK_INSIDE_FN_RETURN_CHAR L"yza"
#endif

#include "../token_structs.h"

extern int     TOK_CLASS_BLOCK_INSIDE_FN_RETURN;

extern wchar_t *TOK_CLASS_BLOCK_INSIDE_CHARS[ TOK_CLASS_BLOCK_INSIDE_NUM ][3];

int is_token_block_inside        (token *tok, wchar_t *tok_val);
void tok_block_inside_c_code(token *tok, wchar_t **l, int *llen);

#endif // TOKEN_TOKEN_TYPES_BLOCK_INSIDE_H
