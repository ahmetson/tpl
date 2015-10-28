/**
 * HARPL tokeniň öz aýratynlyklary bar. Sebäbi başga tokenlere garanyňda, bu tokenler maksimum tükeniksiz uzynlygy bolup bilýär.
**/
#ifndef TOKEN_HARPL_H
#define TOKEN_HARPL_H

extern char HARPL_OPENER;

int increment_string_tokens();
int add_char_to_last_string(char c);

int is_token_string_const_data    (token *tok);

int work_with_string(token *string_tok, char *mode, char c);
int prepare_to_work_with_string(token *string_tok, char *mode, char c, int c_pos, int line);

#endif // TOKEN_HARPL_H