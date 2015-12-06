#ifndef TOKEN_TOKEN_TYPES_LOOP_STTMNT_H
#define TOKEN_TOKEN_TYPES_LOOP_STTMNT_H

#include "../token_structs.h"

#ifndef UNTIL_WORD
#define UNTIL_WORD "ta"
#define UNTIL_CLOSE_WORD "bolyancha"
#endif // UNTIL_WORD

#ifndef TOK_CLASS_LOOP_STTMNT_NUM
#define TOK_CLASS_LOOP_STTMNT_NUM 2
#endif // TOK_CLASS_LOOP_STTMNT_NUM

extern int TOK_CLASS_LOOP_STTMNT_UNTIL_NUM;
extern int TOK_CLASS_LOOP_STTMNT_UNTIL_CLOSE_NUM;

extern char *TOK_CLASS_LOOP_STTMNT_CHARS[TOK_CLASS_LOOP_STTMNT_NUM][2];
extern char *TOK_CLASS_LOOP_STTMNT_WORDS[TOK_CLASS_LOOP_STTMNT_NUM];

void tok_loop_sttmnt_c_code(token *tok, char **l, int *llen);

#endif // TOKEN_TOKEN_TYPES_LOOP_STTMNT_H
