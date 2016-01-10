/** Logiki operatorlar
*/
#include <stdlib.h>
#include <string.h>
#include "logic.h"
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

