/** Parsing wagty tokeni tanamak üçin ulanylýan söz bilen baglanyşykly funksiýalar
**/
#include <string.h>
#include "token_word.h"
#include "../fns.h"

/** Token sözüni pozýar. Başyna bir harp goýýar.  */
int set_token_word_value(wchar_t *tok_string, wchar_t c)
{
    empty_string(tok_string, wcslen(tok_string));
	tok_string[0] = c;
	return 1;
}
