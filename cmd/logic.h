/** Logiki komandalar */
#ifndef CMD_LOGIC_H
#define CMD_LOGIC_H
#include "../cmds.h"

extern int CMD_LOGIC_BINAR_TYPE;
extern int CMD_LOGIC_MONO_TYPE;

int is_cmd_logic(command *cmd);
void cmd_mono_logic_mod(command *cmd, int item_num);
void cmd_binar_logic_mod(command *cmd, int item_num);

void cmd_logic_c_code(command *cmd, char **l, int *llen);

#endif // CMD_LOGIC_H
