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

#ifndef FN_UNVALIDE_INSIDE_CMDS_NUM
#define FN_UNVALIDE_INSIDE_CMDS_NUM 2
#endif // FN_UNVALIDE_INSIDE_CMDS_NUM

extern int unvalid_fn_inside_cmds[][2];

extern int FUNC_CLASS_3RD_PARTY;
extern int FUNC_TYPE_STD;
extern int FUNC_CLASS_USER_DEF;

typedef struct{
    int type_class;
    int type_num;
}func_arg;

#include "../parenthesis.h"
typedef struct{
    int file_num;
    wchar_t file_name[MAX_FILE_LEN];
    wchar_t name[MAX_FILE_LEN];
    int type_class;
    int type_num;
    int args_num;
    int args;
    wchar_t inf_wchar_t;
    int  inf_wchar_t_pos;
    int  inf_line_pos;
    int  return_class;
    int  return_type;
    wchar_t c_name[MAX_FILE_LEN];
    wchar_t  c_lib[MAX_FILE_LEN];
    void (*make_args_string)(parenthesis *paren, wchar_t **l, int *llen);
}func;

void add_func_to_list(func f);


/** Funksiýanyň ady häzir TPL'e belli bolan funksiýalaryň arasynda gözlenilýär.

    \fnName - gözlemeli funksiýanyň ady
**/
int is_fn_exist(wchar_t *fn_name);


/** Funksiýanyň maglumatlaryny gaýtarýar. Maglumatlaryny gaýtarmaly funksiýa ady boýunça alynýar
    Bu funksiýa hökmän is_fn_exist() funksiýa şowly bolan soň çagyrylmaly

    \fnName - maglumatlary alynmaly funksiýanyň ady
**/
func *fn_get_by_name(wchar_t *fn_name);


func_arg *get_fn_args(int args_num);


void add_arg_to_last_func(func_arg arg, int pos);


void make_user_def_fn_args(parenthesis *paren, wchar_t **line, int *llen);

#endif // FNS_FN_H
