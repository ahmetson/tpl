#ifndef CMD_CONV_BASIC_TYPES_H
#define CMD_CONV_BASIC_TYPES_H
#include "../cmds.h"

#ifndef CMD_CONV_TYPES
#define CMD_CONV_TYPES 4
#define CMD_CLASS_CONV_BASIC_TYPE_NUM 0
#define CMD_CONV_IF 0
#define CMD_CONV_FI 1
#define CMD_CONV_CI 2
#define CMD_CONV_AI 3
#endif // CMD_CONV_TYPES

extern wchar_t *conv_c_codes[CMD_CONV_TYPES][3];

int is_cmd_conv_basic_type(command *cmd);

void cmd_conv_basic_type_mod(command *cmd, int item_num);

int semantic_cmd_conv_basic_type(command *cmd);

int cmd_arr_conv_basic_return_type(command *cmd, int *type_class, int *type_num);

void cmd_conv_basic_type_c_code(command *cmd, wchar_t **l, int *llen);

void get_conv_type_num(command *cmd, int *rclass, int *rtype);

#endif // CMD_CONV_BASIC_TYPES_H
