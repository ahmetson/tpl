/** Ýaýlaryň tipleri bilen işleýän funksiýalar we maglumatlar
**/
#ifndef PAREN_TYPES_H
#define PAREN_TYPES_H

#include "../parenthesis.h"

#ifndef CONST_PAREN_TYPES_NUM
#define CONST_PAREN_TYPES_NUM 4
#endif // CONST_PAREN_TYPES_NUM

extern int PAREN_TYPE_EMPTY;
extern int PAREN_TYPE_FNS_ARGS;
extern int PAREN_TYPE_DEF_FNS_ARGS;
extern int PAREN_TYPE_DEF_TYPE;

extern int (*PAREN_TYPES[])(parenthesis *paren);

extern char *PAREN_TYPES_WORDS[];

int paren_type_is_empty(parenthesis *paren);
int paren_type_is_fns_args(parenthesis *paren);
int paren_type_is_def_fns_args(parenthesis *paren);
int paren_type_is_def_type(parenthesis *paren);

int empty_paren_return_type(parenthesis *paren, int *ret_class, int *ret_num);
int paren_fns_args_return_type(parenthesis *paren, int *ret_class, int *ret_num);

int is_param_item_int(parenthesis *par);

#endif // PAREN_TYPES_H

