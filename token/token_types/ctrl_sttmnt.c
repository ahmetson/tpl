#include <stdlib.h>
#include <string.h>
#include "ctrl_sttmnt.h"
#include "../../fns.h"

int TOK_CLASS_CTRL_STTMNT_IF_NUM = 0;
int TOK_CLASS_CTRL_STTMNT_IFELSE_NUM = 1;
int TOK_CLASS_CTRL_STTMNT_ELSE_NUM = 2;
int TOK_CLASS_CTRL_STTMNT_CLOSE_BLOCK_NUM = 3;

wchar_t *TOK_CLASS_CTRL_STTMNT_CHARS[TOK_CLASS_CTRL_STTMNT_NUM][2] = {
    {IF_WORD,  L"if "},
    {IFELSE_WORD,  L"} else if "},
    {ELSE_WORD,  L"} else {"},
    {CTRL_STTMNT_CLOSE_BLOCK,  L" {"}
};

// DEBUG UCHIN
wchar_t *TOK_CLASS_CTRL_STTMNT_WORDS[TOK_CLASS_CTRL_STTMNT_NUM] = {
	L"eger",
	L"ýa",
	L"ýogsa",
	L"bolsa"
};


void tok_ctrl_sttmnt_c_code(token *tok, wchar_t **l, int *llen)
{
    wcsadd_on_heap( l, llen, TOK_CLASS_CTRL_STTMNT_CHARS[tok->potentional_types[0].type_num][1] );
}

