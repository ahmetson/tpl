/** Arifmetiki operatorlar
**/
#ifndef CMD_ARIF_H
#define CMD_ARIF_H
#include "../cmds.h"

int is_cmd_arif(command *cmd);
int cmd_arif_mod(command *cmd, int item_num);

int semantic_cmd_arif(command *cmd);

int cmd_arif_return_type(command *cmd, int *ret_class, int *ret_type);

void cmd_arif_c_code(command *cmd, char **l, int *llen);

#endif // CMD_ARIF_H
