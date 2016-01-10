#ifndef CMD_LOOP_STTMNT_H
#define CMD_LOOP_STTMNT_H

#include "../cmds.h"

extern int CMD_CLASS_LOOP_STTMNT_LOGIC;

int is_cmd_loop_sttmnt(command *cmd);
void cmd_loop_sttmnt_until_mod(command *cmd, int item_num);

int semantic_cmd_loop_sttmnt(command *cmd);

/** Faýla degişli kody C koda ýazýar **/
void cmd_loop_sttmnt_c_code(command *cmd, wchar_t **l, int *llen);

#endif // CMD_LOOP_STTMNT_H
