#ifndef TOKEN_TOKEN_TYPES_UTYPE_H
#define TOKEN_TOKEN_TYPES_UTYPE_H


#ifndef UTYPE_DEF_SIGNIFIER_WORD
#define UTYPE_DEF_SIGNIFIER_WORD L"tipi"
#endif // UTYPE_DEF_SIGNIFIER_WORD

#ifndef UTYPE_DEF_SIGNIFIER
#define UTYPE_TOKEN_TYPES   2
#define UTYPE_DEF_SIGNIFIER 0
#define UTYPE_ITEM_SEPARATOR 1
#define UTYPE_DEF_SIGNIFIER_DB_WORD L"komandany gutaryjy"
#endif // UTYPE_DEF_SIGNIFIER

extern wchar_t *TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[3];

#include "../token_structs.h"
int get_utype(token *tok, int *tok_class, int *tok_type);

void utype_item_separator_c_code(token *tok, wchar_t **l, int *llen);

int is_token_utype              (token *tok, wchar_t *tok_val);


#endif // TOKEN_TOKEN_TYPES_UTYPE_H
