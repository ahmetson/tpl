/**
 * HARPL tokeniň öz aýratynlyklary bar. Sebäbi başga tokenlere garanyňda, bu tokenler maksimum tükeniksiz uzynlygy bolup bilýär.
**/
#ifndef TOKEN_HARPL_H
#define TOKEN_HARPL_H

extern wchar_t HARPL_OPENER;
extern wchar_t *HARPL_OPENER_STRING;

int increment_string_tokens();
int add_wchar_t_to_last_string(wchar_t c);
int add_string_to_last_string(wchar_t *str);
void free_last_string();

wchar_t *get_string_item(int str_num);

int is_token_string_const_data    (token *tok);

token parse_string(FILE *f);
void  string_prepare(token *string_toke);

int is_valid_string_const_data(wchar_t *str, int after_quote_pos);
int is_string_const_data_compl(wchar_t *tok_val, wchar_t onstack);

int parser_mode_string( FILE *s, command *cmd );

#endif // TOKEN_HARPL_H
