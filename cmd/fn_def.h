#ifndef CMD_FN_DEF_H
#define CMD_FN_DEF_H

#ifndef CMD_FN_DEF_TYPES
#define CMD_FN_DEF_TYPE 0
#define CMD_FN_DEF_DEBUG_CHARS "ulanyjynyň funksiýasyny yglan etmek"
#define CMD_FN_DEC_TYPE 1
#define CMD_FN_DEC_DEBUG_CHARS "ulanyjynyň funksiýasyny beýan etmek"
#endif // CMD_FN_DEF_TYPES

#include "../cmds.h"
int is_cmd_fn_def(command *cmd);
void mod_cmd_by_fn_def(command *cmd, int item_num);

int semantic_cmd_fn_def(command *cmd);
int semantic_cmd_fn_dec(command *cmd);

void cmd_fn_def_c_code(command *cmd, char **l, int *llen);
void cmd_fn_dec_c_code(command *cmd, char **l, int *llen);

#endif // CMD_FN_DEF_H
