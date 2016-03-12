#include <stdlib.h>
#include <string.h>
#include "utype.h"
#include "../token_structs.h"
#include "../token_types.h"
#include "../../tokens.h"
#include "../../fns.h"

wchar_t *TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[3] = {L"/",  L".", L"birlik bölüji"};

int get_utype(token *tok, int *tok_class, int *tok_type)
{
    *tok_class = TOK_CLASS_UTYPE_CON;
    *tok_type  = tok->potentional_types[0].type_num;
    return 1;
}

void utype_item_separator_c_code(token *tok, wchar_t **l, int *llen)
{
    wcsadd_on_heap( l, llen, TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[1] );
}


int is_token_utype(token *tok, wchar_t *tok_val)
{
	/*Go through array of possible types*/
	int i, answer, found = 0;
	token_type tok_type;

	answer = wcsstr_by_offset(UTYPE_DEF_SIGNIFIER_WORD, tok_val, 0);
	if (answer>=0)
    {
        tok_type.type_num = UTYPE_DEF_SIGNIFIER;							// Number of token type
        tok_type.type_class = TOK_CLASS_UTYPE;
        tok_type.need_value = 0;
        tok_type.is_compl = (answer==0) ? 1 : 0;
        tok_type.type_must_check = 0;
        tok_type.parenthesis = 0;
        tok->is_compl = (answer==0) ? 1 : 0;

        add_potentional_token_type(tok, tok_type);

        // Tokene gornush girizilen son chagyrylmaly
        if (tok_type.is_compl==1)
            tok->type_class = TOK_CLASS_UTYPE;
        found = 1;
	}

    /// Birinji Ulanyjynyň tipi bilen bagly token tanalmady.
    /// Ikinji token görnüşi bilen deňeşdirip görülýär
	if (!found)
    {
        answer = wcsstr_by_offset(TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[0], tok_val, 0);
        if (answer>=0)
        {
            tok_type.type_num = UTYPE_ITEM_SEPARATOR;							// Number of token type
            tok_type.type_class = TOK_CLASS_UTYPE;
            tok_type.need_value = 0;
            tok_type.is_compl = (answer==0) ? 1 : 0;
            tok_type.type_must_check = 0;
            tok_type.parenthesis = 0;
            tok->is_compl = (answer==0) ? 1 : 0;

            add_potentional_token_type(tok, tok_type);

            // Tokene gornush girizilen son chagyrylmaly
            if (tok_type.is_compl==1)
                tok->type_class = TOK_CLASS_UTYPE;
            found = 1;
        }
    }

	return found;
}
