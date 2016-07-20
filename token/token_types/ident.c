/** Identifikatoryň maglumatlary
**/
#include <stdlib.h>
#include <string.h>
#include "ident.h"
#include "../../main/glob.h"
#include "../../fns/fn_helpers.h"
#include "../../semantic/compare_token_types.h"
#include "../token_types.h"
#include "../../tokens.h"
#include "../keywords.h"
#include "../../fns.h"
#include "../../fns/fn_helpers.h"

// token type: <class> <number>
int get_tok_type_ident_ident_val_type(token *tok, int *tok_class, int *tok_type)
{
    if (is_inside_fn())
    {
        if ( is_tmp_fn_arg_ident_used( tok->potentional_types[0].value ) )
        {
            get_tmp_fn_arg_value_type( tok->potentional_types[0].value, tok_class, tok_type );
            return 1;
        }
        if (is_tmp_fn_var_ident_used(tok->potentional_types[0].value))
        {
            get_tmp_fn_var_def_value_type(tok->potentional_types[0].value, tok_class, tok_type );
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
    if ( is_inside_loc_fn() )
    {
        if ( is_loc_fn_arg_ident_used( tok->potentional_types[0].value ) )
        {
            get_loc_fn_arg_value_type( tok->potentional_types[0].value, tok_class, tok_type );
            return 1;
        }
        if (is_loc_fn_var_ident_used(tok->potentional_types[0].value))
        {
            get_loc_fn_var_def_value_type(tok->potentional_types[0].value, tok_class, tok_type );
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


/** Tokeniň C kodyny gaýtarýar.*/
void tok_ident_c_code(token *tok, wchar_t **l, int *llen)
{
    wchar_t *val = tok->potentional_types[0].value;
    wcsadd_on_heap( l, llen, val );
}


int is_token_ident(token *tok, wchar_t *tok_val)
{
	token_type tok_type;
	//printf(L"Identifikator barlagy: %s\n", tok_val->val);

	// Identifikatorlar birinji harpy harp bolmaly
	if (!iswalpha(tok_val[0]))
		return 0;
	//printf(L"Identifikator harpdan bashlayar\n");

	int i;
	// Identifikator awchar_t sozi bolup bilenok
	for(i=0; i<MAX_KEYWORDS_NUM; i++)
	{
		if (wcslen(keywords[i])!=wcslen(tok_val))
			continue;
		//len = (wcslen(keywords[i])>wcslen(tok_val->val)) ? wcslen(keywords[i]) : wcslen(tok_val->val);

		//printf(L"For %s, Result of comparing: %0d\n", tok_val->val, wcsncmp(tok_val->val, keywords[i], wcslen(keywords[i])));
		if (wcsncmp(tok_val, keywords[i], wcslen(keywords[i]) )==0 )
			return 0;
	}
	//printf(L"Identifikator awchar_t sozi dal\n");

	// Identifikator harplardan, sanlardan we '_' belgiden durmaly
	for(i=0; i<wcslen(tok_val); i++)
	{
		if ( iswalnum(tok_val[i] )==0 && tok_val[i]!=L'_')
			return 0;
	}

	// This is - identifikator
	tok_type.type_num = 0;							// Number of token type

	tok_type.need_value = 1;
	tok_type.type_class = TOK_CLASS_IDENT;
	tok_type.parenthesis = 1;
	wcsncpy(tok_type.value, tok_val, wcslen(tok_val)+1);
	tok_type.is_compl = 1;
	tok_type.type_must_check = 0;

	add_potentional_token_type(tok, tok_type);

	// Tokene gornush girizilen son chagyrylmaly
	if (tok_type.is_compl==1)
		tok->type_class = TOK_CLASS_IDENT;

	return 1;
}

