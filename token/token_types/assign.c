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

// Ulna maglumat yglan etmeli bolan komanda-da ikinji token bolup durmaly.
// Token bolsa programma dilde belli bolan operator harplary bolyar.
// Bu operator chep baglanma diyilip okalyar.
char *LEFT_ASSIGN_TOKEN_VALUE = "<-";
int   LEFT_ASSIGN_TOK_NUM = 1;

char *RIGHT_ASSIGN_TOKEN_VALUE = "->";
int   RIGHT_ASSIGN_TOK_NUM = 2;

// DEBUG UCHIN
char *ASSIGN_TOK_NUM_WORDS[2] = {
	"to_left <-",
	"to_right ->"
};

char *RIGHT_ASSIGN_C_CODES[2] = {"=", "{" };

void tok_assign_c_code(token *tok, char **l, int *llen)
{
    char *c_code;
    if (tok->potentional_types[0].type_num==RIGHT_ASSIGN_TOK_NUM &&
        tok->potentional_types[0].value[0]=='1')
    {
        c_code = RIGHT_ASSIGN_C_CODES[1];
    }
    else
    {
        c_code = RIGHT_ASSIGN_C_CODES[0]; // Çep we sag tarapda ülňi bolan baglanmalaryň C kody.
    }

    if (!(*llen))
    {
        *llen += strlen(c_code)+1;
        *l = realloc(*l, *llen);
        strncpy(*l, c_code, strlen(c_code)+1);
    }
    else
    {
        *llen = strlen(c_code);
        *l = realloc(*l, *llen);
        strncat(*l, c_code, strlen(c_code));
    }

}







