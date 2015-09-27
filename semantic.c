/*
 * Semantikany barlayan funksiya
**/
#include <stdio.h>
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
        if (cmd->items[0].type==TOKEN_ITEM && cmd->items[0].tok.type_class==IDENT_TYPE_CLASS)
        {
            //debug_cmd(cmd);
            //debug_GLOB_VAR_DEFS(GLOB_VAR_DEFS);
            //debug_LOCAL_VAR_DEFS(LOCAL_VAR_DEFS);
            token *item = &cmd->items[0].tok; // Gysgaltmak uchin ulanlyar

            if (!is_ident_used(item->potentional_types[0].value))
                print_err(CODE7_LEFT_IDENT_NOT_DEFINED);
        }


        // IKINJI BIRLIGIN BARLANMASY
        //      eyyam yglan edilen ulnin identifikatory bolsa - identifikator hokman on bir yerde yglan edilmeli
        if (cmd->items[2].type==TOKEN_ITEM && cmd->items[2].tok.type_class==IDENT_TYPE_CLASS)
        {
            //debug_cmd(cmd);
            //debug_GLOB_VAR_DEFS(GLOB_VAR_DEFS);
            //debug_LOCAL_VAR_DEFS(LOCAL_VAR_DEFS);
            token *item = &cmd->items[2].tok; // Gysgaltmak uchin ulanlyar
            if (!is_ident_used(item->potentional_types[0].value))
                print_err(CODE7_RIGHT_IDENT_NOT_DEFINED);
        }


    }
    // KOMANDANYN CHEPE BAGLANMA GORNUSHI UCHIN

    CUR_PART = prev_part;
    return 1;
}
