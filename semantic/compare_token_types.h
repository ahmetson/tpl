#ifndef SEMANTIC_COMPARE_TOKEN_TYPES_H
#define SEMANTIC_COMPARE_TOKEN_TYPES_H

int get_var_def_value_type(char *ident, int *type_class, int *type_num, char onlyglob);
int get_cmd_value_type(command *cmd, int *type_class, int *type_num);

#endif // SEMANTIC_COMPARE_TOKEN_TYPES_H
