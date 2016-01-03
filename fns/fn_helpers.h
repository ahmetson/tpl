#ifndef FNS_FN_HELPERS_H
#define FNS_FN_HELPERS_H
/** Funksiýalaryň komandalary bilen işleýän kömekçi faňksiýeler.
*/
#include "../cmds.h"

int is_inside_fn();

int is_unvalid_inside_fn_cmd(command *cmd);

int is_fn_def(command *cmd);

void prepare_tmp_fn_data_container(command *cmd);

int is_fn_close_cmd(command *cmd);

int add_to_tmp_fn_def_var_list(command *cmd);
int add_to_tmp_fn_def_arr_list(command *cmd);
int add_to_tmp_fn_algor_list(command *cmd);

void add_to_tmp_fn_var_def(command *cmd);
void add_to_tmp_fn_algor(command *cmd);
void add_to_tmp_fn_arr_def(command *cmd);

int is_tmp_fn_ident_used(char *ident);

int is_fn_name_used(char *ident);
int get_fn_by_ident(char *ident);

int is_tmp_fn_var_ident_used(char *ident);
int get_tmp_fn_var_def_value_type(char *ident, int *type_class, int *type_num);

int is_tmp_fn_arr_ident_used(char *ident);
int get_tmp_func_arr_address_by_ident(char *ident, int *type);

void get_loc_fn_prototype(int n, char **p, int *l);
void write_fn_args(int n, char **p, int *l);
void c_trans_source_fn_add_algor(FILE *f, int fn);

void c_trans_source_add_fn_def_var(FILE *f, int fn);
void c_trans_source_add_fn_def_arr(FILE *f, int fn);


void add_to_func_list();

void add_to_last_tmp_func_arr_items(command *cmd);


void add_fn_dec(command *cmd);
int is_fn_already_dec(char *ident);

#endif // FNS_FN_HELPERS_H