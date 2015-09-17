#ifndef DEV_DEBUG_H
#define DEV_DEBUG_H

#include "algor.h"

void debug_cmd(command *cmd);

void debug_token(token *tok);

void debug_token_type(token_type *tok_type);

char *get_type_class(int type_class_num);

char *get_cmd_class(int cmd_class_num);

char *get_cmd_class_type(int cmd_class_num, int cmd_type_num);


void debug_glob_def_vars(global_def_var *g);

void debug_loc_def_vars(local_def_var *l);
#endif
