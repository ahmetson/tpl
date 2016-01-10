#ifndef CMD_BLOCK_H
#define CMD_BLOCK_H

#include "../cmds.h"

extern int CMD_CLASS_BLOCK_CLOSE;

int is_cmd_block(command *cmd);
void cmd_block_close_mod(command *cmd, int item_num);

int semantic_cmd_block(command *cmd);

/** Faýla degişli kody C koda ýazýar **/
void cmd_block_c_code(command *cmd, wchar_t **l, int *llen);


#endif // CMD_BLOCK_H
