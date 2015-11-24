/** Global ülňini çagyrýan funksiýa
**/
#ifndef CMD_CALL_GLOB_VAR_H
#define CMD_CALL_GLOB_VAR_H
#include "../cmds.h"

extern int CMD_CALL_GLOB_VAR_TYPE_NUM;

int is_cmd_call_glob_var(command *c);
int cmd_call_glob_var_mod(command *cmd, int tok_num);
int semantic_cmd_call_glob_var(command *cmd);
int cmd_call_glob_var_return_type(command *cmd, int *return_class, int *ret_type);
void cmd_call_glob_var_c_code(command *cmd, char **line, int *llen);

#endif // CMD_CALL_GLOB_VAR_H
