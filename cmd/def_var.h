/**
 * Ülňini yglan edýän komanda.
**/
#ifndef CMD_DEF_VAR_H
#define CMD_DEF_VAR_H
#include "../cmds.h"

extern int DEF_VAR_TYPE_NUM;

int is_cmd_def_var(command *cmd);
void def_var_cmd_mod(command *cmd, token *item, int item_num);


// Gutarylan komandanyň ülňi yglan etme komandasymydygyny barlaýar
int is_glob_def_var_cmd(command *cmd);
int is_def_var_cmd(command *cmd);

int is_def_arr_cmd(command *cmd);

// Komanda ulni yglan etme bolsa, ulni yglan etmelerin sanawyna goshyar
int add_to_def_var_list(command *cmd);
void global_called_vars_add(command *cmd);
int cmd_def_var_return_type(command *cmd, int *return_class, int *return_type);

/// Ýasalan kodda çagyrylan global ülňileriň yglan edilen .h faýlynyň çagyrylan ýerinde inklud etmeli
void work_with_called_glob_vars();

void cmd_def_var_as_subcmd_c_code(command *cmd, char **l, int *llen);


int semantic_cmd_def_var(command *cmd);

#endif // CMD_DEF_VAR_H
