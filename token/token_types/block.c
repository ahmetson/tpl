#include <stdlib.h>
#include <string.h>
#include "block.h"
#include "../token_types.h"
#include "../../tokens.h"
#include "../../fns.h"


int TOK_CLASS_BLOCK_CLOSE_NUM = 0;

wchar_t *TOK_CLASS_BLOCK_CHARS[TOK_CLASS_BLOCK_NUM][3] = {
    {L"===",  L"}", L"ýapýan"}
};


void tok_block_c_code(token *tok, wchar_t **l, int *llen)
{
    wcsadd_on_heap( l, llen, TOK_CLASS_BLOCK_CHARS[tok->potentional_types[0].type_num][1] );
}

int is_token_block(token *tok, wchar_t *tok_val)
{
	/*Go through array of possible types*/
	int i, answer, found = 0;
	token_type tok_type;

	for(i=0; i<TOK_CLASS_BLOCK_NUM; i++)
	{

		answer = wcsstr_by_offset(TOK_CLASS_BLOCK_CHARS[i][0], tok_val, 0);
		if (answer>=0)
		{
			tok_type.type_num = i;							// Number of token type
			tok_type.type_class = TOK_CLASS_BLOCK;
			tok_type.need_value = 0;
			tok_type.is_compl = (answer==0) ? 1 : 0;
			tok_type.type_must_check = 0;
			tok_type.parenthesis = 0;
			tok->is_compl = (answer==0) ? 1 : 0;

			add_potentional_token_type(tok, tok_type);

			// Tokene gornush girizilen son chagyrylmaly
			if (tok_type.is_compl==1)
				tok->type_class = TOK_CLASS_BLOCK;
			found = 1;
		}
	}

	return found;
}

