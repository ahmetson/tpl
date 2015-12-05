#ifndef TOKEN_TOKEN_TYPES_BLOCK_H
#define TOKEN_TOKEN_TYPES_BLOCK_H

#include "../token_structs.h"

#ifndef TOK_CLASS_BLOCK_NUM
#define TOK_CLASS_BLOCK_NUM 1
#endif // TOK_CLASS_BLOCK_NUM

extern int TOK_CLASS_BLOCK_CLOSE_NUM;

extern char *TOK_CLASS_BLOCK_CHARS[TOK_CLASS_BLOCK_NUM][2];
extern char *TOK_CLASS_BLOCK_WORDS[TOK_CLASS_BLOCK_NUM];

void tok_block_c_code(token *tok, char **l, int *llen);

#endif // TOKEN_TOKEN_TYPES_BLOCK_H

