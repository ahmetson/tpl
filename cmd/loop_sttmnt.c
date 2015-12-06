/** Dolandyryş komandanyň EGER-ÝA-ÝOGSA diýen görnüşi */

/** Komandanyň iki görnüşi bolýar:
        1) logiki: şert üçin logiki ýa deňeşdirme komandalar ulanylýar.
            Üç sany birlikden ybarat bolmaly:
                Identifikator - "tä" diýen açar sözi
                Ýaý - bir birlik saklap, ol birligem ýa deňeşdirme ýa logiki komanda bolmaly.
                Identifikator - "bolýança" diýen açar sözi
            Özbaşdak manyny gaýtarmaýar.
        2) sanaw: sanawdaky her element boýunça geçýär
            INTÄK DÜZÜLMELI

*/
#include "../cmds.h"
#include "../main/glob.h"
#include "loop_sttmnt.h"
#include "../error.h"
#include "../token/token_types/loop_sttmnt.h"

int CMD_CLASS_LOOP_STTMNT_LOGIC = 0;

int is_cmd_loop_sttmnt(command *cmd)
{
    if (!cmd->items_num)
        return 0;

    command_item *fci = get_cmd_item(cmd->items, 0);
    if (fci->type!=TOKEN_ITEM)
        return 0;   // elmydama birinji birlik - token - achar sozi bolmaly
    if (fci->tok.potentional_types[0].type_class!=TOK_CLASS_LOOP_STTMNT)
        return 0;

    // EGER diyen komanda tanalmaly
    if (fci->tok.potentional_types[0].type_num==TOK_CLASS_LOOP_STTMNT_UNTIL_NUM)
    {
        cmd_loop_sttmnt_until_mod(cmd, 0);
    }


    if (cmd->items_num>=2)
    {
        command_item *sci = get_cmd_item(cmd->items, 1);
        if (sci->type!=PAREN_ITEM)
            return 0;
        int par_item_type = -1, par_item_class = -1, par_item_type_num;
        if (!get_paren_item_type(&sci->paren, &par_item_type, &par_item_class, &par_item_type_num))
            return 0;
        if (!(par_item_type==CMD_ITEM && (par_item_class==CMD_CLASS_CMP || par_item_class==CMD_CLASS_LOGIC )))
            return 0;

        cmd_loop_sttmnt_until_mod(cmd, 1);
    }


    if (cmd->items_num>=3)
    {
        command_item *tci = get_cmd_item(cmd->items, 2);

        if (!(tci->type==TOKEN_ITEM && tci->tok.potentional_types[0].type_class==TOK_CLASS_LOOP_STTMNT &&
               tci->tok.potentional_types[0].type_class==TOK_CLASS_LOOP_STTMNT_UNTIL_CLOSE_NUM))
        {
            cmd_loop_sttmnt_until_mod(cmd, 2);
        }
    }

    // Komandalaryň iň ulysy 3 birlikden ybarat bolmaly
    if (cmd->items_num>3)
    {
        return 0;
    }

}


void cmd_loop_sttmnt_until_mod(command *cmd, int item_num)
{
    if (!item_num)
    {
        cmd->cmd_class = CMD_CLASS_LOOP_STTMNT;
        cmd->cmd_type = CMD_CLASS_LOOP_STTMNT_LOGIC;
        cmd->is_compl = 0;
        cmd->ns = 0;
        cmd->parenthesis = 0;
    }
    else if (item_num==1)
    {

    }
    else if (item_num==2)
    {
        cmd->is_compl = 1;
    }
}


/** Faýla degişli kody C koda ýazýar **/
void cmd_loop_sttmnt_c_code(command *cmd, char **l, int *llen)
{
    command_item *e1 = get_cmd_item(cmd->items,0);
    if (e1->type==TOKEN_ITEM)
    {
        TOK_GET_C_CODE[e1->tok.potentional_types[0].type_class][e1->tok.potentional_types[0].type_num](&e1->tok, l, llen);
    }

    command_item *e2 = get_cmd_item(cmd->items,1);
    paren_get_c_code(&e2->paren, l, llen);

    command_item *e3 = get_cmd_item(cmd->items,2);
    TOK_GET_C_CODE[e3->tok.potentional_types[0].type_class][e3->tok.potentional_types[0].type_num](&e3->tok, l, llen);
}

