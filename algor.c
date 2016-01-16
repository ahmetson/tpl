/*
 * Algoritmli fayly yasayan funksiya
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "algor.h"
#include "translator_to_c.h"
#include "semantic.h"
#include "error.h"
#include "dev_debug.h"
#include "main/inf.h"
#include "cmd/def_var.h"
#include "main/user_def_type.h"
#include "fns/fn_helpers.h"


int algor_add_cmd(command add_cmd)
{
    // komanda dogrylygy barlanyar
    check_semantics(&add_cmd);

    ++GLOB_SUBCMDS_NUM;

	GLOB_SUBCMD_ITEMS_LIST = realloc(GLOB_SUBCMD_ITEMS_LIST, sizeof(**GLOB_SUBCMD_ITEMS_LIST)*GLOB_SUBCMDS_NUM);
    if (GLOB_SUBCMD_ITEMS_LIST==NULL)
    {
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

            GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1][i] = *get_cmd_item(add_cmd.items,i);

        }
        add_cmd.items = GLOB_SUBCMDS_NUM-1;

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


void var_def_add(command *cmd, wchar_t glob)
{
    int ident_tok_pos = 2;  // GLOB: @, def_type, ident
    if (!glob)
        ident_tok_pos = 1;  //  LOKAL: def_type, ident

    command_item *ci = get_cmd_item(cmd->items, ident_tok_pos);
    wchar_t *tok_name = ci->tok.potentional_types[0].value;

	// Identifikator eyyam yglan edilen eken.
	if (glob)
    {
        if (is_ident_used(&ci->tok, 1))
        {
            CUR_PART = 4;
            print_err(CODE4_VARS_IDENT_USED, &ci->tok);
        }
    }
    else
    {
        if (is_ident_used(&ci->tok, 0))
        {
            CUR_PART = 4;
            print_err(CODE4_VARS_IDENT_USED, &ci->tok);
        }
    }
    command_item *prevci = get_cmd_item(cmd->items, ident_tok_pos-1);
	glob_ident new_def = {
	    prevci->tok.potentional_types[0].type_class,
		prevci->tok.potentional_types[0].type_num,
		L"",
		prevci->tok.inf_file_num,
        prevci->tok.inf_wchar_t_num,
		prevci->tok.inf_wchar_t,
		prevci->tok.inf_line_num
	};

	wcsncpy( new_def.name, tok_name, wcslen( tok_name )+1 );

    // Taze gosuljak komandanyn gowrumi, kuchadaky eyelenen gowrume goshulyar
    if (glob)
    {
        ++GLOB_VAR_DEFS_NUMS;
        GLOB_VAR_DEFS = realloc(GLOB_VAR_DEFS, sizeof(*GLOB_VAR_DEFS)*GLOB_VAR_DEFS_NUMS);
        GLOB_VAR_DEFS[GLOB_VAR_DEFS_NUMS-1] = new_def;
    }
    else
    {
        ++LOCAL_VAR_DEFS_NUMS;
        LOCAL_VAR_DEFS = realloc(LOCAL_VAR_DEFS, sizeof(*LOCAL_VAR_DEFS)*LOCAL_VAR_DEFS_NUMS);
        LOCAL_VAR_DEFS[LOCAL_VAR_DEFS_NUMS-1] = new_def;
    }
}


void arr_def_add(command *cmd, wchar_t glob)
{
    int ident_tok_pos = cmd->items_num-1;  // Identifikator - bu komandadaky iň soňky element

    command_item *ci = get_cmd_item(cmd->items, ident_tok_pos);
    wchar_t *tok_name = ci->tok.potentional_types[0].value;

	// Identifikator eyyam yglan edilen eken.
	if (glob==1)
    {
        if (is_ident_used(&ci->tok, 1))
        {
            CUR_PART = 4;
            print_err(CODE4_VARS_IDENT_USED, &ci->tok);
        }
    }
    else if (!glob)
    {
        if (is_ident_used(&ci->tok, 0))
        {
            CUR_PART = 4;
            print_err(CODE4_VARS_IDENT_USED, &ci->tok);
        }
    }
    else if (glob==-1)
    {
        if (is_ident_used(&ci->tok, 2))
        {
            printf("IDENT 5");
            CUR_PART = 4;
            print_err(CODE4_VARS_IDENT_USED, &ci->tok);
        }
    }
    command_item *prevci = get_cmd_item(cmd->items, ident_tok_pos-1);
    command_item *yay_ci = get_cmd_item(cmd->items, ident_tok_pos-2);

    array_item ai = {
        prevci->tok.inf_file_num,
        prevci->tok.inf_wchar_t_num,
        prevci->tok.inf_wchar_t,
        prevci->tok.inf_line_num,
        L"",
        yay_ci->paren.elems_num,
        prevci->tok.potentional_types[0].type_class,
		prevci->tok.potentional_types[0].type_num
    };

	wcsncpy( ai.ident, tok_name, wcslen( tok_name )+1 );

    // Taze gosuljak komandanyn gowrumi, kuchadaky eyelenen gowrume goshulyar
    if (glob==1)
    {
        ++GLOB_ARR_DEFS_NUMS;
        GLOB_ARR_DEFS = realloc(GLOB_VAR_DEFS, sizeof(*GLOB_ARR_DEFS)*GLOB_ARR_DEFS_NUMS);
        GLOB_ARR_DEFS[GLOB_ARR_DEFS_NUMS-1] = ai;

        GLOB_ARR_DEFS_ITEMS = realloc(GLOB_ARR_DEFS_ITEMS, sizeof(*GLOB_ARR_DEFS_ITEMS)*GLOB_ARR_DEFS_NUMS);
        GLOB_ARR_DEFS_ITEMS[GLOB_ARR_DEFS_NUMS-1] = NULL;

        add_to_last_glob_arr_items(cmd);
    }
    else if (!glob)
    {
        ++LOCAL_ARR_DEFS_NUMS;
        LOCAL_ARR_DEFS = realloc(LOCAL_ARR_DEFS, sizeof(*LOCAL_ARR_DEFS)*LOCAL_ARR_DEFS_NUMS);
        LOCAL_ARR_DEFS[LOCAL_ARR_DEFS_NUMS-1] = ai;

        LOCAL_ARR_DEFS_ITEMS = realloc(LOCAL_ARR_DEFS_ITEMS, sizeof(*LOCAL_ARR_DEFS_ITEMS)*LOCAL_ARR_DEFS_NUMS);
        LOCAL_ARR_DEFS_ITEMS[LOCAL_ARR_DEFS_NUMS-1] = NULL;

        add_to_last_loc_arr_items(cmd);
    }
    else if (glob==-1)
    {
        ++TMP_USER_DEF_TYPES_ARRS_NUMS;
        TMP_USER_DEF_TYPES_ARRS = realloc(TMP_USER_DEF_TYPES_ARRS, sizeof(*TMP_USER_DEF_TYPES_ARRS)*TMP_USER_DEF_TYPES_ARRS_NUMS);
        TMP_USER_DEF_TYPES_ARRS[TMP_USER_DEF_TYPES_ARRS_NUMS-1] = ai;

        TMP_USER_DEF_TYPES_ARRS_ITEMS = realloc(TMP_USER_DEF_TYPES_ARRS_ITEMS, sizeof(*TMP_USER_DEF_TYPES_ARRS_ITEMS)*TMP_USER_DEF_TYPES_ARRS_NUMS);
        TMP_USER_DEF_TYPES_ARRS_ITEMS[TMP_USER_DEF_TYPES_ARRS_NUMS-1] = NULL;

        add_to_last_tmp_arr_items(cmd);
    }
}



// Lokal fayllar uchin
wchar_t loc_head_file[] = L".h";
wchar_t loc_source_file[] = L".c";

int is_glob_def_var_in_cur()
{
	int i;
	for(i=0; i<GLOB_VAR_DEFS_NUMS; ++i)
	{
		if (GLOB_VAR_DEFS[i].inf_file_num==CUR_FILE_NUM-1)
			return 1;
	}
	return 0;
}

int is_glob_def_arr_in_cur()
{
	int i;
	for(i=0; i<GLOB_ARR_DEFS_NUMS; ++i)
	{
		if (GLOB_ARR_DEFS[i].inf_file_num==CUR_FILE_NUM-1)
			return 1;
	}
	return 0;
}

int is_glob_arr_def_exist(wchar_t *name)
{
    int i, len;
    for (i=0; i<GLOB_ARR_DEFS_NUMS; ++i)
    {
        len = wcslen(name)>wcslen(GLOB_ARR_DEFS[i].ident)?wcslen(name):wcslen(GLOB_ARR_DEFS[i].ident);
        if (wcsncmp(GLOB_ARR_DEFS[i].ident, name, len)==0)
        {
            return 1;
        }
    }
    return 0;
}

int is_glob_var_def_exist(wchar_t *name)
{
    int i, len;
    for (i=0; i<GLOB_VAR_DEFS_NUMS; ++i)
    {
        len = wcslen(name)>wcslen(GLOB_VAR_DEFS[i].name)?wcslen(name):wcslen(GLOB_VAR_DEFS[i].name);
        if (wcsncmp(GLOB_VAR_DEFS[i].name, name, len)==0)
        {
            return 1;
        }
    }
    return 0;
}


int is_local_var_def_exist(wchar_t *name)
{
    int i, len = wcslen(name);
    for (i=0; i<LOCAL_VAR_DEFS_NUMS; ++i)
    {
        if (wcslen(LOCAL_VAR_DEFS[i].name)==len &&
            wcsncmp(LOCAL_VAR_DEFS[i].name, name, len)==0)
        {
            return 1;
        }
    }
    return 0;
}

int is_local_arr_def_exist(wchar_t *name)
{
    int i, len;
    for (i=0; i<LOCAL_ARR_DEFS_NUMS; ++i)
    {
        len = wcslen(name)>wcslen(LOCAL_ARR_DEFS[i].ident)?wcslen(name):wcslen(LOCAL_ARR_DEFS[i].ident);
        if (wcsncmp(LOCAL_ARR_DEFS[i].ident, name, len)==0)
        {
            return 1;
        }
    }
    return 0;
}

int is_var_def_exist(wchar_t *ident)
{
    if (is_glob_var_def_exist(ident) ||
        is_glob_var_dec_exist(ident) ||
        is_local_var_def_exist(ident))
        return 1;
}

int is_arr_def_exist(wchar_t *ident)
{
    if (is_glob_arr_def_exist(ident) ||
        is_glob_arr_dec_exist(ident) ||
        is_local_arr_def_exist(ident))
        return 1;
}

/** Ulninin ady boýuncha onun on yglan edilendigini barlayar.
**/
int is_ident_used(token *t, wchar_t except_code)
{
    wchar_t *ident = t->potentional_types[0].value;

    /// Ähli bolup biljek identifikatorlaryň bölümlerinde barlamaly.
    ///     Ülňileriň adymy, funksiýalaryň adymy, açar sözlerimi we ş.m.
    if (!except_code)
    {
        int res = is_var_def_exist(ident);
        if (!res)
            res = is_arr_def_exist(ident);
        if (!res)
            res = is_utype_ident(ident);
        if (!res)
            res = is_fn_name_used(ident);
        return res;
    }
    /// Global ülňileriň maglumatlary beýan edilen sanawda identifikator gözlenenok.
    else if (except_code==1)
    {
        return is_local_var_def_exist(ident) || is_glob_var_def_exist(ident)
            || is_local_arr_def_exist(ident) || is_glob_arr_def_exist(ident)
            || is_utype_ident(ident) || is_fn_name_used(ident);
    }
    /// Diňe häzirki wagtlaýyn sanawyň birliklerini saklaýan sanawdaky birlikleriň atlarynda görülýär
    else if(except_code==2)
    {
        return is_tmp_triangle_block_item_ident(ident);
    }
    /// Diňe global ülňileriň sanawynda gözlenmeli
    else if (except_code==3)
    {
        int answer = is_glob_var_def_exist(ident) ||
        is_glob_var_dec_exist(ident) ||
        is_glob_arr_def_exist(ident) ||
        is_glob_arr_dec_exist(ident) ||
        is_fn_name_used(ident);
        return answer;
    }
    return 0;
}


