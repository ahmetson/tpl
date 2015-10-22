/*
 * Programma dilde global maglumatlar uchin ulanylyan maglumatlar
**/
#ifndef ASSIGN_H
#define ASSIGN_H


// Ulna maglumat yglan etmeli bolan komanda-da ikinji token bolup durmaly.
// Token bolsa programma dilde belli bolan operator harplary bolyar.
// Bu operator chep baglanma diyilip okalyar.
extern char *LEFT_ASSIGN_TOKEN_VALUE;
extern int   LEFT_ASSIGN_TOK_NUM;

extern char *RIGHT_ASSIGN_TOKEN_VALUE;
extern int   RIGHT_ASSIGN_TOK_NUM;

// DEBUG UCHIN
char *ASSIGN_TOK_NUM_WORDS[2];
#endif
