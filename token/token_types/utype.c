#include <stdlib.h>
#include <string.h>
#include "utype.h"
#include "../token_structs.h"
#include "../token_types.h"
#include "../../fns.h"

wchar_t *TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[3] = {L"/",  L".", L"birlik bölüji"};

int get_utype(token *tok, int *tok_class, int *tok_type)
{

    *tok_class = TOK_CLASS_UTYPE_CON;
    *tok_type  = tok->potentional_types[0].type_num;
    return 1;
}

void utype_item_separator_c_code(token *tok, wchar_t **l, int *llen)
{
    wcsadd_on_heap( l, llen, TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[1] );
}
