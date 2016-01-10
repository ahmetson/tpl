/**
 * Programma hakdaky maglumatlar bilen, kompilýasiýa edilip duran wagty görkezmek üçin ýasalan ülňiler we funksiýalar
**/
#ifndef MAIN_INF_H
#define MAIN_INF_H
#include "../token/token_structs.h"
#include "../cmds.h"

void inf_add_source_code_last_file();

void inf_add_to_token(token *tok, wchar_t c, int c_pos, int line);
void update_inf();
void inf_next_line();
void inf_next_wchar_t();

token *inf_get_last_token(command *cmd);
token *inf_get_parens_last_token(parenthesis *par);


#endif // MAIN_INF_H
