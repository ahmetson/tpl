#include <string.h>
#include <stdlib.h>
#include "utype_con.h"
#include "../../main/user_def_type.h"
#include "../token_types.h"
#include "../../tokens.h"
#include "../../fns.h"


void tok_utype_con_c_code(token *tok, wchar_t **l, int *llen)
{
    int utype_num = tok->potentional_types[0].type_num;
    wcsadd_on_heap( l, llen, USER_DEF_TYPES[utype_num].ident );
}

int is_token_utype_con(token *tok, wchar_t *tok_val)
{
    if (is_utype_ident(tok_val))
    {
        token_type tok_type;
        get_utype_addr(tok_val, &tok_type.type_num);							// Number of token type
        tok_type.type_class = TOK_CLASS_UTYPE_CON;
        tok_type.need_value = 1;
        wcsncpy(tok_type.value, tok_val, wcslen(tok_val)+1);
        tok_type.is_compl = 1;
        tok_type.type_must_check = 0;
        tok_type.parenthesis = 0;
        tok->is_compl = 1;
        tok->type_class = TOK_CLASS_UTYPE_CON;

        add_potentional_token_type(tok, tok_type);
        return 1;
    }
    return 0;
}



