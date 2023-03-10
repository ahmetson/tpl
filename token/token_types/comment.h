#ifndef TOKEN_TOKEN_TYPES_COMMENT_H
#define TOKEN_TOKEN_TYPES_COMMENT_H
#include <wchar.h>
#include "../token_structs.h"

#ifndef TOK_CLASS_COMMENT_TYPES_NUM
#define TOK_CLASS_COMMENT_TYPES_NUM 2
#define TOK_CLASS_COMMENT_SINGLE_TYPE 0
#define TOK_CLASS_COMMENT_SINGLE_OPENER L"//"
#define TOK_CLASS_COMMENT_SINGLE_DEBUG L"Bir setir"
#define TOK_CLASS_COMMENT_BLOCK_TYPE  1
#define TOK_CLASS_COMMENT_BLOCK_OPENER  L"/*"
#define TOK_CLASS_COMMENT_BLOCK_CLOSER  L"*/"
#define TOK_CLASS_COMMENT_BLOCK_DEBUG  L"Blok"
#endif

int is_token_comment            (token *tok, wchar_t *tok_val);

#endif // TOKEN_TOKEN_TYPES_COMMENT_H
