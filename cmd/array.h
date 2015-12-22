#ifndef CMD_ARRAY_H
#define CMD_ARRAY_H

#include "../cmds.h"

extern int CMD_CLASS_ARR_DEF;
extern int CMD_CLASS_ARR_CON;

typedef struct{
    int inf_file_num;
    int inf_char_num;
    char inf_char;
    int inf_line_num;
    char ident[MAXLEN];
    int  incs;
    int  type_class;
    int  type_num;
}array_item;

typedef struct{
    int arr_num;
    int inc_num;
    int elem_num;
}array_inc_item;

int is_cmd_arr(command *cmd);

void cmd_arr_con_mod(command *cmd, int item_num);
void cmd_arr_def_mod(command *cmd, int item_num);

int cmd_arr_con_return_type(command *cmd, int *type_class, int *type_num);

int semantic_cmd_arr_con(command *cmd);
int semantic_cmd_arr_def(command *cmd);

void cmd_arr_con_c_code(command *cmd, char **l, int *llen);


void add_to_last_glob_arr_items(command *cmd);
void add_to_last_loc_arr_items(command *cmd);
void add_to_last_dec_arr_items(command *cmd);
void add_to_last_tmp_arr_items(command *cmd);

void get_arr_address_by_ident(char *ident, int *type, int *num);

void global_called_arrs_add(command *cmd);

/// Ýasalan kodda çagyrylan global ülňileriň yglan edilen .h faýlynyň çagyrylan ýerinde inklud etmeli
void work_with_called_glob_arrs();

void add_arr_elem_inf_c_code(char **line, int *line_len, int arr_type, int arr_num, int max_items);

#endif // CMD_ARRAY_H
