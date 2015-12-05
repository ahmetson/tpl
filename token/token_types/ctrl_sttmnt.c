#include "ctrl_sttmnt.h"
#include <stdlib.h>
#include <string.h>

int TOK_CLASS_CTRL_STTMNT_IF_NUM = 0;
int TOK_CLASS_CTRL_STTMNT_IFELSE_NUM = 1;
int TOK_CLASS_CTRL_STTMNT_ELSE_NUM = 2;
int TOK_CLASS_CTRL_STTMNT_CLOSE_BLOCK_NUM = 3;

char *TOK_CLASS_CTRL_STTMNT_CHARS[TOK_CLASS_CTRL_STTMNT_NUM][2] = {
    {IF_WORD,  "if "},
    {IFELSE_WORD,  "} else if "},
    {ELSE_WORD,  "} else {"},
    {CTRL_STTMNT_CLOSE_BLOCK,  " {"}
};

// DEBUG UCHIN
char *TOK_CLASS_CTRL_STTMNT_WORDS[TOK_CLASS_CTRL_STTMNT_NUM] = {
	"eger",
	"ýa",
	"ýogsa",
	"bolsa"
};


void tok_ctrl_sttmnt_c_code(token *tok, char **l, int *llen)
{

    *llen += strlen(TOK_CLASS_CTRL_STTMNT_CHARS[tok->potentional_types[0].type_num][1]);
    if (!(*llen-strlen(TOK_CLASS_CTRL_STTMNT_CHARS[tok->potentional_types[0].type_num][1])))
    {
        *llen += 1;
        *l = realloc(*l, *llen);
        strncpy(*l, TOK_CLASS_CTRL_STTMNT_CHARS[tok->potentional_types[0].type_num][1], strlen(TOK_CLASS_CTRL_STTMNT_CHARS[tok->potentional_types[0].type_num][1])+1);
    }
    else
    {
        *l = realloc(*l, *llen);
        strncat(*l, TOK_CLASS_CTRL_STTMNT_CHARS[tok->potentional_types[0].type_num][1], strlen(TOK_CLASS_CTRL_STTMNT_CHARS[tok->potentional_types[0].type_num][1]));
    }
}

