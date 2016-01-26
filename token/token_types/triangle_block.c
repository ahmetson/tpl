#include "triangle_block.h"
#include "../token_types.h"
#include "../../tokens.h"
#include "../../fns.h"

wchar_t *TRIANGLE_BLOCK_TOKENS_KEYWORDS[TRIANGLE_BLOCK_TOKENS_NUM][2] = {
  {L"<:", L"blogy açýan"},
  {L":>", L"blogy ýapýan"}
};


int is_token_triangle_block(token *tok, wchar_t *tok_val)
{
	/*Go through array of possible types*/
	int i, answer, found = 0;
	token_type tok_type;

	for(i=0; i<TRIANGLE_BLOCK_TOKENS_NUM; i++)
	{

		answer = wcsstr_by_offset(TRIANGLE_BLOCK_TOKENS_KEYWORDS[i][0], tok_val, 0);
		if (answer>=0)
		{
			tok_type.type_num = i;							// Number of token type
			tok_type.type_class = TOK_CLASS_TRIANGLE_BLOCK;
			tok_type.need_value = 0;
			tok_type.is_compl = (answer==0) ? 1 : 0;
			tok_type.type_must_check = 0;
			tok_type.parenthesis = 0;
			tok->is_compl = (answer==0) ? 1 : 0;

			add_potentional_token_type(tok, tok_type);

			// Tokene gornush girizilen son chagyrylmaly
			if (tok_type.is_compl==1)
				tok->type_class = TOK_CLASS_TRIANGLE_BLOCK;
			found = 1;
		}
	}

	return found;
}
