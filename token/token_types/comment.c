#include "comment.h"
#include "../token_types.h"
#include "../../tokens.h"
#include "../../fns.h"

int is_token_comment(token *tok, wchar_t *tok_val)
{
	/*Go through array of possible types*/
	int complete = 0, len = wcslen(tok_val), type;
	token_type tok_type;

    if (len>0)
    {
        if ( tok_val[0]!=L'/' )
            return 0;
        if ( len>1 )
        {
            /// Bir setirli token bilen işlemeli
            if ( tok_val[1]==L'/' )
            {
                type     = TOK_CLASS_COMMENT_SINGLE_TYPE;
                if (len==2)
                {
                    complete = 0;
                }
                else if (tok_val[len-1]==L'\n')
                {
                    complete = 1;
                }
                else if (tok->potentional_types[0].is_compl)
                {
                    return 0;
                }
            }
            else if (tok_val[1]=='*')
            {
                type     = TOK_CLASS_COMMENT_BLOCK_TYPE;
                if (len==2)
                {
                    complete = 0;
                }
                else if (len>2)
                {
                    if (tok_val[len-1]==L'*')
                    {
                        complete = 0;
                    }
                    else if (len>3 && tok_val[len-1]==L'/' && tok_val[len-2]==L'*')
                    {
                        complete = 1;
                    }
                    else if (tok->potentional_types[0].is_compl)
                    {
                        return 0;
                    }
                }
            }
            else
            {
                return 0;
            }

            /// Maglumaty tokene salmaly
            tok_type.type_num = type;							// Number of token type
            tok_type.type_class = TOK_CLASS_COMMENT;
            tok_type.need_value = 0;
            tok_type.is_compl = complete;
            tok_type.type_must_check = 0;
            tok_type.parenthesis = 1;
            tok->is_compl = complete;
            tok->type_class = TOK_CLASS_COMMENT;

            add_potentional_token_type(tok, tok_type);
            return 1;
        }
    }

	return 0;
}
