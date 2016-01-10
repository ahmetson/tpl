/*
 * Programma dilde ulna maglumat baglama komandasynyn operatory


   Token gornushinin programmada ulanmak uchin konstantasy
		TOK_CLASS_ASSIGN

   Ulni baglanma token gornushinin iki gornushi bar:
	  LEFT  : <-
	  RIGHT : ->
**/
#include <stdlib.h>
#include <string.h>

#include "assign.h"
#include "../../fns.h"

// Ulna maglumat yglan etmeli bolan komanda-da ikinji token bolup durmaly.
// Token bolsa programma dilde belli bolan operator harplary bolyar.
// Bu operator chep baglanma diyilip okalyar.
wchar_t *LEFT_ASSIGN_TOKEN_VALUE = L"<-";
int   LEFT_ASSIGN_TOK_NUM = 1;

wchar_t *RIGHT_ASSIGN_TOKEN_VALUE = L"->";
int   RIGHT_ASSIGN_TOK_NUM = 2;

// DEBUG UCHIN
wchar_t *ASSIGN_TOK_NUM_WORDS[2] = {
	L"to_left <-",
	L"to_right ->"
};

wchar_t *RIGHT_ASSIGN_C_CODES[2] = {L"=", L"{" };

void tok_assign_c_code(token *tok, wchar_t **l, int *llen)
{
    wchar_t *c_code;
    if (tok->potentional_types[0].type_num==RIGHT_ASSIGN_TOK_NUM &&
        tok->potentional_types[0].value[0]==L'1')
    {
        c_code = RIGHT_ASSIGN_C_CODES[1];
    }
    else
    {
        c_code = RIGHT_ASSIGN_C_CODES[0]; // Çep we sag tarapda ülňi bolan baglanmalaryň C kody.
    }

    wcsadd_on_heap( l, llen, c_code );
}







