/** Logiki operatorlar
*/
#include <stdlib.h>
#include <string.h>
#include "logic.h"
#include "../token_types.h"
#include "../../tokens.h"
#include "../../fns.h"


/** WE                  &   */
int TOK_CLASS_LOGIC_AND_TYPE = 0;

/** ÝA                  ?   */
int TOK_CLASS_LOGIC_OR_TYPE = 1;

/** DÄL BOLSA           ! */
int TOK_CLASS_LOGIC_NOT_TYPE = 2;


/// Tokenleriň başlap bilýän harplary
wchar_t LOGIC_AND_CHAR = L'&';
wchar_t LOGIC_OR_CHAR = L'?';
wchar_t LOGIC_NOT_CHAR = L'!';



// Arifmetiki operatorlar üçin ulanylýan harplar.
// Ikinji element bolsa, C dilinde, arifmetiki operatorlar üçin ulanylýan harp ekwiwalentleri
wchar_t *TOK_CLASS_LOGIC_CHARS[TOK_CLASS_LOGIC_TYPES_NUM][2] = {
    {L"&",  L"&&"},
    {L"?",  L"||"},
    {L"!",  L"!"}
};

// Debaglamak üçin
wchar_t *LOGIC_TOK_NUM_WORDS[TOK_CLASS_LOGIC_TYPES_NUM] = {
    L"we &",
    L"ýa ?",
    L"däl bolsa !"
};


void tok_logic_c_code(token *tok, wchar_t **l, int *llen)
{
    wcsadd_on_heap( l, llen, TOK_CLASS_LOGIC_CHARS[tok->potentional_types[0].type_num][1] );
}

int is_token_logic(token *tok, wchar_t *tok_val)
{
    if (!wcslen(tok_val) || wcslen(tok_val)>2)
        return 0;

    int i;
    wchar_t found = 0;
    for (i=0; i<TOK_CLASS_LOGIC_TYPES_NUM; ++i)
    {

        if (wcslen(tok_val)==wcslen(TOK_CLASS_LOGIC_CHARS[i][0]) &&
            wcsncmp(tok_val, TOK_CLASS_LOGIC_CHARS[i][0], wcslen(TOK_CLASS_LOGIC_CHARS[i][0]))==0)
        {
            token_type tok_type;
            tok_type.type_num   = i;	// Number of token type
            tok_type.type_class = TOK_CLASS_LOGIC;
            tok_type.need_value = 0;
            tok_type.parenthesis = 1;
            tok_type.is_compl = 1;
            tok_type.type_must_check = 0;

            tok->is_compl     = 1;
            tok->type_class   = TOK_CLASS_LOGIC;

            add_potentional_token_type(tok, tok_type);

            found = 1;
        }
    }

    return found;
}


