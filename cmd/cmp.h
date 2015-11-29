/** Deňeşdirme komandasy.
    Dolandyrýan komandalarda ulanmak üçin döredildi
**/
#ifndef CMD_CMP_H
#define CMD_CMP_H
#include "../cmds.h"

extern int CMD_CMP_TYPE;

int is_cmd_cmp(command *cmd);
void cmd_cmp_mod(command *cmd, int item_num);
int semantic_cmd_cmp(command *cmd);

void cmd_cmp_c_code(command *cmd, char **l, int *llen);

#endif // CMD_CMP_H
