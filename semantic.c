/*
 * Semantikany barlayan funksiya
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tpl.h"
#include "cmds.h"
#include "semantic.h"
#include "error.h"
#include "dev_debug.h"

// Komandanyn semantikasyny barlayar.
int check_semantics(command *cmd)
{
    // Komandanyn gornushi boyuncha, shol gornush boyuncha semantikany barlayan funksiya chagyrylyar
    if (cmd->cmd_class==ASSIGN_CLASS_NUM)
        return semantic_cmd_assign(cmd);
    return 0;
}

// Baglama komandasynyn semantikasy
int semantic_cmd_assign(command *cmd)
{
    int prev_part = CUR_PART;
    CUR_PART = 7;
    // KOMANDANYN CHEPE BAGLANMA GORNUSHI UCHIN
    if (cmd->cmd_type==LEFT_ASSIGN_TOK_NUM)
    {
        //printf("Chepe baglanyan komandanyn semantikasy barlanmaly\n");

        // BIRINJI BIRLIK BARLANYAR
        //      ulninin yglan etme bolsa                      - identifikator hokman goshulandyr shonun uchin barlananok
        //      eyyam ygaln edilen ulnin identifikatory bolsa - identifikator hokman on bir yerde yglan edilmeli
        if (cmd->items[0].type==TOKEN_ITEM && cmd->items[0].tok.type_class==TOK_CLASS_IDENT)
        {
            //debug_cmd(cmd);
            //debug_GLOB_VAR_DEFS(GLOB_VAR_DEFS);
            //debug_LOCAL_VAR_DEFS(LOCAL_VAR_DEFS);
            token item = cmd->items[0].tok; // Gysgaltmak uchin ulanlyar

            if (!is_ident_used(item.potentional_types[0].value, cmd->ns))
            {
                add_global_used_var(&item, item.potentional_types[0].value);
                //print_err(CODE7_LEFT_IDENT_NOT_DEFINED);
            }
        }


        // IKINJI BIRLIGIN BARLANMASY
        //      eyyam yglan edilen ulnin identifikatory bolsa - identifikator hokman on bir yerde yglan edilmeli
        if (cmd->items[2].type==TOKEN_ITEM && cmd->items[2].tok.type_class==TOK_CLASS_IDENT)
        {
            //debug_cmd(cmd);
            //debug_GLOB_VAR_DEFS(GLOB_VAR_DEFS);
            //debug_LOCAL_VAR_DEFS(LOCAL_VAR_DEFS);
            token item = cmd->items[2].tok; // Gysgaltmak uchin ulanlyar
            if (!is_ident_used(item.potentional_types[0].value, cmd->ns))
            {
                add_global_used_var(&item, item.potentional_types[0].value);
                //print_err(CODE7_LEFT_IDENT_NOT_DEFINED);
            }
        }
    }
    // KOMANDANYN CHEPE BAGLANMA GORNUSHI UCHIN

    CUR_PART = prev_part;
    return 1;
}


/**
 * Ulanylýan global ülňileriň hataryna goşýar
**/
int add_global_used_var(token *tok, char *tok_name)
{
    var_def_item new_def = {
		"",
		0,
		0,
		'0',
		"",
		tok->potentional_types[0].type_class,
		tok->potentional_types[0].type_num,
        tok->ns
	};

	strncpy(new_def.file_name, CUR_FILE_NAME, strlen(CUR_FILE_NAME)+1);
	strncpy(new_def.ident, tok_name, strlen(tok_name)+1);

    if (!UNKNOWN_USED_VARS_NUM)
    {
        UNKNOWN_USED_VARS_SIZE = sizeof(new_def);
        UNKNOWN_USED_VARS = malloc(UNKNOWN_USED_VARS_SIZE);
    }
    else
    {
        UNKNOWN_USED_VARS_SIZE += sizeof(new_def);
        UNKNOWN_USED_VARS = realloc(UNKNOWN_USED_VARS, UNKNOWN_USED_VARS_SIZE);
    }

    UNKNOWN_USED_VARS[UNKNOWN_USED_VARS_NUM++] = new_def;

    return 1;
}


int add_global_right_data_item(command *cmd)
{
    long size = sizeof(right_data_cmd_item) * (GLOB_RIGHT_DATA_CMDS_NUM+1);
    if (!GLOB_RIGHT_DATA_CMDS_NUM++)
    {
        GLOB_RIGHT_DATA_CMDS_LIST = malloc(size);
    }
    else
    {
        GLOB_RIGHT_DATA_CMDS_LIST = realloc(GLOB_RIGHT_DATA_CMDS_LIST, size);
    }

    //debug_cmd(cmd);
    right_data_cmd_item *item = &GLOB_RIGHT_DATA_CMDS_LIST[GLOB_RIGHT_DATA_CMDS_NUM-1];
    item->cmd_class = cmd->cmd_class;
    item->cmd_type  = cmd->cmd_type;

    item->left      = cmd->items[0];
    item->right     = cmd->items[cmd->items_num-1].tok.potentional_types[0];
    strncpy(item->inf_file_name, CUR_FILE_NAME, strlen(CUR_FILE_NAME)+1);


    return 1;
}

