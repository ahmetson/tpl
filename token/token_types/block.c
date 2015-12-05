#include "block.h"
#include <stdlib.h>
#include <string.h>

int TOK_CLASS_BLOCK_CLOSE_NUM = 0;

char *TOK_CLASS_BLOCK_CHARS[TOK_CLASS_BLOCK_NUM][2] = {
    {"===",  "}"}
};

// DEBUG UCHIN
char *TOK_CLASS_BLOCK_WORDS[TOK_CLASS_BLOCK_NUM] = {
	"ýapýan"
};


void tok_block_c_code(token *tok, char **l, int *llen)
{
    *llen += strlen(TOK_CLASS_BLOCK_CHARS[tok->potentional_types[0].type_num][1]);
    if (!(*llen-strlen(TOK_CLASS_BLOCK_CHARS[tok->potentional_types[0].type_num][1])))
    {
        *llen += 1;
        *l = realloc(*l, *llen);
        strncpy(*l, TOK_CLASS_BLOCK_CHARS[tok->potentional_types[0].type_num][1], strlen(TOK_CLASS_BLOCK_CHARS[tok->potentional_types[0].type_num][1])+1);
    }
    else
    {
        *l = realloc(*l, *llen);
        strncat(*l, TOK_CLASS_BLOCK_CHARS[tok->potentional_types[0].type_num][1], strlen(TOK_CLASS_BLOCK_CHARS[tok->potentional_types[0].type_num][1]));
    }
}

