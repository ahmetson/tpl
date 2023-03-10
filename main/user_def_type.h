#ifndef MAIN_USER_DEF_TYPE_H
#define MAIN_USER_DEF_TYPE_H

#include "glob.h"
#include "../cmds.h"

typedef struct {
    wchar_t ident[MAX_VAR_LEN];
    int   items_num;
}utype_type;

typedef struct {
    wchar_t ident[MAX_VAR_LEN];
    int  type_class;
    int  type_num;
    int  arr_items_addr;    // Eger birlik birsyhly sanaw bolsa, birlikleriniň adresi alynýar.
}utype_item_type;

extern wchar_t             *USER_DEF_TYPES_SOURCE_NAME_NO_H;
extern wchar_t             *USER_DEF_TYPES_SOURCE_NAME;

extern utype_type       *USER_DEF_TYPES;
extern utype_item_type **USER_DEF_TYPE_ITEMS;
extern int               USER_DEF_TYPES_NUM;

extern array_inc_item ***USER_DEF_TYPES_ARRS_ITEMS;
extern array_item      **USER_DEF_TYPES_ARRS;
extern int              *USER_DEF_TYPES_ARRS_NUMS;

extern utype_item_type  *TMP_USER_DEF_TYPE_ITEMS;
extern int               TMP_USER_DEF_TYPES_NUM;
extern array_inc_item  **TMP_USER_DEF_TYPES_ARRS_ITEMS;
extern array_item       *TMP_USER_DEF_TYPES_ARRS;
extern int               TMP_USER_DEF_TYPES_ARRS_NUMS;

int get_utype_addr(wchar_t *ident, int *utype_addr);
int get_utype_item_addr(int utype_num, wchar_t *item_ident, int *utype_item_addr);
int is_utype_arr_itemy(int utype_num, int item_num);
int get_utype_addr_by_cmd(command *cmd, int *utype_addr);
int get_utype_item_addr_by_cmd(command *cmd, int *utype_item_addr);


void add_addtn_headers(FILE *f);

void add_utype_define_c_code();

int is_reserved_source_name(wchar_t *fname);

void parse_triangle_block_inside();

void exit_triangle_block_parser();

int is_triangle_block_support_cmd(command *cmd);

void add_to_tmp_triangle_blok_items(command *cmd);

void free_tmp_user_type();

int is_utype_ident(wchar_t *ident);

int get_utype_item_addr_by_cmd(command *cmd, int *utype_item_addr);

int get_arr_item_type_by_cmd(command *cmd, int *ret_class, int *ret_type);

int get_utype_item_type_by_cmd(command *cmd, int *ret_class, int *ret_type);

int is_utype_arr_item(int utype_num, int item_num);

int get_utype_item_type(int utype_num, wchar_t *item_ident, int *ret_class, int *ret_type);

void add_new_utype(command *cmd);
void move_tmp_utype_items_to_last();

#endif // MAIN_USER_DEF_TYPE_H
