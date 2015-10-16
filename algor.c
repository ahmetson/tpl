/*
 * Algoritmli fayly yasayan funksiya
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "algor.h"
#include "translator_to_c.h"
#include "error.h"
#include "dev_debug.h"

int algor_add_cmd(command add_cmd)
{
    // komanda dogrylygy barlanyar
    check_semantics(&add_cmd);

    GLOB_SUBCMDS_NUM++;
	GLOB_SUBCMD_ITEMS_LIST = realloc(GLOB_SUBCMD_ITEMS_LIST, sizeof(command_item*)*GLOB_SUBCMDS_NUM);
    if (GLOB_SUBCMD_ITEMS_LIST==NULL)
    {
        //printf("Ichki komandalar uchin yer taplymady\n");
        print_err(CODE4_CANT_IDENT_CMD);
    }
    else
    {
        long size = cmd.items_num * sizeof(command_item);
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
        long new_CUR_ALGOR_SIZE = CUR_ALGOR_SIZE+sizeof(add_cmd);
        if (CUR_ALGOR_SIZE)
            CUR_ALGOR = realloc(CUR_ALGOR, new_CUR_ALGOR_SIZE);
        else
            CUR_ALGOR = malloc(new_CUR_ALGOR_SIZE);

        // TODO: komandanyn kopiyasyny goymaly.
        CUR_ALGOR[CUR_ALGOR_ITEMS_NUM++] = add_cmd;

        CUR_ALGOR_SIZE = new_CUR_ALGOR_SIZE;
    }
	return 1;
}

// Global yglan edilen funksiyalaryn hataryna yglan edilen ulni goshulyar
int glob_vars_add_cmd(command cmd)
{
	// Yglan edilen ulniler boyuncha gidilyar.
	// Eger eyyam sheyle ulni yglan edilen bolsa, chykylyar.

	// Lokal yglan edilen funksiyalar boyuncha barlanyar.
	// Eger eyyam sheyle lokal ulni yglan edilen bolsa, yalnyshlyk gaytarylyar.
	char *tok_name = cmd.items[2].tok.potentional_types[0].value;


	// Identifikator eyyam yglan edilen eken.
	if (is_ident_used(tok_name))
        return 0;

	/*
	int size = sizeof(cmd);
	if (cmd.tokens_num>0)
		size += (cmd.tokens_num * sizeof(token));*/
	//printf("%s -> %s\n", CUR_FILE_NAME, tok_name);
	//printf("%d -> %d", cmd.tokens[1].potentional_types[0].type_class, cmd.tokens[1].potentional_types[0].type_num);
	//debug_cmd(&cmd);
	global_def_var new_def = {
		"",
		"",
		cmd.items[1].tok.potentional_types[0].type_class,
		cmd.items[1].tok.potentional_types[0].type_num
	};
	strncpy(new_def.file_name, CUR_FILE_NAME, strlen(CUR_FILE_NAME)+1);
	strncpy(new_def.tok_name, tok_name, strlen(tok_name)+1);


	//debug_token(&cmd.tokens[1]);
	//printf("\n%s -> %s\n", new_def.file_name, new_def.tok_name);
	//printf("%d -> %d\n", new_def.tok_class, new_def.tok_type);


	// Taze gosuljak komandanyn gowrumi, kuchadaky eyelenen gowrume goshulyar
	GLOB_VAR_DEFS_SIZE += sizeof(new_def);
	//printf("%d new size\n", new_CUR_ALGOR_SIZE);
	if (GLOB_VAR_DEFS_NUM)
        GLOB_VAR_DEFS = realloc(GLOB_VAR_DEFS, GLOB_VAR_DEFS_SIZE);
    else
        GLOB_VAR_DEFS = malloc(GLOB_VAR_DEFS_SIZE);

	GLOB_VAR_DEFS[GLOB_VAR_DEFS_NUM++] = new_def;

	return 1;

}


int loc_vars_add_cmd(command cmd)
{
	char *tok_name = cmd.items[1].tok.potentional_types[0].value;

    // Identifikator eyyam yglan edilen eken.
	if (is_ident_used(tok_name))
        return 0;
	/*
	int size = sizeof(cmd);
	if (cmd.tokens_num>0)
		size += (cmd.tokens_num * sizeof(token));*/
	//printf("%s -> %s\n", CUR_FILE_NAME, tok_name);
	//printf("%d -> %d", cmd.tokens[0].potentional_types[0].type_class, cmd.tokens[0].potentional_types[0].type_num);
	//debug_cmd(&cmd);
	//debug_cmd(&cmd);
	//debug_token_type(&cmd.tokens[0].potentional_types[0]);
	local_def_var new_def = {
		"",
		cmd.items[0].tok.potentional_types[0].type_class,
		cmd.items[0].tok.potentional_types[0].type_num
	};
	strncpy(new_def.tok_name, tok_name, strlen(tok_name)+1);


	//debug_token(&cmd.tokens[1]);
	//printf("%d -> %d\n", new_def.tok_class, new_def.tok_type);


	// Taze gosuljak komandanyn gowrumi, kuchadaky eyelenen gowrume goshulyar
	long new_size = LOCAL_VAR_DEFS_SIZE+sizeof(new_def);
	//printf("%d new size\n", new_CUR_ALGOR_SIZE);
	LOCAL_VAR_DEFS = realloc(LOCAL_VAR_DEFS, new_size);

	LOCAL_VAR_DEFS[LOCAL_VAR_DEFS_NUM++] = new_def;

	LOCAL_VAR_DEFS_SIZE = new_size;

	return 1;
}


// Lokal fayllar uchin
char loc_head_file[] = ".h";
char loc_source_file[] = ".c";

int is_glob_def_var_in_cur()
{
	int i;
	for(i=0; i<GLOB_VAR_DEFS_NUM; ++i)
	{
		if (strlen(GLOB_VAR_DEFS[i].file_name)==strlen(CUR_FILE_NAME) &&
			!strncmp(GLOB_VAR_DEFS[i].file_name, CUR_FILE_NAME, strlen(CUR_FILE_NAME)))
			return 1;
	}
	return 0;
}


/*
 * Ülňiniň ady boýunça onuň global ülňileriň arasynda yglan edilendigini barlaýar
**/
int is_ident_glob_used(char *ident)
{
    // Global yglan edilen ulnilerin arasynda barlanyar
    int i, len;
    for (i=0; i<GLOB_VAR_DEFS_NUM; ++i)
    {
        len = (strlen(ident)<strlen(GLOB_VAR_DEFS[i].tok_name))?strlen(GLOB_VAR_DEFS[i].tok_name):strlen(ident);
        if (strncmp(GLOB_VAR_DEFS[i].tok_name, ident, len)==0)
            return 1;
    }

    return 0;
}

/*
 * Ulninin ady boyuncha onun on yglan edilendigini barlayar.
**/
int is_ident_used(char *ident)
{
    int i, len;
    // Lokal yglan edilen ulnilerin arasynda barlanyar
    for (i=0; i<LOCAL_VAR_DEFS_NUM; ++i)
    {
        len = (strlen(ident)<strlen(LOCAL_VAR_DEFS[i].tok_name))?strlen(LOCAL_VAR_DEFS[i].tok_name):strlen(ident);
        if (strncmp(LOCAL_VAR_DEFS[i].tok_name, ident, len)==0)
            return 1;
    }

    // Global yglan edilen ulnilerin arasynda barlanyar
    return is_ident_glob_used(ident);
}
