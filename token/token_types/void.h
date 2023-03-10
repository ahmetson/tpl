#ifndef TOKEN_TOKEN_TYPES_VOID_H
#define TOKEN_TOKEN_TYPES_VOID_H

#ifndef TOK_VOID_NUM
#define TOK_VOID_NUMS 1
#define TOK_VOID_NUM  0
#define TOK_VOID_NUM_KEYWORD L"hiç_zat"
#endif // TOK_VOID_NUM

#include "../token_structs.h"

extern wchar_t *TOK_VOID_CHARS[TOK_VOID_NUMS][3];

void tok_void_con_c_code(token *tok, wchar_t **l, int *llen);

int is_token_void               (token *tok, wchar_t *tok_val);


#endif // TOKEN_TOKEN_TYPES_VOID_H

