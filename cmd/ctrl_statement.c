#include "../cmds.h"
#include "../main/glob.h"
#include "../main/inf.h"
#include "ctrl_statement.h"
#include "../error.h"
#include "../token/token_types/ctrl_sttmnt.h"
/** Dolandyryş komandanyň EGER-ÝA-ÝOGSA diýen görnüşi */

/** Komandanyň üç görnüşi bolýar:
        1) if:
            Üç sany birlikden ybarat bolmaly:
                Identifikator - "eger" diýen açar sözi
                Ýaý - bir birlik saklap, ol birligem ýa deňeşdirme ýa logiki komanda bolmaly.
                Identifikator - "bolsa" diýen açar sözi
            Özbaşdak manyny gaýtarmaýar.
        2) elif
            Üç sany birlikden ybarat bolmaly:
                Identifikator - "ýa" diýen açar sözi
                Ýaý - bir birlik saklap, ol birligem ýa deňeşdirme ýa logiki komanda bolmaly.
                Identifikator - "bolsa" diýen açar sözi
            Özbaşdak manyny gaýtarmaýar
        3) else
            Bir birlikden ybarat bolmaly:
                Identifikator - "ýogsa" diýen açar sözi bolmaly.

    ŞERT #8: Komandalaryň işleýşi.

*/


int CMD_CLASS_CTRL_STTMNT_IF = 0;
int CMD_CLASS_CTRL_STTMNT_IFELSE = 1;
int CMD_CLASS_CTRL_STTMNT_ELSE = 2;

int is_cmd_ctrl_sttmnt(command *cmd)
{
    if (!cmd->items_num)
        return 0;

    command_item *fci = get_cmd_item(cmd->items, 0);
    if (fci->type!=TOKEN_ITEM)
        return 0;   // elmydama birinji birlik - token - awchar_t sozi bolmaly
    if (fci->tok.potentional_types[0].type_class!=TOK_CLASS_CTRL_STTMNT)
        return 0;

    // EGER diyen komanda tanalmaly
    if (fci->tok.potentional_types[0].type_num==TOK_CLASS_CTRL_STTMNT_IF_NUM)
    {
        cmd_ctrl_sttmnt_if_mod(cmd, 0);
    }
    // Ya diyen komanda tanalmaly
    else if (fci->tok.potentional_types[0].type_num==TOK_CLASS_CTRL_STTMNT_IFELSE_NUM)
    {
        cmd_ctrl_sttmnt_ifelse_mod(cmd, 0);
    }
    // yogsa diyen komanda tanalmaly
    else if (fci->tok.potentional_types[0].type_num==TOK_CLASS_CTRL_STTMNT_ELSE_NUM)
    {
        cmd_ctrl_sttmnt_else_mod(cmd, 0);
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

        if (cmd->cmd_type==CMD_CLASS_CTRL_STTMNT_IF)
        {
            cmd_ctrl_sttmnt_if_mod(cmd, 1);
        }
        else if (cmd->cmd_type==CMD_CLASS_CTRL_STTMNT_IFELSE)
        {
            cmd_ctrl_sttmnt_ifelse_mod(cmd, 1);
        }
    }

    if (cmd->items_num>=3)
    {
        command_item *tci = get_cmd_item(cmd->items, 2);

        if (!(tci->type==TOKEN_ITEM && tci->tok.potentional_types[0].type_class==TOK_CLASS_CTRL_STTMNT &&
               tci->tok.potentional_types[0].type_class==TOK_CLASS_CTRL_STTMNT_CLOSE_BLOCK_NUM))
        {
            if (cmd->cmd_type==CMD_CLASS_CTRL_STTMNT_IF)
            {
                cmd_ctrl_sttmnt_if_mod(cmd, 2);
            }
            else if (cmd->cmd_type==CMD_CLASS_CTRL_STTMNT_IFELSE)
            {
                cmd_ctrl_sttmnt_ifelse_mod(cmd, 2);
            }
        }
    }

    // Komandalaryň iň ulysy 3 birlikden ybarat bolmaly
    if (cmd->items_num>3)
    {
        return 0;
    }

}


void cmd_ctrl_sttmnt_if_mod(command *cmd, int item_num)
{
    if (!item_num)
    {
        cmd->cmd_class = CMD_CLASS_CTRL_STTMNT;
        cmd->cmd_type = CMD_CLASS_CTRL_STTMNT_IF;
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

void cmd_ctrl_sttmnt_ifelse_mod(command *cmd, int item_num)
{
    if (!item_num)
    {
        cmd->cmd_class = CMD_CLASS_CTRL_STTMNT;
        cmd->cmd_type = CMD_CLASS_CTRL_STTMNT_IFELSE;
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


void cmd_ctrl_sttmnt_else_mod(command *cmd, int item_num)
{
    if (!item_num)
    {
        cmd->cmd_class = CMD_CLASS_CTRL_STTMNT;
        cmd->cmd_type = CMD_CLASS_CTRL_STTMNT_ELSE;
        cmd->is_compl = 1;
        cmd->ns = 0;
        cmd->parenthesis = 0;
    }
}


int semantic_cmd_ctrl_sttmnt(command *cmd)
{
    if (GLOB_BLOCKS_NUM>=2)
    {
        block_inc *blast = &GLOB_BLOCKS[GLOB_BLOCKS_NUM-2];
        block_inc *last = &GLOB_BLOCKS[GLOB_BLOCKS_NUM-1];

        if (blast->inc_num==last->inc_num && blast->type_class==CMD_CLASS_CTRL_STTMNT &&
            (blast->type_num==CMD_CLASS_CTRL_STTMNT_IF || blast->type_num==CMD_CLASS_CTRL_STTMNT_IFELSE))
                return 1;
    }
    /// Başlangyç 'Eger' diýen komanda ýok

    CUR_PART = 7;
    print_err(CODE7_NEED_IF_OR_IFELSE_CMD, (token *)inf_get_last_token(cmd));

    return 0;
}


/** Faýla degişli kody C koda ýazýar **/
void cmd_ctrl_sttmnt_c_code(command *cmd, wchar_t **l, int *llen)
{
    command_item *e1 = get_cmd_item(cmd->items,0);
    if (e1->type==TOKEN_ITEM)
    {
        TOK_GET_C_CODE[e1->tok.potentional_types[0].type_class][e1->tok.potentional_types[0].type_num](&e1->tok, l, llen);
    }

    if (cmd->cmd_type!=CMD_CLASS_CTRL_STTMNT_ELSE)
    {
        command_item *e2 = get_cmd_item(cmd->items,1);
        paren_get_c_code(&e2->paren, l, llen);

        command_item *e3 = get_cmd_item(cmd->items,2);
        TOK_GET_C_CODE[e3->tok.potentional_types[0].type_class][e3->tok.potentional_types[0].type_num](&e3->tok, l, llen);
    }
}

