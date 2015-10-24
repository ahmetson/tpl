/**
 * HARPL tokeniň öz aýratynlyklary bar. Sebäbi başga tokenlere garanyňda, bu tokenler maksimum tükeniksiz uzynlygy bolup bilýär.
**/
#ifndef TOKEN_HARPL_H
#define TOKEN_HARPL_H

extern char HARPL_OPENER;

int increment_string_tokens();
int add_char_to_last_string(char c);

int is_token_string_const_data    (token *tok);

#endif // TOKEN_HARPL_H
