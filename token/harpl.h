/** HARPL tokeniň öz aýratynlyklary bar. Sebäbi başga tokenlere garanyňda, bu tokenler maksimum tükeniksiz uzynlygy bolup bilýär.*/
#ifndef TOKEN_HARPL_H
#define TOKEN_HARPL_H
#include "token_structs.h"
#include "../cmds.h"

extern wchar_t HARPL_OPENER;
extern wchar_t *HARPL_OPENER_STRING;

int increment_string_tokens();
int last_glob_str_add_chr(wchar_t c);
int last_glob_str_conc_str(wchar_t *str);
void last_glob_str_free();

wchar_t *get_glob_str_by_index(int str_num);

int is_token_string_const_data(token *tok);

token parse_string(FILE *f);
void  set_token_string_params(token *string_toke);
void last_glob_str_init();

int is_string_const_data_compl(wchar_t *tok_val, wchar_t onstack);

int parser_mode_string( FILE *s, command *cmd );

#endif // TOKEN_HARPL_H
