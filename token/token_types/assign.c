/** Programma dilde ülňä maglumat baglama komandasynyn operatorlary

    Iki sany görnüşi bar:
        1) LEFT  : <-
        2) RIGHT : -> */
#include <stdlib.h>
#include <string.h>

#include "assign.h"
#include "../token_types.h"
#include "../../tokens.h"
#include "../../fns.h"

/// Token tipleriniň numerleri
int   LEFT_ASSIGN_TOK_NUM = 1,
      RIGHT_ASSIGN_TOK_NUM = 2;


wchar_t *TOK_CLASS_ASSIGN_CHARS[2][2] = {
    {L"<-", L"çepde ülňi"},
    {L"->", L"ülňi sagda"}
};

wchar_t *RIGHT_ASSIGN_C_CODES[2] = {L"=", L"{" };

void tok_assign_c_code(token *tok, wchar_t **l, int *llen)
{
    wchar_t *c_code;
    if (tok->potentional_types[0].type_num==RIGHT_ASSIGN_TOK_NUM &&
        tok->potentional_types[0].value[0]==L'1')
    {
        c_code = RIGHT_ASSIGN_C_CODES[1];
    }
    else
    {
        c_code = RIGHT_ASSIGN_C_CODES[0]; // Çep we sag tarapda ülňi bolan baglanmalaryň C kody.
    }

    wcsadd_on_heap( l, llen, c_code );
}


int is_token_assign(token *tok, wchar_t *tok_val)
{
    if (is_token_var_left_assign(tok, tok_val))
        return 1;
    return is_token_var_rigtht_assign(tok, tok_val);
}

// <-
int is_token_var_rigtht_assign(token *tok, wchar_t *tok_val)
{
    int answer;
    wchar_t *zero = L"0";
	token_type tok_type;

	answer = wcsstr_by_offset( TOK_CLASS_ASSIGN_CHARS[1][0], tok_val, 0);
	if (answer>=0)
	{
		tok_type.type_num = RIGHT_ASSIGN_TOK_NUM;	// Number of token type
		tok_type.type_class = TOK_CLASS_ASSIGN;
		tok_type.need_value = 1;
		wcsncpy(tok_type.value, zero, wcslen(zero)+1);
		tok_type.is_compl = (answer==0) ? 1 : 0;
		tok_type.type_must_check = 0;
		tok_type.parenthesis = 0;
		tok->is_compl = (answer==0) ? 1 : 0;

		add_potentional_token_type(tok, tok_type);

		// Tokene gornush girizilen son chagyrylmaly
		if (tok_type.is_compl==1)
			tok->type_class = TOK_CLASS_ASSIGN;

		return 1;
	}
	return 0;
}
int is_token_var_left_assign(token *tok, wchar_t *tok_val)
{
	int answer;
	token_type tok_type;

	answer = wcsstr_by_offset( TOK_CLASS_ASSIGN_CHARS[0][0], tok_val, 0);
	if (answer>=0)
	{
		tok_type.type_num = LEFT_ASSIGN_TOK_NUM;	// Number of token type
		tok_type.type_class = TOK_CLASS_ASSIGN;
		tok_type.need_value = 0;
		tok_type.is_compl = (answer==0) ? 1 : 0;
		tok_type.type_must_check = 0;
		tok_type.parenthesis = 0;
		tok->is_compl = (answer==0) ? 1 : 0;

		add_potentional_token_type(tok, tok_type);

		// Tokene gornush girizilen son chagyrylmaly
		if (tok_type.is_compl==1)
			tok->type_class = TOK_CLASS_ASSIGN;

		return 1;
	}
	return 0;
}






