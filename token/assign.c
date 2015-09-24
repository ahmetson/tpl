/*
 * Programma dilde ulna maglumat baglama komandasynyn operatory


   Token gornushinin programmada ulanmak uchin konstantasy
		ASSIGN_TYPE_CLASS

   Ulni baglanma token gornushinin iki gornushi bar:
	  LEFT  : <-
	  RIGHT : ->
**/

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






