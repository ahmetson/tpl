/*
 * Programma dilde global maglumatlar uchin ulanylyan maglumatlar
**/
#ifndef GLOB_H
#define GLOB_H
#include <wchar.h>
#include "../token_structs.h"

extern wchar_t GLOB_IDENT_OPENER;

extern wchar_t *glob_def_string;

int is_token_def_glob           (token *tok, wchar_t *tok_val);


#endif
