/*
 * Harplardan yasalyan geljekki tokenin teksti
**/
#include <string.h>

#include "token_word.h"
#include "../fns.h"

/*
 * Token sözüni pozýar. Başyna bir harp goýýar.
**/
void set_token_word_value(char *tok_string, char c)
{
    empty_string(tok_string, strlen(tok_string));
	tok_string[0] = c;
}
