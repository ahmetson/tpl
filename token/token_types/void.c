/**
*/
#include <stdlib.h>
#include <string.h>
#include "void.h"
#include "../../fns.h"

wchar_t *TOK_VOID_CHARS[TOK_VOID_NUMS][3] = {
    {TOK_VOID_NUM_KEYWORD, L"Boş tip", L"void"} };

void tok_void_con_c_code(token *tok, wchar_t **l, int *llen)
{
    int type_num = tok->potentional_types[0].type_num;
    wcsadd_on_heap( l, llen, TOK_VOID_CHARS[type_num][2] );
}
