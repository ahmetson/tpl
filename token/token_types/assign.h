/*
 * Programma dilde global maglumatlar uchin ulanylyan maglumatlar
**/
#ifndef ASSIGN_H
#define ASSIGN_H
#include "../token_structs.h"


// Ulna maglumat yglan etmeli bolan komanda-da ikinji token bolup durmaly.
// Token bolsa programma dilde belli bolan operator harplary bolyar.
// Bu operator chep baglanma diyilip okalyar.
extern wchar_t *LEFT_ASSIGN_TOKEN_VALUE;
extern int   LEFT_ASSIGN_TOK_NUM;

extern wchar_t *RIGHT_ASSIGN_TOKEN_VALUE;
extern int   RIGHT_ASSIGN_TOK_NUM;

// DEBUG UCHIN
extern wchar_t *ASSIGN_TOK_NUM_WORDS[2];

extern wchar_t *RIGHT_ASSIGN_C_CODES[2];

void tok_assign_c_code(token *tok, wchar_t **l, int *llen);
#endif
