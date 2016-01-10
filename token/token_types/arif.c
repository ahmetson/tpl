/** Arifmetiki operatorlar:
 *  +, -, *, :
 **/
#include <stdlib.h>
#include <string.h>
#include "arif.h"
#include "../../fns.h"


/** GOŞMAK  +   */
int TOK_CLASS_ARIF_PLUS_TYPE = 0;
wchar_t ARIF_PLUS_CHAR = L'+';

/** AÝYRMAK -   */
int TOK_CLASS_ARIF_MINUS_TYPE = 1;
wchar_t ARIF_MINUS_CHAR = L'-';

/** KÖPELTMEK * */
int TOK_CLASS_ARIF_MULTI_TYPE = 2;
wchar_t ARIF_MULTI_CHAR = L'*';

/** BÖLMEK :    */
int TOK_CLASS_ARIF_DIV_TYPE = 3;
wchar_t ARIF_DIV_CHAR = L':';


// Arifmetiki operatorlar üçin ulanylýan harplar.
// Ikinji element bolsa, C dilinde, arifmetiki operatorlar üçin ulanylýan harp ekwiwalentleri
wchar_t *TOK_CLASS_ARIF_CHARS[TOK_CLASS_ARIF_TYPES_NUM][2] = {
    {L"+", L"+"},
    {L"-", L"-"},
    {L"*", L"*"},
    {L":", L"/"}
};

// Debaglamak üçin
wchar_t *ARIF_TOK_NUM_WORDS[TOK_CLASS_ARIF_TYPES_NUM] = {
    L"Goşmak",
    L"Aýyrmak",
    L"Köpeltmek",
    L"Bölmek"
};


void tok_arif_c_code(token *tok, wchar_t **l, int *llen)
{
    wcsadd_on_heap( l, llen, TOK_CLASS_ARIF_CHARS[tok->potentional_types[0].type_num][1] );
}
