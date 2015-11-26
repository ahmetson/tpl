/** Funksiýalar bilen işleýär
 *
**/
#ifndef FNS_FN_H
#define FNS_FN_H

#include "../tokens.h"

#ifndef MAX_FILE_LEN
#define MAX_FILE_LEN 255
#endif

#ifndef FN_ARGS_MULTIPLE
#define FN_ARGS_MULTIPLE -1
#endif

extern int FUNC_CLASS_3RD_PARTY;
extern int FUNC_TYPE_STD;

typedef struct{
    int type_class;
    int type_num;
}func_arg;

#include "../parenthesis.h"
typedef struct{
    int file_num;
    char file_name[MAX_FILE_LEN];
    char name[MAX_FILE_LEN];
    int type_class;
    int type_num;
    int args_num;
    func_arg *args;
    char inf_char;
    int  inf_char_pos;
    int  inf_line_pos;
    int  return_class;
    int  return_type;
    char c_name[MAX_FILE_LEN];
    char  c_lib[MAX_FILE_LEN];
    void (*make_args_string)(parenthesis *paren, char **l, int *llen);
}func;

void add_func_to_list(func f);


/** Funksiýanyň ady häzir TPL'e belli bolan funksiýalaryň arasynda gözlenilýär.

    \fnName - gözlemeli funksiýanyň ady
**/
int is_fn_exist(char *fn_name);


/** Funksiýanyň maglumatlaryny gaýtarýar. Maglumatlaryny gaýtarmaly funksiýa ady boýunça alynýar
    Bu funksiýa hökmän is_fn_exist() funksiýa şowly bolan soň çagyrylmaly

    \fnName - maglumatlary alynmaly funksiýanyň ady
**/
func *fn_get_by_name(char *fn_name);


void add_arg_to_last_func(func_arg arg, int pos);

#endif // FNS_FN_H
