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

