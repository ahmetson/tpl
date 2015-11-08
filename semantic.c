/*
 * Semantikany barlayan funksiya
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tpl.h"
#include "cmds.h"
#include "cmd/fn_call.h"
#include "cmd/assign.h"
#include "semantic.h"
#include "error.h"
#include "main/inf.h"
#include "dev_debug.h"

// Komandanyn semantikasyny barlayar.
int check_semantics(command *cmd)
{
    // Komandanyn gornushi boyuncha, shol gornush boyuncha semantikany barlayan funksiya chagyrylyar
    return CMD_CHECK_SEMANTICS[cmd->cmd_class][cmd->cmd_type](cmd);
}

/**
 * Ulanylýan global ülňileriň hataryna goşýar
**/
int unknown_used_var_add(token *tok, char *tok_name)
{
    var_def_item new_def = {
		"",
		tok->inf_file_num,
		tok->inf_line_num,
		tok->inf_char_num,
		tok->inf_char,
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

int add_global_both_ident_item(command *cmd)
{
    long size = sizeof(both_ident_cmd_item) * (GLOB_BOTH_IDENT_CMDS_NUM+1);
    if (!GLOB_BOTH_IDENT_CMDS_NUM++)
    {
        GLOB_BOTH_IDENT_CMDS_LIST = malloc(size);
    }
    else
    {
        GLOB_BOTH_IDENT_CMDS_LIST = realloc(GLOB_BOTH_IDENT_CMDS_LIST, size);
    }

    //debug_cmd(cmd);
    both_ident_cmd_item *item = &GLOB_BOTH_IDENT_CMDS_LIST[GLOB_BOTH_IDENT_CMDS_NUM-1];
    item->cmd_class = cmd->cmd_class;
    item->cmd_type  = cmd->cmd_type;

    item->left      = cmd->items[0];
    item->right     = cmd->items[cmd->items_num-1];
    strncpy(item->inf_file_name, CUR_FILE_NAME, strlen(CUR_FILE_NAME)+1);

    return 1;
}

void work_with_unknown_fns()
{
    int i;
    for(i=0; i<UNKNOWN_CALLED_FNS_NUM; ++i)
    {
        if (!semantic_cmd_fn_call(&UNKNOWN_CALLED_FNS[i]))
        {
            CUR_PART = 7;
            print_err(CODE7_UNKNOWN_FNS_CALLED, &UNKNOWN_CALLED_FNS[i].items[1].tok);
        }
    }
}
void work_with_unknown_tokens()
{
    int i, x, y;
    for(i=0; i<UNKNOWN_TOKENS_NUM; ++i)
    {
        if(!TOK_RETURN_TYPE[UNKNOWN_TOKENS[i].tok->potentional_types[0].type_class]
                            [UNKNOWN_TOKENS[i].tok->potentional_types[0].type_num](UNKNOWN_TOKENS[i].tok, &x, &y))
        {
            CUR_PART = 7;
            print_err(CODE7_UNKNOWN_DATA_USED, UNKNOWN_TOKENS[i].tok);
        }
    }
}
void work_with_unknown_cmds()
{
    int i, x, y;
    for(i=0; i<UNKNOWN_CMDS_NUM; ++i)
    {
        if(!CMD_RETURN_TYPE[UNKNOWN_CMDS[i].cmd->cmd_class][UNKNOWN_CMDS[i].cmd->cmd_type](UNKNOWN_CMDS[i].cmd, &x, &y))
        {
            CUR_PART = 7;
            print_err(CODE7_UNKNOWN_DATA_USED, (token *)inf_get_last_token(UNKNOWN_CMDS[i].cmd));
        }
    }
}
void work_with_unknown_parens()
{
    int i, x, y;
    for(i=0; i<UNKNOWN_PARENS_NUM; ++i)
    {
        if(!PAREN_RETURN_TYPE[UNKNOWN_PARENS[i].paren->type](UNKNOWN_PARENS[i].paren, &x, &y))
        {
            CUR_PART = 7;
            print_err(CODE7_UNKNOWN_DATA_USED, &inf_tok);
        }
    }
}


/** ŞERT #1: Eger 'A' ülňä 'B' ülňi baglansa, ýa 'B' ülňä 'A' ülňi baglansa
 *           "A<->B baglanyşygy bolup bilenok" diýen ýalňyşlygy görkezmeli.

    ++ Ülňä baglanma bolan komandanyň işlerinde

    1) Eger ikinji sag tarapdaky maglumat birligi identifikator bolsa
        a) Iki sany identifikatory saklaýan sanawa goşulýar (Global ülňi).

    ++ Parser işläp bolan soň
    1) Eger iki sany identifikator saklap, deňeşdirmeli bolsalar
        a) Iki identifikatory deňeşdirýän funksiýa çagyrylýar.

    ++ Iki sany identifikatory deňeşdirýän funksiýanyň beýany
    1) Her iki sany identifikatory
        a) Birinji identifikator bellenilýär.
        b) Her iki sany identifikator boýunça
            I) Ikinji sanawdan ikinji identifikator bellenilýär
           II) Eger birinji sanawyň birinji identifikatory we ikinji sanawyň ikinji identifikatory deň bolsa
                *) (""A<->B baglanyşygy bolup bilenok"") diýen ýalňyşlyk çap edilýär
**/
/*
// 'A' -> 'B', 'B' -> 'A' sanawa täzesi goşulýar
void compare_idents_add_new(char *l, token tok)
{
    ++COMPARE_IDENTS_NUM;
    COMPARE_IDENTS = realloc(COMPARE_IDENTS, sizeof(*COMPARE_IDENTS)*COMPARE_IDENTS_NUM);

    compare_ident ci;
    strncpy(ci.l, l, strlen(l)+1);
    ci.r = tok;

    COMPARE_IDENTS[COMPARE_IDENTS_NUM-1] = ci;
}


// 'A' -> 'B', 'B' -> 'A' sanawda, 'A' -> 'B', 'B' -> 'A' däldigini barlaýar
void check_a_to_b_b_to_b()
{
    int j, i, len;
    for (i=0; i<COMPARE_IDENTS_NUM; ++i)
    {
        for(j=0; j<COMPARE_IDENTS_NUM; ++j)
        {
            len = strlen(COMPARE_IDENTS[i].l)>strlen(COMPARE_IDENTS[j].r.potentional_types[0].value)?
                strlen(COMPARE_IDENTS[i].l) : strlen(COMPARE_IDENTS[j].r.potentional_types[0].value);
            if (strncmp(COMPARE_IDENTS[i].l, COMPARE_IDENTS[j].r.potentional_types[0].value, len)==0)
            {
                CUR_PART =  7;
                print_err(CODE7_A_TO_B_B_TO_A, &COMPARE_IDENTS[j].r);
            }
        }
    }
}
*/
