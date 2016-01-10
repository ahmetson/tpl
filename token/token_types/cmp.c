/** Deňeşdirme operatorlarynyň tokenleri
**/
#include <string.h>
#include <stdlib.h>
#include "cmp.h"
#include "../../fns.h"

/** DEŇDIR           =   */
int TOK_CLASS_CMP_EQ_TYPE = 0;

/** KIÇIDIR          <   */
int TOK_CLASS_CMP_LT_TYPE = 1;

/** ULUDYR           > */
int TOK_CLASS_CMP_GT_TYPE = 2;

/** KIÇIDIR ÝA DEŇDIR <=    */
int TOK_CLASS_CMP_EQLT_TYPE = 3;

/** ULUDYR ÝA DEŇDIR  >=    */
int TOK_CLASS_CMP_EQGT_TYPE = 4;

/// Tokenleriň başlap bilýän harplary
wchar_t CMP_EQ_CHAR = L'=';
wchar_t CMP_LT_CHAR = L'<';
wchar_t CMP_GT_CHAR = L'>';



// Arifmetiki operatorlar üçin ulanylýan harplar.
// Ikinji element bolsa, C dilinde, arifmetiki operatorlar üçin ulanylýan harp ekwiwalentleri
wchar_t *TOK_CLASS_CMP_CHARS[TOK_CLASS_CMP_TYPES_NUM][2] = {
    {L"=",  L"=="},
    {L"<",  L"<"},
    {L">",  L">"},
    {L"<=", L"<="},
    {L">=", L">="}
};

// Debaglamak üçin
wchar_t *CMP_TOK_NUM_WORDS[TOK_CLASS_CMP_TYPES_NUM] = {
    L"deň",
    L"kiçi <",
    L"uly >",
    L"kiçi ýa deň <=",
    L"uly ýa deň >="
};

void tok_cmp_c_code(token *tok, wchar_t **l, int *llen)
{
    wcsadd_on_heap( l, llen, TOK_CLASS_CMP_CHARS[tok->potentional_types[0].type_num][1] );
}

