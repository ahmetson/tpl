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
#include "main/glob.h"
#include "dev_debug.h"

// Komandanyn semantikasyny barlayar.
int check_semantics(command *cmd)
{
    // Komandanyn gornushi boyuncha, shol gornush boyuncha semantikany barlayan funksiya chagyrylyar
    return CMD_CHECK_SEMANTICS[cmd->cmd_class][cmd->cmd_type](cmd);
}

void work_with_semantic()
{
    if (GLOB_VAR_DECS_NUMS)
    {
        work_with_glob_var_decs();
    }
    if (GLOB_ARR_DECS_NUMS)
    {
        work_with_glob_arr_decs();
    }
    if (DEC_FUNCS_NUM)
    {
        work_with_glob_fn_decs();
    }

	/// Ýasalan programmanyň baş faýly tanalmaly
	if ( !wcslen(MAIN_FILE_NAME) )
	{
		print_err(CODE0_NOT_FOUND_MAIN_FILE, &inf_tok);
	}

    /** Eger ýasaljak programma birnäçe ýasalan kodlarda algoritmlerden düzülen bolsa,
        Hemme ýasaljak kodlardaky algoritmler ýerine ýetiriler ýaly,
        Baş ýasalan koddaky algoritmleriň soňlarynda, galan ýasalan kodlaryň
        Algoritmlerini saklaýan funksiýalar (ýasalan kodly faýlyň adyna gabat gelýär) çagyrylmaly*/
    if (MAIN_FILE_INCLUDES_NUM)
    {
        if ( !add_addtn_file_fns() )
            print_err(CODE0_INCLUDES_CANT_ADD, &inf_tok);
    }

}



/** Parsing edilen soň semantikany barlaýar.
    Barlanmaly işler:
        1. Parsingden soň komandalar boş bolmaly.
        2. Blok içkiligi bolmaly däl*/
void check_semantics_after_parsing(command *cmd)
{
    /// Eger token bar bolsa, diymek komanda salynmandyr
	if ( cmd->items_num )
    {
        print_err(CODE2_REMAIN_TOKEN, (token *)inf_get_last_token(cmd));
    }

    if (GLOB_BLOCK_INCLUDES)
    {
        /** Blok açylan soň, içindäki komandalar üçin bloklaryň basgançagy ulalýar.
            Emma açylan blogyň ýapylýan bölümi ýok bolany üçin, blok açylýan komandanyň maglumatlaryny
            ulanyja görkezmeli. Görkezmek üçin bolsa, komandanyň maglumatlary içindeliginiň sanawyndan alynýar.

            Alynmak üçin bolsa GLOB_BLOCK_INCLUDES-1 sany ulanylýar. */
        block_inc *bi = get_block_by_inc_num(GLOB_BLOCK_INCLUDES-1);
        inf_tok.inf_file_num = bi->inf_file_num;
        inf_tok.inf_line_num = bi->inf_line_num;
        inf_tok.inf_wchar_t_num = bi->inf_wchar_t_num;
        inf_tok.inf_wchar_t     = bi->inf_wchar_t;

        print_err(CODE2_REQUIRED_END_BLOCK, &inf_tok);
    }
    /// Indiki faýl parsing edilende bloklar üçin maglumatlara gerek bolýar
    if (GLOB_BLOCKS_NUM)
    {
        GLOB_BLOCKS_NUM = 0;
        free(GLOB_BLOCKS);
        GLOB_BLOCKS = NULL;
    }
}

