/*
 * Translyator, yagny C kodyny yasamak bilen ishleyan funksiyalar
**/
#ifndef TRANSLATOR_TO_C_H
#define TRANSLATOR_TO_C_H

#include <stdio.h>

// C kod nirede saklanyar?
extern char *C_SOURCE_FOLDER;

extern int TRANS_C_BLOCK_DEPTH;

/** Berlen kodly ýazga, içindeliginiň basgançagyna görä, '\t' goýýar */
void write_tabs_c_code(char **l, int *llen, int tabs_num);

/** Kodly setiri ýazýar. Eger kodly setir blogyň içinde bolsa, onda öňünden TAB goýýar */
void write_code_line(FILE *f, char **l, int *llen, int tabs_num, char *code);

// Header fayly ulanmaga tayynlayar
int prepare_h_source(FILE *f, char *f_name);
int finishize_h_source(FILE *f);
int prepare_c_source(FILE *f, char *h_source);

// Global funksiya yglan etmelerini fayllara goshyar.
int c_trans_header_add_glob_def_var(FILE *f);
int c_trans_source_add_glob_def_var(FILE *f);

// Global funksiya yglan etmelerini fayllara goshyar.
int c_trans_header_add_glob_def_arr(FILE *f);
int c_trans_source_add_glob_def_arr(FILE *f);

// Lokal yglan edilen funksiyalary ichine goshyar
int c_trans_source_add_loc_def_var(FILE *f, char main_file);

int prepare_main_func(FILE *f);
int finishize_main_func(FILE *f);
int finishize_void_func(FILE *f);

int c_trans_source_add_algor(FILE *f, int main_file);

// Lokal faylyn algoritmlerini yasayar
int work_with_translator(char main_file);

int c_trans_write_file_fn_prototype(FILE *f_h, char *fn_name);
int c_trans_write_file_fn_open(FILE *f_h, char *fn_name);
char *c_trans_get_file_fn_prototype(char *fn_name, char *out);

int is_open_block_cmd(command *cmd);
int is_close_block_cmd(command *cmd);

#endif
