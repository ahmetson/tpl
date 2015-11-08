/** Identifikatoryň maglumatlary
**/
#include <stdlib.h>
#include <string.h>
#include "ident.h"
#include "../../main/glob.h"
#include "../../semantic/compare_token_types.h"

// token type: <class> <number>
int get_tok_type_ident_ident_val_type(token *tok, int *tok_class, int *tok_type)
{
    //debug_token(tok);
    if(get_user_var_def_value_type(tok->potentional_types[0].value, tok_class, tok_type, FILES[tok->inf_file_num].source))
    {
        //set_def_type_alias_const_data(tok_class, tok_type);
        return 1;
    }
    *tok_class = TOK_CLASS_UNKNOWN;
    return 0;
}


/** Tokeniň C kodyny gaýtarýar.
**/
void tok_ident_c_code(token *tok, char **l, int *llen)
{
    *llen += strlen(tok->potentional_types[0].value);
    *l = realloc(*l, *llen);
    strncat(*l, tok->potentional_types[0].value, strlen(tok->potentional_types[0].value));
}
