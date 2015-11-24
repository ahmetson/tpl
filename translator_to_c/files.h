/**
 * Ýasalan kodlar bilen işleýän funksiýalar hem-de
 * Ýasalan kodlara degişli global ülňilerdäki magtlumatlar bilen işleýän funksiýalar
**/

#ifndef TRANSLATOR_TO_C_FILES_H
#define TRANSLATOR_TO_C_FILES_H
#include <stdio.h>
#include "../main/main_file.h"

char *get_header_source_by_source(char *s);
char *get_c_source_by_source(char *s);

int add_includes_to_source(FILE *s, char (*included_files)[MAX_FILE_LEN], int files_num);

#endif
