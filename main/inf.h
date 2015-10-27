/**
 * Programma hakdaky maglumatlar bilen, kompilýasiýa edilip duran wagty görkezmek üçin ýasalan ülňiler we funksiýalar
**/
#ifndef MAIN_INF_H
#define MAIN_INF_H

void inf_add_source_code_last_file();

void inf_add_to_token(token *tok, char c, int c_pos, int line);
int inf_next_line(int *c_pos, int *line);
int inf_next_char(char c, int *c_pos, int *line);

token *inf_get_last_token(command *cmd);

#endif // MAIN_INF_H
