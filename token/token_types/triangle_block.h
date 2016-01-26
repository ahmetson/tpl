#ifndef TOKEN_TOKEN_TYPES_TRIANGLE_BLOCK_H
#define TOKEN_TOKEN_TYPES_TRIANGLE_BLOCK_H

#define TRIANGLE_BLOCK_TOKENS_NUM 2
#define TOKEN_TRIANGLE_BLOCK_OPEN_TYPE 0
#define TOKEN_TRIANGLE_BLOCK_CLOSE_TYPE 1
#include <wchar.h>
#include "../token_structs.h"

extern wchar_t *TRIANGLE_BLOCK_TOKENS_KEYWORDS[TRIANGLE_BLOCK_TOKENS_NUM][2];

int is_token_triangle_block     (token *tok, wchar_t *tok_val);


#endif // TOKEN_TOKEN_TYPES_TRIANGLE_BLOCK_H