glob_ident *glob_vars_def_get_by_name(wchar_t *name)
{
    int i, len;
    glob_ident *ret;
    for (i=0; i<GLOB_VAR_DEFS_NUMS; ++i)
    {
        // Lokal ülňiler üçin, funksiýanyň ady gabat gelmeli.
        len = wcslen(name)>wcslen(GLOB_VAR_DEFS[i].name)?wcslen(name):wcslen(GLOB_VAR_DEFS[i].name);
        if (wcsncmp(GLOB_VAR_DEFS[i].name, name, len)==0)
        {
            ret = &GLOB_VAR_DEFS[i];
            break;
        }
    }
    return ret;
}


array_item *glob_arrs_def_get_by_name(wchar_t *name)
{
    int i, len;
    array_item *ret;
    for (i=0; i<GLOB_ARR_DEFS_NUMS; ++i)
    {
        // Lokal ülňiler üçin, funksiýanyň ady gabat gelmeli.
        len = wcslen(name)>wcslen(GLOB_ARR_DEFS[i].ident)?wcslen(name):wcslen(GLOB_ARR_DEFS[i].ident);
        if (wcsncmp(GLOB_ARR_DEFS[i].ident, name, len)==0)
        {
            ret = &GLOB_ARR_DEFS[i];
            break;
        }
    }
    return ret;
}


