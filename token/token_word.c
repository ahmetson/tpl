/*
 * Harplardan yasalyan geljekki tokenin teksti
**/
#include "token_word.h"

/*
 * Token sozune taze harp goyyar
**/
void set_token_word_value(char *tok_string, char c)
{
	tok_string[0] = c;
	tok_string[1] = '\0';
}
