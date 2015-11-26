/*
 * Algoritmli fayly yasayan funksiya
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main/glob.h"
#include "tpl.h"
#include "algor.h"
#include "translator_to_c.h"
#include "semantic.h"
#include "error.h"
#include "dev_debug.h"
#include "main/inf.h"
#include "cmd/def_var.h"

int algor_add_cmd(command add_cmd)
{
    // komanda dogrylygy barlanyar
    check_semantics(&add_cmd);

    ++GLOB_SUBCMDS_NUM;
	GLOB_SUBCMD_ITEMS_LIST = realloc(GLOB_SUBCMD_ITEMS_LIST, sizeof(*GLOB_SUBCMD_ITEMS_LIST)*GLOB_SUBCMDS_NUM);
    if (GLOB_SUBCMD_ITEMS_LIST==NULL)
    {
        printf("SALAM 1");
        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(&add_cmd));
    }
    else
    {
        long size = cmd.items_num * sizeof(**GLOB_SUBCMD_ITEMS_LIST);
        GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1] = malloc(size);

            // Ichki komanda uchin yere, birlikler gechirilyar.
        int i=0;
        for (i=0; i<add_cmd.items_num; ++i)
        {

            GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1][i] = add_cmd.items[i];

        }
        add_cmd.items = GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1];

        /*
        int size = sizeof(cmd);
        if (cmd.tokens_num>0)
            size += (cmd.tokens_num * sizeof(token));*/
        // Taze gosuljak komandanyn gowrumi, kuchadaky eyelenen gowrume goshulyar
        CUR_ALGOR_SIZE += sizeof(*CUR_ALGOR);
        if (CUR_ALGOR_SIZE)
            CUR_ALGOR = realloc(CUR_ALGOR, CUR_ALGOR_SIZE);
        else
            CUR_ALGOR = malloc(CUR_ALGOR_SIZE);

        // TODO: komandanyn kopiyasyny goymaly.
        CUR_ALGOR[CUR_ALGOR_ITEMS_NUM++] = add_cmd;
    }
	return 1;
}


void var_def_add(command *cmd, char glob)
{
    int ident_tok_pos = 2;  // GLOBAL: @, def_type, ident
    if (!glob)
        ident_tok_pos = 1;  //  LOKAL: def_type, ident

    char *tok_name = cmd->items[ident_tok_pos].tok.potentional_types[0].value;

	// Identifikator eyyam yglan edilen eken.
	if (glob)
    {
        if (is_ident_used(&cmd->items[ident_tok_pos].tok, 1))
        {
            CUR_PART = 4;
            print_err(CODE4_VARS_IDENT_USED, &cmd->items[ident_tok_pos].tok);
        }
    }
    else
    {
        if (is_ident_used(&cmd->items[ident_tok_pos].tok, 0))
        {
            CUR_PART = 4;
            print_err(CODE4_VARS_IDENT_USED, &cmd->items[ident_tok_pos].tok);
        }
    }

	glob_ident new_def = {
	    cmd->items[ident_tok_pos-1].tok.potentional_types[0].type_class,
		cmd->items[ident_tok_pos-1].tok.potentional_types[0].type_num,
		"",
		cmd->items[ident_tok_pos-1].tok.inf_file_num,
        cmd->items[ident_tok_pos-1].tok.inf_char_num,
		cmd->items[ident_tok_pos-1].tok.inf_char,
		cmd->items[ident_tok_pos-1].tok.inf_line_num
	};

	strncpy(new_def.name, tok_name, strlen(tok_name)+1);

    // Taze gosuljak komandanyn gowrumi, kuchadaky eyelenen gowrume goshulyar
    if (glob)
    {
        ++GLOBAL_VAR_DEFS_NUMS;
        GLOBAL_VAR_DEFS = realloc(GLOBAL_VAR_DEFS, sizeof(*GLOBAL_VAR_DEFS)*GLOBAL_VAR_DEFS_NUMS);
        GLOBAL_VAR_DEFS[GLOBAL_VAR_DEFS_NUMS-1] = new_def;
    }
    else
    {
        ++LOCAL_VAR_DEFS_NUMS;
        LOCAL_VAR_DEFS = realloc(LOCAL_VAR_DEFS, sizeof(*LOCAL_VAR_DEFS)*LOCAL_VAR_DEFS_NUMS);
        LOCAL_VAR_DEFS[LOCAL_VAR_DEFS_NUMS-1] = new_def;
    }
}


// Lokal fayllar uchin
char loc_head_file[] = ".h";
char loc_source_file[] = ".c";

int is_glob_def_var_in_cur()
{
	int i;
	for(i=0; i<GLOBAL_VAR_DEFS_NUMS; ++i)
	{
		if (GLOBAL_VAR_DEFS[i].inf_file_num==CUR_FILE_NUM-1)
			return 1;
	}
	return 0;
}

int is_glob_var_def_exist(char *name)
{
    int i, len;
    for (i=0; i<GLOBAL_VAR_DEFS_NUMS; ++i)
    {
        len = strlen(name)>strlen(GLOBAL_VAR_DEFS[i].name)?strlen(name):strlen(GLOBAL_VAR_DEFS[i].name);
        if (strncmp(GLOBAL_VAR_DEFS[i].name, name, len)==0)
        {
            return 1;
        }
    }
    return 0;
}


int is_local_var_def_exist(char *name)
{
    int i, len;
    for (i=0; i<LOCAL_VAR_DEFS_NUMS; ++i)
    {
        len = strlen(name)>strlen(LOCAL_VAR_DEFS[i].name)?strlen(name):strlen(LOCAL_VAR_DEFS[i].name);
        if (strncmp(LOCAL_VAR_DEFS[i].name, name, len)==0)
        {
            return 1;
        }
    }
    return 0;
}

int is_var_def_exist(char *ident)
{
    if (is_glob_var_def_exist(ident) ||
        is_glob_var_dec_exist(ident) ||
        is_local_var_def_exist(ident))
        return 1;
}

/*
 * Ulninin ady boyuncha onun on yglan edilendigini barlayar.
**/
int is_ident_used(token *t, char except_code)
{
    char *ident = t->potentional_types[0].value;

    /// Ähli bolup biljek identifikatorlaryň bölümlerinde barlamaly.
    ///     Ülňileriň adymy, funksiýalaryň adymy, açar sözlerimi we ş.m.
    if (!except_code)
    {
        return is_var_def_exist(ident);
    }
    /// Global ülňileriň maglumatlary yglan edilen sanawda identifikator gözlenenok.
    else if (except_code==1)
    {
        return is_local_var_def_exist(ident) || is_glob_var_def_exist(ident);
    }
    return 0;
}


glob_ident *glob_vars_def_get_by_name(char *name)
{
    int i, len;
    glob_ident *ret;
    for (i=0; i<GLOBAL_VAR_DEFS_NUMS; ++i)
    {
        //printf("%d: check <%s> that defined <%s>\n, current <%s> defined in <%s>\n", USER_VAR_DEFS_NUM, ident, f, USER_VAR_DEFS[i].ident, USER_VAR_DEFS[i].file_name);
        // Lokal ülňiler üçin, funksiýanyň ady gabat gelmeli.
        len = strlen(name)>strlen(GLOBAL_VAR_DEFS[i].name)?strlen(name):strlen(GLOBAL_VAR_DEFS[i].name);
        if (strncmp(GLOBAL_VAR_DEFS[i].name, name, len)==0)
        {
            ret = &GLOBAL_VAR_DEFS[i];
            break;
        }
    }
    return ret;
}