/// Bloklar ýaýlaryň içinde bolup bolanok. Şonuň
/// üçin diňe komandalary parsing edilýän funksiýa-da çagyrylýar.
void work_with_blocks(command *cmd)
{
    //if ((cmd.cmd_class==CTRL_STATEMENT))
    if (cmd->is_compl)
    {
        /// Eger komanda blogy ýapýan bolsa
        if (is_close_block_cmd(cmd))
        //if ((cmd.cmd_class==CMD_CLASS_CTRL_STATEMENT) || (cmd.cmd_class==CMD_CLASS_CLOSE_BLOCK && cmd.cmd_type==CMD_CLOSE_BLOCK_TYPE_NUM))
        {
            close_block(cmd);
        }

        /// Eger komanda blogy açýan bolsa
        if (is_open_block_cmd(cmd))
        //if (cmd.cmd_class==CMD_CLASS_CTRL_STATEMENT)
        {
            open_block(cmd);
        }

        if (is_close_block_cmd(cmd) || is_open_block_cmd(cmd))
        {
            work_with_cmd();
            init_cmd(cmd, 0);
        }

    }
}


void open_block(command *cmd)
{
    ++GLOB_BLOCKS_NUM;
    GLOB_BLOCKS = realloc(GLOB_BLOCKS, sizeof(*GLOB_BLOCKS)*GLOB_BLOCKS_NUM);

    block_inc bi;
    bi.type_class = cmd->cmd_class;
    bi.type_num = cmd->cmd_type;
    bi.inc_num  = GLOB_BLOCK_INCLUDES;
    command_item *ci = get_cmd_item(cmd->items, 0);
    token *inf_tok;
    if (ci->type==TOKEN_ITEM)
    {
        inf_tok = &ci->tok;
    }
    else if (ci->type==CMD_ITEM)
    {
        inf_tok = inf_get_last_token(&ci->cmd);
    }
    else if (ci->type==PAREN_ITEM)
    {
        inf_tok = inf_get_parens_last_token(&ci->paren);
    }
    bi.inf_file_num = inf_tok->inf_file_num;   // Blok açýan bloklaryň birinji tokeni elmydama token birligi bolýar
    bi.inf_line_num = inf_tok->inf_line_num;
    bi.inf_wchar_t_num = inf_tok->inf_wchar_t_num;
    bi.inf_wchar_t = inf_tok->inf_wchar_t;

    GLOB_BLOCKS[GLOB_BLOCKS_NUM-1] = bi;

    // Içindäki bloklaryň sany ulalmaly.
    ++GLOB_BLOCK_INCLUDES;
}


void close_block()
{
    --GLOB_BLOCK_INCLUDES;
}

block_inc *get_block_by_inc_num(int inc_num)
{
    int i;
    for (i=0; i<GLOB_BLOCKS_NUM; ++i)
    {
        if (GLOB_BLOCKS[i].inc_num==inc_num)
            return &GLOB_BLOCKS[i];
    }

}

int is_tmp_triangle_block_item_ident(wchar_t *ident)
{
    int i, len = wcslen(ident);
    for (i=0; i<TMP_USER_DEF_TYPES_NUM; ++i)
    {
        if (wcslen(TMP_USER_DEF_TYPE_ITEMS[i].ident)==len &&
            wcsncmp(TMP_USER_DEF_TYPE_ITEMS[i].ident, ident, len)==0)
        {
            return 1;
        }
    }
    return 0;
}

