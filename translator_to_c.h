/*
 * Translyator, yagny C kodyny yasamak bilen ishleyan funksiyalar
**/
#ifndef TRANSLATOR_TO_C_H
#define TRANSLATOR_TO_C_H

#include <stdio.h>

// C kod nirede saklanyar?
extern char *C_SOURCE_FOLDER;

// Header fayly ulanmaga tayynlayar
int prepare_h_source(FILE *f, char *f_name);
int finishize_h_source(FILE *f);
int prepare_c_source(FILE *f, char *h_source);

// Global funksiya yglan etmelerini fayllara goshyar.
int c_trans_header_add_glob_def_var(FILE *f);
int c_trans_source_add_glob_def_var(FILE *f);

// Lokal yglan edilen funksiyalary ichine goshyar
int c_trans_source_add_loc_def_var(FILE *f, char main_file);

int prepare_main_func(FILE *f);
int finishize_main_func(FILE *f);
#endif
