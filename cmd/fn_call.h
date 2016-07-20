/** Funksiýalar bilen işleýän maglumatlar
**/
#ifndef CMD_FN_H
#define CMD_FN_H

#include "../fns/fn.h"
#include "../cmds.h"

extern int FN_CALL_TYPE_NUM;

int is_cmd_fn_call(command *cmd);
void fn_call_cmd_mod(command *cmd, int item_num);
int fn_call_cmd_return_type(command *cmd, int *return_class, int *ret_type);
int semantic_cmd_fn_call(command *cmd);

/** Çagyrylan funksiýanyň kabul edýän argumentleriniň, funksiýanyň garaşýan argument tiplerini gabat gelýänini barlaýar

    \argn  - funksiýanyň garaşýan argumentleriniň sany

    \args  - funksiýanyň garaşýan argumentleri

    \paren - funksiýa çagyrylynda berlen maglumatlar
**/
int check_fn_args(int argn, int args_num, parenthesis *paren);


// C translator
void cmd_fn_call_c_code(command *cmd, wchar_t **l, int *len);

int cmd_fn_call_return_type(command *cmd, int *return_class, int *return_type);

void trans_to_c_write_last_fn_arg( wchar_t **mem, int *memlen, wchar_t *add );
void trans_to_c_move_fn_arg_to_malloc( token *tok );

#endif // CMD_FN_H
