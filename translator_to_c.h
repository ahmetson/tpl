/** Translyator, yagny C kodyny yasamak bilen ishleyan funksiyalar */
#ifndef TRANSLATOR_TO_C_H
#define TRANSLATOR_TO_C_H

#include <stdio.h>
#include "cmds.h"
#include "cmd/array.h"

// file length - 255, command length - 95
#ifndef MAX_PREP_LEN
#define MAX_PREP_LEN 350
#endif

/** Kodly setiri ýazýar. Eger kodly setir blogyň içinde bolsa, onda öňünden TAB goýýar */
void write_strings_to_file(FILE *f, wchar_t **l, int *llen, wchar_t *code);

// Header fayly ulanmaga tayynlayar
int write_headers_to_h_source(FILE *f, wchar_t *f_name);
int write_h_source_ending(FILE *f);
int write_headers_to_c_source(FILE *f, wchar_t *h_source);

// Global funksiya yglan etmelerini fayllara goshyar.
int write_glob_var_def_to_h_source(FILE *f);
int write_glob_var_def_to_c_source(FILE *f);

// Global funksiya yglan etmelerini fayllara goshyar.
int write_glob_arr_def_to_h_source(FILE *f);
int write_glob_arr_def_to_c_source(FILE *f);

// Lokal yglan edilen funksiyalary ichine goshyar
int write_loc_var_def(FILE *f, wchar_t main_file);

void write_utype_structs( );
int write_main_func_open_part(FILE *f);
int write_main_func_close_part(FILE *f);
int write_void_func_close_part(FILE *f);

int write_algor(FILE *f, int main_file);

// Lokal faylyn algoritmlerini yasayar
int translator_to_c();

int write_fn_prototype_to_h_source(FILE *f_h, wchar_t *fn_name);
int write_fn_open_part_to_c_source(FILE *f_h, wchar_t *fn_name);
wchar_t *make_fn_prototype(wchar_t *fn_name, wchar_t *out);

int is_open_block_cmd(command *cmd);
int is_close_block_cmd(command *cmd);

void write_type_c_code_analog(int type_class, int type_num, wchar_t **l, int *llen);
void write_type_init_val_c_code(wchar_t is_arr, int type_class, int type_num, wchar_t **l, int *llen);
void write_terminator(wchar_t **l, int *llen);

void write_tok_c_code( token *t, wchar_t **mem, int *memlen );
void write_cmd_c_code( command *cmd, wchar_t **mem, int *memlen );
void write_cmd_item_c_code( command_item *ci, wchar_t **mem, int *memlen );
void write_paren_item_c_code( parenthesis_elem *pe, wchar_t **mem, int *memlen );


void async_source_codes();

void add_string_to_free_list ( wchar_t *add_me, int *list_items_num, wchar_t ***l );

void write_loc_multi_free_call( FILE *f );
void  write_multi_free_call_in_def_fn( FILE *f, int func_num );

void add_arr_elems_to_free_list( wchar_t *base, int cur_nested, int nested, array_inc_item *elems_num, int *list_items_num, wchar_t ***l );

void add_utype_string_elems_to_free_list( int utype_num, wchar_t *add_me, int *list_items_num, wchar_t ***l );

void write_fn_prototype_void_def(FILE *f_h, wchar_t *fn_name);

void add_arr_utype_string_elems_to_free_list( int cur_nested, int nested, array_inc_item *elems_num, int utype_num, wchar_t *add_me, int *list_items_num, wchar_t ***l );

void write_substr_to_file( int i, int finish, FILE *f, wchar_t *str );

#endif
