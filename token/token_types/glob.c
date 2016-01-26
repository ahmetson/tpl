/*
 * Programma dilde global maglumatlar uchin ulanylyan maglumatlar
**/
#include <wchar.h>
#include "glob.h"
#include "../token_types.h"
#include "../../tokens.h"
#include "../../fns.h"


/// Global maglumatlar, öňünde duran ýörite token arkaly tanalýarlar.
wchar_t *glob_def_string = L"@";

wchar_t GLOB_IDENT_OPENER = L'@';

/** Parsing edilende söz boýunça tokeni tanaýar*/
int is_token_def_glob(token *tok, wchar_t *tok_val)
{
	int answer;
	token_type tok_type;

	answer = wcsstr_by_offset(glob_def_string, tok_val, 0);
	if (answer>=0)
	{
		tok_type.type_num = 0;							// Number of token type
		tok_type.type_class = TOK_CLASS_GLOB;
		tok_type.need_value = 0;
		tok_type.is_compl = (answer==0) ? 1 : 0;
		tok_type.type_must_check = 0;
        tok_type.parenthesis = 0;
		tok->is_compl = (answer==0) ? 1 : 0;

		add_potentional_token_type(tok, tok_type);

		// Tokene gornush girizilen son chagyrylmaly
		if (tok_type.is_compl==1)
			tok->type_class = TOK_CLASS_GLOB;

		return 1;
	}
	return 0;
}
