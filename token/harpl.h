/**
 * HARPL tokeniň öz aýratynlyklary bar. Sebäbi başga tokenlere garanyňda, bu tokenler maksimum tükeniksiz uzynlygy bolup bilýär.
**/
#ifndef TOKEN_HARPL_H
#define TOKEN_HARPL_H

extern char HARPL_OPENER;

int increment_string_tokens();
int add_char_to_last_string(char c);
int add_string_to_last_string(char *str);
void free_last_string();

char *get_string_item(int str_num);

int is_token_string_const_data    (token *tok);

token parse_string(FILE *f);
void  string_prepare(token *string_toke);

int is_valid_string_const_data(char *str, int after_quote_pos);
int is_string_const_data_compl(char *tok_val, char onstack);
#endif // TOKEN_HARPL_H
