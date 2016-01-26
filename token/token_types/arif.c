/** Arifmetiki operatorlaryň tokenleriniň dört görnüşi bar:
*  +, -, *, :*/
#include <stdlib.h>
#include <string.h>
#include "arif.h"
#include "../token_types.h"
#include "../../tokens.h"
#include "../../fns.h"


/// Token tipleriniň numerleri
int TOK_CLASS_ARIF_PLUS_TYPE  = 0,      /** GOŞMAK  +   */
    TOK_CLASS_ARIF_MINUS_TYPE = 1,      /** AÝYRMAK -   */
    TOK_CLASS_ARIF_MULTI_TYPE = 2,      /** KÖPELTMEK * */
    TOK_CLASS_ARIF_DIV_TYPE   = 3;      /** BÖLMEK :    */


/** Arifmetiki operatorlaryň tokenleri üçin ulanylýan harplar.
    Birinji, TPL kodda, Ikinji element bolsa, C dilinde, ulanylýan ekwiwalentleri*/
wchar_t *TOK_CLASS_ARIF_CHARS[TOK_CLASS_ARIF_TYPES_NUM][3] = {
    {L"+", L"+", L"Goşmak"},
    {L"-", L"-", L"Aýyrmak"},
    {L"*", L"*", L"Köpeltmek"},
    {L":", L"/", L"Bölmek"}
};

/** Tokeniň c dildäki kodyny gaýtarýar*/
void tok_arif_c_code(token *tok, wchar_t **l, int *llen)
{
    wcsadd_on_heap( l, llen, TOK_CLASS_ARIF_CHARS[tok->potentional_types[0].type_num][1] );
}

/** Parsing edilende söz boýunça tokeni tanaýar*/
int is_token_arif(token *tok, wchar_t *tok_val)
{
    int i;
    for (i=0; i<TOK_CLASS_ARIF_TYPES_NUM; ++i)
    {
        if ( is_wcseq( tok_val, TOK_CLASS_ARIF_CHARS[i][0] ) )
        {
            token_type tok_type;
            tok_type.type_num   = i;	            //Token tipiniň nomeri
            tok_type.type_class = TOK_CLASS_ARIF;
            tok_type.need_value = 0;
            tok_type.parenthesis = 1;
            tok_type.is_compl = 1;
            tok_type.type_must_check = 0;

            tok->is_compl     = 1;
            tok->type_class   = TOK_CLASS_ARIF;

            add_potentional_token_type(tok, tok_type);

            return 1;
        }
    }

    return 0;
}
