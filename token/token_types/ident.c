/** Identifikatoryň maglumatlary
**/
#include <stdlib.h>
#include <string.h>
#include "ident.h"
#include "../../main/glob.h"
#include "../../fns/fn_helpers.h"
#include "../../fns.h"
#include "../../semantic/compare_token_types.h"

// token type: <class> <number>
int get_tok_type_ident_ident_val_type(token *tok, int *tok_class, int *tok_type)
{
    if (is_inside_fn())
    {
        if (is_tmp_fn_var_ident_used(tok->potentional_types[0].value))
        {
            get_tmp_fn_var_def_value_type(tok->potentional_types[0].value, tok_class, tok_type);
            return 1;
        }
        else if (is_glob_var_def_exist(tok->potentional_types[0].value))
        {
            get_var_def_value_type(tok->potentional_types[0].value, tok_class, tok_type, 1);
            return 1;
        }
        else if (is_glob_var_dec_exist(tok->potentional_types[0].value))
        {
            get_glob_var_dec_value_type(tok->potentional_types[0].value, tok_class, tok_type);
            return 1;
        }
    }
    else
    {
        if (is_local_var_def_exist(tok->potentional_types[0].value))
        {
            get_var_def_value_type(tok->potentional_types[0].value, tok_class, tok_type, 0);
            return 1;
        }
        else if (is_glob_var_def_exist(tok->potentional_types[0].value))
        {
            get_var_def_value_type(tok->potentional_types[0].value, tok_class, tok_type, 1);
            return 1;
        }
        else if (is_glob_var_dec_exist(tok->potentional_types[0].value))
        {
            get_glob_var_dec_value_type(tok->potentional_types[0].value, tok_class, tok_type);
            return 1;
        }
    }

    *tok_class = TOK_CLASS_UNKNOWN;
    return 0;
}


/** Tokeniň C kodyny gaýtarýar.
**/
void tok_ident_c_code(token *tok, wchar_t **l, int *llen)
{
    wchar_t *val = tok->potentional_types[0].value;
    wcsadd_on_heap( l, llen, val );
}
