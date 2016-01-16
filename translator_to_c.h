/*
 * Translyator, yagny C kodyny yasamak bilen ishleyan funksiyalar
**/
#ifndef TRANSLATOR_TO_C_H
#define TRANSLATOR_TO_C_H

#include <stdio.h>
#include "cmds.h"

// file length - 255, command length - 95
#ifndef MAX_PREP_LEN
#define MAX_PREP_LEN 350
#endif

// C kod nirede saklanyar?
extern wchar_t *C_SOURCE_FOLDER;

extern int TRANS_C_BLOCK_DEPTH;

/** Berlen kodly ýazga, içindeliginiň basgançagyna görä, '\t' goýýar */
void write_tabs_c_code(wchar_t **l, int *llen, int tabs_num);

/** Kodly setiri ýazýar. Eger kodly setir blogyň içinde bolsa, onda öňünden TAB goýýar */
void write_code_line(FILE *f, wchar_t **l, int *llen, int tabs_num, wchar_t *code);

// Header fayly ulanmaga tayynlayar
int prepare_h_source(FILE *f, wchar_t *f_name);
int finishize_h_source(FILE *f);
int prepare_c_source(FILE *f, wchar_t *h_source);

// Global funksiya yglan etmelerini fayllara goshyar.
int c_trans_header_add_glob_def_var(FILE *f);
int c_trans_source_add_glob_def_var(FILE *f);

// Global funksiya yglan etmelerini fayllara goshyar.
int c_trans_header_add_glob_def_arr(FILE *f);
int c_trans_source_add_glob_def_arr(FILE *f);

// Lokal yglan edilen funksiyalary ichine goshyar
int c_trans_source_add_loc_def_var(FILE *f, wchar_t main_file);

int prepare_main_func(FILE *f);
int finishize_main_func(FILE *f);
int finishize_void_func(FILE *f);

int c_trans_source_add_algor(FILE *f, int main_file);

// Lokal faylyn algoritmlerini yasayar
int work_with_translator();

int c_trans_write_file_fn_prototype(FILE *f_h, wchar_t *fn_name);
int c_trans_write_file_fn_open(FILE *f_h, wchar_t *fn_name);
wchar_t *c_trans_get_file_fn_prototype(wchar_t *fn_name, wchar_t *out);

int is_open_block_cmd(command *cmd);
int is_close_block_cmd(command *cmd);

void get_type_c_code(int type_class, int type_num, wchar_t **l, int *llen);
void get_type_init_val_c_code(wchar_t is_arr, int type_class, int type_num, wchar_t **l, int *llen);
void get_cmd_end_c_code(wchar_t **l, int *llen);

void tok_get_c_code( token *t, wchar_t **mem, int *memlen );
void cmd_get_c_code( command *cmd, wchar_t **mem, int *memlen );
void cmd_item_get_c_code( command_item *ci, wchar_t **mem, int *memlen );
void paren_item_get_c_code( parenthesis_elem *pe, wchar_t **mem, int *memlen );


void work_with_translator_whole_project();

#endif
