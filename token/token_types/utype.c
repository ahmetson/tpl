#include <stdlib.h>
#include <string.h>
#include "utype.h"
#include "../token_structs.h"
#include "../token_types.h"

char *TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[3] = {"/",  ".", "birlik bölüji"};

int get_utype(token *tok, int *tok_class, int *tok_type)
{

    *tok_class = TOK_CLASS_UTYPE_CON;
    *tok_type  = tok->potentional_types[0].type_num;
    return 1;
}

void utype_item_separator_c_code(token *tok, char **l, int *llen)
{
    *llen += strlen(TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[1]);
    if (!(*llen-strlen(TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[1])))
    {
        *l = realloc(*l, *llen+1);
        strncpy(*l, TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[1], strlen(TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[1])+1);
    }
    else
    {
        *l = realloc(*l, *llen);
        strncat(*l, TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[1], strlen(TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[1]));
    }
}
