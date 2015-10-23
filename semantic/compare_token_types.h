#ifndef SEMANTIC_COMPARE_TOKEN_TYPES_H
#define SEMANTIC_COMPARE_TOKEN_TYPES_H


// Sag tarapy konstanta maglumat we çep tarapy komanda birligi bolan komandalaryň bir token tipdedigi barlanýar
void compare_token_types_right_data();
void compare_token_types_left_data();
void compare_token_types_both_ident();

int get_user_var_def_value_type(char *ident, int *type_class, int *type_num, char *file_name);
int get_cmd_value_type(command *cmd, int *type_class, int *type_num);

#endif // SEMANTIC_COMPARE_TOKEN_TYPES_H
