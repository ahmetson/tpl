#include <stdlib.h>
#include <string.h>
#include "loop_sttmnt.h"
#include "../../fns.h"

int TOK_CLASS_LOOP_STTMNT_UNTIL_NUM = 0;
int TOK_CLASS_LOOP_STTMNT_UNTIL_CLOSE_NUM = 1;

wchar_t *TOK_CLASS_LOOP_STTMNT_CHARS[TOK_CLASS_LOOP_STTMNT_NUM][2] = {
    {UNTIL_WORD,  L"while (!"},
    {UNTIL_CLOSE_WORD,  L") {"}
};

// DEBUG UCHIN
wchar_t *TOK_CLASS_LOOP_STTMNT_WORDS[TOK_CLASS_LOOP_STTMNT_NUM] = {
	L"tä",
	L"bolýança"
};


void tok_loop_sttmnt_c_code(token *tok, wchar_t **l, int *llen)
{
    wcsadd_on_heap( l, llen, TOK_CLASS_LOOP_STTMNT_CHARS[tok->potentional_types[0].type_num][1] );
}

