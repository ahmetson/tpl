#ifndef DEV_DEBUG_H
#define DEV_DEBUG_H

#include "algor.h"

void debug_cmd(command *cmd);

void debug_token(token *tok);

void debug_token_type(token_type *tok_type);

char *get_type_class(int type_class_num);
char *get_tok_type_value(token_type *tok_type);

char *get_cmd_class(int cmd_class_num);

char *get_cmd_class_type(int cmd_class_num, int cmd_type_num);


void debug_GLOB_VAR_DEFS();
void debug_LOCAL_VAR_DEFS();
#endif
