/**
 * Baş ýasaljak kodly faýl bilen bagly ähli maglumatlar saklanýar
**/

#ifndef TPL_MAIN_FILE_H
#define TPL_MAIN_FILE_H

#ifndef MAX_FILE_LEN
#define MAX_FILE_LEN 255
#endif // MAX_FILE_LEN
#ifndef MAX_INCLUDE_LEN
#define MAX_INCLUDE_LEN MAX_FILE_LEN+10
#endif // MAX_INCLUDE_LEN

extern char            MAIN_FILE_NAME[MAX_FILE_LEN];
extern int             MAIN_FILE_INCLUDES_NUM;
extern char            (*MAIN_FILE_INCLUDES)[2][MAX_INCLUDE_LEN];

/**
 * Baş ýasaljak kodyň soňuna, başga ýasaljak kodlaryň algoritmlerini çagyrjak funksiýalar goýulýar.
**/
int add_addtn_file_fns();

#endif
