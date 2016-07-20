/**
*/
#include <stdlib.h>
#include <string.h>
#include "void.h"
#include "../token_types.h"
#include "../../tokens.h"
#include "../../fns.h"


wchar_t *TOK_VOID_CHARS[TOK_VOID_NUMS][3] = {
    {TOK_VOID_NUM_KEYWORD, L"Boş tip", L"void"} };

void tok_void_con_c_code(token *tok, wchar_t **l, int *llen)
{
    int type_num = tok->potentional_types[0].type_num;
    wcsadd_on_heap( l, llen, TOK_VOID_CHARS[type_num][2] );
}


int is_token_void(token *tok, wchar_t *tok_val)
{
	/*Go through array of possible types*/
	int answer, found = 0;
	token_type tok_type;

	answer = wcsstr_by_offset(TOK_VOID_CHARS[0][0], tok_val, 0);
    if (answer>=0)
    {
        tok_type.type_num = 0;							// Number of token type
        tok_type.type_class = TOK_CLASS_VOID;
        tok_type.need_value = 0;
        tok_type.is_compl = (answer==0) ? 1 : 0;
        tok_type.type_must_check = 0;
        tok_type.parenthesis = 1;
        tok->is_compl = (answer==0) ? 1 : 0;

        add_potentional_token_type(tok, tok_type);

        // Tokene gornush girizilen son chagyrylmaly
        if (tok_type.is_compl==1)
            tok->type_class = TOK_CLASS_VOID;
        found = 1;
    }

	return found;
}

