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

void tok_assign_c_code(token *tok, char **l, int *llen)
{
    *llen += strlen("=");
    if (!(*llen))
    {
        *llen += 1;
        *l = realloc(*l, *llen);
        strncpy(*l, "=", strlen("=")+1);
    }
    else
    {
        *l = realloc(*l, *llen);
        strncat(*l, "=", strlen("="));
    }

}







