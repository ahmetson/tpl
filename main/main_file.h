/**
 * Baş ýasaljak kodly faýl bilen bagly ähli maglumatlar saklanýar
**/

#ifndef TPL_MAIN_FILE_H
#define TPL_MAIN_FILE_H
#include "files.h"
#ifndef MAX_INCLUDE_LEN
#define MAX_INCLUDE_LEN MAX_FILE_LEN+10
#endif // MAX_INCLUDE_LEN

extern wchar_t            MAIN_FILE_NAME[MAX_FILE_LEN];
extern int             MAIN_FILE_INCLUDES_NUM;
extern wchar_t            (*MAIN_FILE_INCLUDES)[2][MAX_INCLUDE_LEN];

/**
 * Baş ýasaljak kodyň soňuna, başga ýasaljak kodlaryň algoritmlerini çagyrjak funksiýalar goýulýar.
**/
int add_addtn_file_fns();

void write_to_csource_call_another_files_fn_open( FILE *f );
void trans_to_c_write_addtn_fn_multi_free( FILE *f );
void write_to_csource_call_another_files_fn_close(  );

#endif
