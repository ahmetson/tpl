#include <stdlib.h>
#include <string.h>
#include "ctrl_sttmnt.h"
#include "../token_types.h"
#include "../../tokens.h"
#include "../../fns.h"

int TOK_CLASS_CTRL_STTMNT_IF_NUM = 0;
int TOK_CLASS_CTRL_STTMNT_IFELSE_NUM = 1;
int TOK_CLASS_CTRL_STTMNT_ELSE_NUM = 2;
int TOK_CLASS_CTRL_STTMNT_CLOSE_BLOCK_NUM = 3;

wchar_t *TOK_CLASS_CTRL_STTMNT_CHARS[TOK_CLASS_CTRL_STTMNT_NUM][2] = {
    {IF_WORD,  L"if "},
    {IFELSE_WORD,  L"} else if "},
    {ELSE_WORD,  L"} else {"},
    {CTRL_STTMNT_CLOSE_BLOCK,  L" {"}
};

// DEBUG UCHIN
wchar_t *TOK_CLASS_CTRL_STTMNT_WORDS[TOK_CLASS_CTRL_STTMNT_NUM] = {
	L"eger",
	L"ýa",
	L"ýogsa",
	L"bolsa"
};


void tok_ctrl_sttmnt_c_code(token *tok, wchar_t **l, int *llen)
{
    wcsadd_on_heap( l, llen, TOK_CLASS_CTRL_STTMNT_CHARS[tok->potentional_types[0].type_num][1] );
}

// eger, ýa, ýogsa, bolsa
int is_token_ctrl_sttmnt(token *tok, wchar_t *tok_val)
{
	/*Go through array of possible types*/
	int i, answer, found = 0;
	token_type tok_type;

	for(i=0; i<TOK_CLASS_CTRL_STTMNT_NUM; i++)
	{

		answer = wcsstr_by_offset(TOK_CLASS_CTRL_STTMNT_CHARS[i][0], tok_val, 0);
		if (answer>=0)
		{
			tok_type.type_num = i;							// Number of token type
			tok_type.type_class = TOK_CLASS_CTRL_STTMNT;
			tok_type.need_value = 0;
			tok_type.is_compl = (answer==0) ? 1 : 0;
			tok_type.type_must_check = 0;
			tok_type.parenthesis = 0;
			tok->is_compl = (answer==0) ? 1 : 0;

			add_potentional_token_type(tok, tok_type);

			// Tokene gornush girizilen son chagyrylmaly
			if (tok_type.is_compl==1)
				tok->type_class = TOK_CLASS_CTRL_STTMNT;
			found = 1;
		}
	}

	return found;
}
