/** Ähli bloklara ýa olaryň aglabasy bilen bagly komandalar:
    1) Blogy ýapýan komanda:
        Bir birligi bar:
            BLOK klasyna degişli, ÝAPÝAN görnüşli token.
        Blok özbaşdak many aňlatmaýar.
        Semantikasy:
            Eger içine salynan bloklaryň sany 0-dan kiçi bolsa,
            "Açýan blogy" talap edýän ýalňyşlygy çykardýar.
        Blok ýapýan komandalaryň sanawyna girýär.
*/

#include "block.h"
#include "../cmds.h"
#include "../main/glob.h"
#include "../main/inf.h"
#include "../error.h"
#include "../token/token_types/block.h"

int CMD_CLASS_BLOCK_CLOSE = 0;

int is_cmd_block(command *cmd)
{

    if (cmd->items_num!=1)
        return 0;

    command_item *fci = get_cmd_item(cmd->items, 0);

    if (fci->type==TOKEN_ITEM && (fci->tok.potentional_types[0].type_class==TOK_CLASS_BLOCK &&
         fci->tok.potentional_types[0].type_num==TOK_CLASS_BLOCK_CLOSE_NUM))
    {
        cmd_block_close_mod(cmd, 1);
        return 1;
    }

    return 0;
}


void cmd_block_close_mod(command *cmd, int item_num)
{
    cmd->cmd_class = CMD_CLASS_BLOCK;
    cmd->cmd_type = CMD_CLASS_BLOCK_CLOSE;
    cmd->is_compl = 1;
    cmd->ns = 0;
    cmd->parenthesis = 0;
    cmd->is_compl = 1;
}

int semantic_cmd_block(command *cmd)
{
    if (GLOB_BLOCK_INCLUDES<0)
    {
        CUR_PART = 7;
        print_err(CODE7_NEED_OPEN_BLOCK_CMD, (token *)inf_get_last_token(cmd));
    }
    return 0;
}


/** Faýla degişli kody C koda ýazýar **/
void cmd_block_c_code(command *cmd, wchar_t **l, int *llen)
{
    command_item *e1 = get_cmd_item(cmd->items,0);
    TOK_GET_C_CODE[e1->tok.potentional_types[0].type_class][e1->tok.potentional_types[0].type_num](&e1->tok, l, llen);
}

