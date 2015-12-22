#ifndef CMD_USER_DEF_TYPE_H
#define CMD_USER_DEF_TYPE_H

#ifndef CMD_UTYPE_TYPES_NUM
#define CMD_UTYPE_TYPES_NUM 2
#define CMD_UTYPE_DEF_TYPE 0
#define CMD_UTYPE_CON_TYPE 1
#endif // CMD_UTYPE_TYPES_NUM
#include "../cmds.h"

int is_cmd_utype(command *cmd);

void cmd_utype_con_mod(command *cmd, int item_num);
void cmd_utype_def_mod(command *cmd, int item_num);

int cmd_utype_con_return_type(command *cmd, int *type_class, int *type_num);

int semantic_cmd_utype_con(command *cmd);
int semantic_cmd_utype_def(command *cmd);

void cmd_utype_con_c_code(command *cmd, char **l, int *llen);

#endif // CMD_USER_DEF_TYPE_H
