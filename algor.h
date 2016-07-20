/** Algoritmli fayly yasayan funksiya */
#ifndef ALGOR_H
#define ALGOR_H

#define MAX_TOKEN_LEN 32

#include "cmds.h"
#include "tokens.h"
#include "cmd/array.h"

typedef struct{
    int type_class;
    int type_num;
    int inc_num;
    int inf_file_num;
    int inf_line_num;
    int inf_wchar_t_num;
    wchar_t inf_wchar_t;
}block_inc;

// Algoritmde komandany goshyar
int algor_add_cmd(command cmd);

void var_def_add(command *cmd, int glob);

void arr_def_add(command *cmd, int glob);

// Wagtlayyn fayllaryn rasshireniyasyny
extern wchar_t loc_head_file[MAX_FILE_LEN];
extern wchar_t loc_source_file[MAX_FILE_LEN];

//void write_loc_def_var();
int is_glob_var_def_exist(wchar_t *name);
int is_local_var_def_exist(wchar_t *name);
int is_var_def_exist(wchar_t *name);

//void write_loc_def_var();
int is_glob_arr_def_exist(wchar_t *name);
int is_local_arr_def_exist(wchar_t *name);
int is_arr_def_exist(wchar_t *name);

glob_ident *glob_vars_def_get_by_name(wchar_t *ident);
array_item *glob_arrs_def_get_by_name(wchar_t *name);


// Hazirki parsing edilen faylda global ulni yglan edileninin jogabyny gaytaryar
int is_glob_def_var_in_cur();

/** Identifikatoryň eýýäm ýasaljak programmada ulanylanmy ýa ýoklugy barlanýar.
    \exceptNum - Identifikatoryň haýsy böleklerde barlanmaýan bolsa, şol bölegiň nomeri.
        0 - hemme böleklerde barlanylýar.
        1 - global yglan edilen ülňileriň maglumatlarynyň sanawynda barlananok.
            (Bu ülňiler pragma arkaly parsing edilen faýllardan alynýar)
**/
int is_ident_used(token *t, wchar_t except_num);

/// Bloklar ýaýlaryň içinde bolup bolanok. Şonuň
/// üçin diňe komandalary parsing edilýän funksiýa-da çagyrylýar.
void work_with_blocks();
void open_block();
void close_block();


block_inc *get_block_by_inc_num(int inc_num);

int is_tmp_triangle_block_item_ident(wchar_t *ident);

int is_glob_def_arr_in_cur();

#endif
