/**
 * Ýasalan kodlar bilen işleýän funksiýalar hem-de
 * Ýasalan kodlara degişli global ülňilerdäki magtlumatlar bilen işleýän funksiýalar
**/
#include <stdio.h>

#ifndef TRANSLATOR_TO_C_FILES_H
#define TRANSLATOR_TO_C_FILES_H

char *get_used_glob_file_name(char *ident);
char *get_header_source_by_source(char *s);
char *get_c_source_by_source(char *s);

int add_includes_to_source(FILE *s, char (*included_files)[255], int files_num);

#endif
