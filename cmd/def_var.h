/**
 * Ülňini yglan edýän komanda.
**/
#ifndef CMD_DEF_VAR_H
#define CMD_DEF_VAR_H
#include "../cmds.h"

extern int DEF_VAR_TYPE_NUM;

int is_cmd_def_var(command *cmd);
int def_var_cmd_mod(command *cmd, token *tok, int tok_num);


// Gutarylan komandanyň ülňi yglan etme komandasymydygyny barlaýar
int is_glob_def_var_cmd(command *cmd);
int is_def_var_cmd(command *cmd);

// Komanda ulni yglan etme bolsa, ulni yglan etmelerin sanawyna goshyar
int add_to_def_var_list(command *cmd);

#endif // CMD_DEF_VAR_H
