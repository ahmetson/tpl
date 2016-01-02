/**
*/
#include <stdlib.h>
#include <string.h>
#include "void.h"

char *TOK_VOID_CHARS[TOK_VOID_NUMS][3] = {
    {TOK_VOID_NUM_KEYWORD, "Boş tip", "void"} };

void tok_void_con_c_code(token *tok, char **l, int *llen)
{
    int type_num = tok->potentional_types[0].type_num;
    *llen += strlen(TOK_VOID_CHARS[type_num][2]);
    if (!(*llen-strlen(TOK_VOID_CHARS[type_num][2])))
    {
        *llen += 1;
        *l = realloc(*l, *llen);
        strncpy(*l, TOK_VOID_CHARS[type_num][2], strlen(TOK_VOID_CHARS[type_num][2])+1);
    }
    else
    {
        *l = realloc(*l, *llen);
        strncat(*l, TOK_VOID_CHARS[type_num][2], strlen(TOK_VOID_CHARS[type_num][2]));
    }
}
