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
        if (!add_addtn_file_fns())
            print_err(CODE0_INCLUDES_CANT_ADD, &inf_tok);
    }
}


