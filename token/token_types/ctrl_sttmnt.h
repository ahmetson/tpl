#ifndef TOKEN_TOKEN_TYPES_CTRL_STTMNT_H
#define TOKEN_TOKEN_TYPES_CTRL_STTMNT_H

#include "../token_structs.h"

#ifndef IF_WORD
#define IF_WORD "eger"
#define IFELSE_WORD "ya"
#define ELSE_WORD "yogsa"
#define CTRL_STTMNT_CLOSE_BLOCK "bolsa"
#endif // IF_WORD

#ifndef TOK_CLASS_CTRL_STTMNT_NUM
#define TOK_CLASS_CTRL_STTMNT_NUM 4
#endif // TOK_CLASS_CTRL_STTMNT_NUM

extern int TOK_CLASS_CTRL_STTMNT_IF_NUM;
extern int TOK_CLASS_CTRL_STTMNT_IFELSE_NUM;
extern int TOK_CLASS_CTRL_STTMNT_ELSE_NUM;
extern int TOK_CLASS_CTRL_STTMNT_CLOSE_BLOCK_NUM;

extern char *TOK_CLASS_CTRL_STTMNT_CHARS[TOK_CLASS_CTRL_STTMNT_NUM][2];
extern char *TOK_CLASS_CTRL_STTMNT_WORDS[TOK_CLASS_CTRL_STTMNT_NUM];

void tok_ctrl_sttmnt_c_code(token *tok, char **l, int *llen);

#endif // TOKEN_TOKEN_TYPES_CTRL_STTMNT_H
