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
        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(&add_cmd));
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


int add_user_var_def_item(command cmd)
{
    int ident_tok_pos = (is_glob_def_var_cmd(&cmd)) ? 2 : 1;
    char *tok_name = cmd.items[ident_tok_pos].tok.potentional_types[0].value;

	// Identifikator eyyam yglan edilen eken.
	if (is_ident_used(tok_name, cmd.ns))
        return 0;
char file_name[MAX_FILE_LEN];		// Ülňi haýsy faýla degişli
	int  line;
	int  start_char_position;
	char start_char;
	char ident[MAX_TOKEN_LEN];		    // Ülňiniň ady
	int  tok_class;						// Ülňiniň tipi
	int  tok_type;
	int  ns;
	var_def_item new_def = {
		"",
		cmd.items[ident_tok_pos-1].tok.inf_file_num,
		cmd.items[ident_tok_pos-1].tok.inf_line_num,
		cmd.items[ident_tok_pos-1].tok.inf_char_num,
		cmd.items[ident_tok_pos-1].tok.inf_char,
		"",
		cmd.items[ident_tok_pos-1].tok.potentional_types[0].type_class,
		cmd.items[ident_tok_pos-1].tok.potentional_types[0].type_num,
        cmd.ns
	};
	strncpy(new_def.file_name, CUR_FILE_NAME, strlen(CUR_FILE_NAME)+1);
	strncpy(new_def.ident, tok_name, strlen(tok_name)+1);

    // Taze gosuljak komandanyn gowrumi, kuchadaky eyelenen gowrume goshulyar
	long size = sizeof(new_def) * (USER_VAR_DEFS_NUM+1);

	if (USER_VAR_DEFS_NUM)
        USER_VAR_DEFS = realloc(USER_VAR_DEFS, size);
    else
        USER_VAR_DEFS = malloc(size);

	USER_VAR_DEFS[USER_VAR_DEFS_NUM++] = new_def;

	return 1;
}


// Lokal fayllar uchin
char loc_head_file[] = ".h";
char loc_source_file[] = ".c";

int is_glob_def_var_in_cur()
{
	int i;
	for(i=0; i<USER_VAR_DEFS_NUM; ++i)
	{
	    if (USER_VAR_DEFS[i].ns!=GLOB)
            continue;
		if (strlen(USER_VAR_DEFS[i].file_name)==strlen(CUR_FILE_NAME) &&
			!strncmp(USER_VAR_DEFS[i].file_name, CUR_FILE_NAME, strlen(CUR_FILE_NAME)))
			return 1;
	}
	return 0;
}


/*
 * Ulninin ady boyuncha onun on yglan edilendigini barlayar.
**/
int is_ident_used(char *ident, int ns)
{
    int i, len;
    // Lokal yglan edilen ulnilerin arasynda barlanyar
    for (i=0; i<USER_VAR_DEFS_NUM; ++i)
    {
        if (ns==USER_VAR_DEFS[i].ns && ns==LOCAL)
        {
            len = (strlen(ident)<strlen(USER_VAR_DEFS[i].ident))?strlen(USER_VAR_DEFS[i].ident):strlen(ident);
            // Lokal yglan edilen ülňileriň atlary deň bolmaly
            if (strncmp(USER_VAR_DEFS[i].ident, ident, len)==0)
            {
                len = (strlen(ident)<strlen(USER_VAR_DEFS[i].ident))?strlen(USER_VAR_DEFS[i].ident):strlen(ident);
                // Ülňi atlary deň bolmaly
                if (strncmp(USER_VAR_DEFS[i].ident, ident, len)==0)
                    return 1;
            }
        }
        else
        {
            len = (strlen(ident)<strlen(USER_VAR_DEFS[i].ident))?strlen(USER_VAR_DEFS[i].ident):strlen(ident);
            if (strncmp(USER_VAR_DEFS[i].ident, ident, len)==0)
                return 1;
        }
    }

    return 0;
}
