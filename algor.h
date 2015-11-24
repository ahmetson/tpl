/*
 * Algoritmli fayly yasayan funksiya
**/
#ifndef ALGOR_H
#define ALGOR_H

#define MAX_FILE_LEN 255
#define MAX_TOKEN_LEN 32

#include "cmds.h"
#include "tokens.h"

// Algoritmde komandany goshyar
int algor_add_cmd(command cmd);

void var_def_add(command *cmd, char glob);

// Wagtlayyn fayllaryn rasshireniyasyny
extern char loc_head_file[MAX_FILE_LEN];
extern char loc_source_file[MAX_FILE_LEN];

//void write_loc_def_var();
int is_glob_var_def_exist(char *name);
int is_local_var_def_exist(char *name);
int is_var_def_exist(char *name);

glob_ident *glob_vars_def_get_by_name(char *ident);

// Hazirki parsing edilen faylda global ulni yglan edileninin jogabyny gaytaryar
int is_glob_def_var_in_cur();

/** Identifikatoryň eýýäm ýasaljak programmada ulanylanmy ýa ýoklugy barlanýar.
    \exceptNum - Identifikatoryň haýsy böleklerde barlanmaýan bolsa, şol bölegiň nomeri.
        0 - hemme böleklerde barlanylýar.
        1 - global yglan edilen ülňileriň maglumatlarynyň sanawynda barlananok.
            (Bu ülňiler pragma arkaly parsing edilen faýllardan alynýar)
**/
int is_ident_used(token *t, char except_num);
#endif
