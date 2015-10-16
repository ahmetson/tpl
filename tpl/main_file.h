/**
 * Baş ýasaljak kodly faýl bilen bagly ähli maglumatlar saklanýar
**/

#ifndef TPL_MAIN_FILE_H
#define TPL_MAIN_FILE_H

#define CONST_MAX_FILE_LEN 255

extern char            MAIN_FILE_NAME[CONST_MAX_FILE_LEN];
extern int             MAIN_FILE_INCLUDES_NUM;
extern char            (*MAIN_FILE_INCLUDES)[2][265];

/**
 * Baş ýasaljak kodyň soňuna, başga ýasaljak kodlaryň algoritmlerini çagyrjak funksiýalar goýulýar.
**/
int add_addtn_file_fns();

#endif
