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
