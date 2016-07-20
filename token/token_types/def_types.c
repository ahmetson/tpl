/*
 * Programma dilde belli bolan sada tipler we olara degishli maglumatlar
**/
#include "def_types.h"
#include "../token_types.h"
#include "../../tokens.h"
#include "../../fns.h"

// Programmada belli bolan tiplerin sany
const int DEF_TYPES_NUM = 4;

// tiplering atlarynyn spisogy
def_type def_type_list[] = {
	{L"harp", L"wchar_t", L"L'\\0'"},
	{L"san", L"int", L"-1"},
	{L"drob", L"double", L"-1"},
	//{L"harpl", L"wchar_t *", L"L\"\""}
	{L"harpl", L"wchar_t *", L"NULL"}
};

/** Parsing edilende söz boýunça tokeni tanaýar*/
int is_token_def_type(token *tok, wchar_t *tok_val)
{
	/*Go through array of possible types*/
	int i, answer, found = 0;
	token_type tok_type;

	for(i=0; i<DEF_TYPES_NUM; i++)
	{
		answer = wcsstr_by_offset(def_type_list[i].tk_name, tok_val, 0);
		if (answer>=0)
		{
			tok_type.type_num = i;							// Number of token type
			tok_type.type_class = TOK_CLASS_DEF_TYPE;
			tok_type.need_value = 0;
			tok_type.is_compl = (answer==0) ? 1 : 0;
			tok_type.type_must_check = 0;
			tok_type.parenthesis = 1;
			tok->is_compl = (answer==0) ? 1 : 0;

			add_potentional_token_type(tok, tok_type);

			// Tokene gornush girizilen son chagyrylmaly
			if (tok_type.is_compl==1)
				tok->type_class = TOK_CLASS_DEF_TYPE;
			found = 1;
		}
	}

	return found;
}


