/*
 * Harplardan yasalyan geljekki tokenin teksti
**/
#include "token_word.h"

/*
 * Token sozuni birinji gezek ulanmazdan on, ony taynlayar
**/
void init_token_word(token_word *t)
{
	t->len = 1;
	t->val[0] = '\0';
}

/*
 * Token sozune taze harp goyyar
**/
void set_token_word_value(token_word *tok_word, char c)
{
	tok_word->val[0] = c;
	tok_word->val[1] = '\0';
	tok_word->len = 2;
}
