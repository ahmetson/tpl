#ifndef CMD_BLOCK_INSIDE_H
#define CMD_BLOCK_INSIDE_H
#include "../cmds.h"

int     is_cmd_block_inside( command *cmd );
int     semantic_cmd_block_inside( command *cmd );

void    cmd_block_inside_return_mod( command *cmd );

int cmd_block_inside_return_type(command *cmd, int *type_class, int *type_num);

void cmd_block_inside_fn_return_c_code(command *cmd, wchar_t **l, int *llen);

#endif // CMD_BLOCK_INSIDE_H
