#include <stdlib.h>
#include <string.h>
#include "block.h"
#include "../../fns.h"

int TOK_CLASS_BLOCK_CLOSE_NUM = 0;

wchar_t *TOK_CLASS_BLOCK_CHARS[TOK_CLASS_BLOCK_NUM][2] = {
    {L"===",  L"}"}
};

// DEBUG UCHIN
wchar_t *TOK_CLASS_BLOCK_WORDS[TOK_CLASS_BLOCK_NUM] = {
	L"ýapýan"
};


void tok_block_c_code(token *tok, wchar_t **l, int *llen)
{
    wcsadd_on_heap( l, llen, TOK_CLASS_BLOCK_CHARS[tok->potentional_types[0].type_num][1] );
}

