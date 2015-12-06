#include <stdlib.h>
#include <string.h>
#include "loop_sttmnt.h"

int TOK_CLASS_LOOP_STTMNT_UNTIL_NUM = 0;
int TOK_CLASS_LOOP_STTMNT_UNTIL_CLOSE_NUM = 1;

char *TOK_CLASS_LOOP_STTMNT_CHARS[TOK_CLASS_LOOP_STTMNT_NUM][2] = {
    {UNTIL_WORD,  "while (!"},
    {UNTIL_CLOSE_WORD,  ") {"}
};

// DEBUG UCHIN
char *TOK_CLASS_LOOP_STTMNT_WORDS[TOK_CLASS_LOOP_STTMNT_NUM] = {
	"tä",
	"bolýança"
};


void tok_loop_sttmnt_c_code(token *tok, char **l, int *llen)
{

    *llen += strlen(TOK_CLASS_LOOP_STTMNT_CHARS[tok->potentional_types[0].type_num][1]);
    if (!(*llen-strlen(TOK_CLASS_LOOP_STTMNT_CHARS[tok->potentional_types[0].type_num][1])))
    {
        *llen += 1;
        *l = realloc(*l, *llen);
        strncpy(*l, TOK_CLASS_LOOP_STTMNT_CHARS[tok->potentional_types[0].type_num][1], strlen(TOK_CLASS_LOOP_STTMNT_CHARS[tok->potentional_types[0].type_num][1])+1);
    }
    else
    {
        *l = realloc(*l, *llen);
        strncat(*l, TOK_CLASS_LOOP_STTMNT_CHARS[tok->potentional_types[0].type_num][1], strlen(TOK_CLASS_LOOP_STTMNT_CHARS[tok->potentional_types[0].type_num][1]));
    }
}

