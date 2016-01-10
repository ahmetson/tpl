/**
 * Ýasalan kodlar bilen işleýän funksiýalar hem-de
 * Ýasalan kodlara degişli global ülňilerdäki magtlumatlar bilen işleýän funksiýalar
**/

#ifndef TRANSLATOR_TO_C_FILES_H
#define TRANSLATOR_TO_C_FILES_H
#include <stdio.h>
#include "../main/main_file.h"

wchar_t *get_header_source_by_source(wchar_t *s);
wchar_t *get_c_source_by_source(wchar_t *s);

int add_includes_to_source(FILE *s, wchar_t (*included_files)[MAX_FILE_LEN], int files_num);

#endif
