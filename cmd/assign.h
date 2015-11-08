#ifndef CMD_ASSIGN_H
#define CMD_ASSIGN_H
#include "../cmds.h"

int is_cmd_assign(command  *cmd);
int assign_cmd_mod(command *cmd, int tok_num);
int semantic_cmd_assign(command *cmd);

// C translator
void cmd_assign_c_code(command *cmd, char **l, int *len);

#endif // CMD_ASSIGN_H
