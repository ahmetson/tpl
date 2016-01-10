#include <string.h>
#include <stdlib.h>
#include "utype_con.h"
#include "../../main/user_def_type.h"
#include "../../fns.h"


void tok_utype_con_c_code(token *tok, wchar_t **l, int *llen)
{
    int utype_num = tok->potentional_types[0].type_num;
    wcsadd_on_heap( l, llen, USER_DEF_TYPES[utype_num].ident );
}


