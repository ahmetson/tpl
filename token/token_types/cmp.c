/** Deňeşdirme operatorlarynyň tokenleri
    4 sany token görnüşi. */
#include <string.h>
#include <stdlib.h>
#include "cmp.h"
#include "../token_types.h"
#include "../../tokens.h"
#include "../../fns.h"


int TOK_CLASS_CMP_EQ_TYPE   = 0,       /** DEŇDIR           =   */
    TOK_CLASS_CMP_LT_TYPE   = 1,       /** KIÇIDIR          <   */
    TOK_CLASS_CMP_GT_TYPE   = 2,       /** ULUDYR           > */
    TOK_CLASS_CMP_EQLT_TYPE = 3,       /** KIÇIDIR ÝA DEŇDIR <=    */
    TOK_CLASS_CMP_EQGT_TYPE = 4;       /** ULUDYR ÝA DEŇDIR  >=    */

// Arifmetiki operatorlar üçin ulanylýan harplar.
// Ikinji element bolsa, C dilinde, arifmetiki operatorlar üçin ulanylýan harp ekwiwalentleri
wchar_t *TOK_CLASS_CMP_CHARS[TOK_CLASS_CMP_TYPES_NUM][3] = {
    {L"=",  L"==", L"deň"},
    {L"<",  L"<",  L"kiçi <"},
    {L">",  L">",  L"uly >"},
    {L"<=", L"<=", L"kiçi ýa deň <="},
    {L">=", L">=", L"uly ýa deň >="}
};


void tok_cmp_c_code(token *tok, wchar_t **l, int *llen)
{
    wcsadd_on_heap( l, llen, TOK_CLASS_CMP_CHARS[tok->potentional_types[0].type_num][1] );
}

int is_token_cmp(token *tok, wchar_t *tok_val)
{
    if ( !wcslen( tok_val ) || wcslen( tok_val )>2 )
        return 0;

    int i;
    char found = 0;
    for (i=0; i<TOK_CLASS_CMP_TYPES_NUM; ++i)
    {
        if ( is_wcseq( tok_val, TOK_CLASS_CMP_CHARS[i][0] ) )
        {
            token_type tok_type;
            tok_type.type_num   = i;	// Number of token type
            tok_type.type_class = TOK_CLASS_CMP;
            tok_type.need_value = 0;
            tok_type.parenthesis = 1;
            tok_type.is_compl = 1;
            tok_type.type_must_check = 0;

            tok->is_compl     = 1;
            tok->type_class   = TOK_CLASS_CMP;

            add_potentional_token_type(tok, tok_type);

            found = 1;
        }
    }

    return found;
}

