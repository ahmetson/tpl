#include <string.h>
#include <stdlib.h>
#include "utype_con.h"
#include "../../main/user_def_type.h"


void tok_utype_con_c_code(token *tok, char **l, int *llen)
{
    int utype_num = tok->potentional_types[0].type_num;
    *llen += strlen(USER_DEF_TYPES[utype_num].ident);
    if (!(*llen-strlen(USER_DEF_TYPES[utype_num].ident)))
    {
        *llen += 1;
        *l = realloc(*l, *llen);
        strncpy(*l, USER_DEF_TYPES[utype_num].ident, strlen(USER_DEF_TYPES[utype_num].ident)+1);
    }
    else
    {
        *l = realloc(*l, *llen);
        strncat(*l, USER_DEF_TYPES[utype_num].ident, strlen(USER_DEF_TYPES[utype_num].ident));
    }
}


