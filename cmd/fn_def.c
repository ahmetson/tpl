#include <stdlib.h>
#include <string.h>
#include "fn_def.h"
#include "fn_call.h"
#include "../paren/types.h"
#include "../main/glob.h"
#include "../error.h"
/** Iki sany komandanyň görnüşi bar:
    Funksiýany yglan etmek, (ýaý, ident., tip ýa hiç_zat, sag tarapa baglanma)
    Funksiýany beýan etmek  (ýaý, ident., tip ýa hiç_zat)
*/


int is_cmd_fn_def(command *cmd)
{
    if (!cmd->items_num)
        return;

    command_item *fci = get_cmd_item(cmd->items, 0);
    if (fci->type==CMD_ITEM && (fci->cmd.cmd_class==CMD_CLASS_FN_CALL &&
                                fci->cmd.cmd_type ==FN_CALL_TYPE_NUM))
    {
        mod_cmd_by_fn_def(cmd, 0);
    }
    else
    {
        return 0;
    }

    if (cmd->items_num>=2)
    {
        command_item *sci = get_cmd_item(cmd->items, 1);
        if (sci->type==TOKEN_ITEM && (sci->tok.type_class==TOK_CLASS_DEF_TYPE ||
                                      sci->tok.type_class==TOK_CLASS_UTYPE_CON ||
                                      sci->tok.type_class==TOK_CLASS_VOID))
        {
            mod_cmd_by_fn_def(cmd, 1);
        }
        else
        {
            return 0;
        }

        if (cmd->items_num>=3)
        {
            command_item *tci = get_cmd_item(cmd->items, 2);
            if (tci->type==TOKEN_ITEM && (tci->tok.type_class==TOK_CLASS_ASSIGN &&
                                           tci->tok.potentional_types[0].type_num==RIGHT_ASSIGN_TOK_NUM))
            {
                mod_cmd_by_fn_def(cmd, 2);
            }
            else
            {
                return 0;
            }

            if (cmd->items_num>3)
            {
                return 0;
            }
        }
    }
    return 1;
}


void mod_cmd_by_fn_def(command *cmd, int item_num)
{
    if (item_num==0)
    {
        cmd->is_compl=0;
        cmd->cmd_class=CMD_CLASS_FN_DEF;
    }
    else if (item_num==1)
    {
        cmd->is_compl=1;
        cmd->cmd_type=CMD_FN_DEC_TYPE;
    }
    else if (item_num==2)
    {
        cmd->cmd_type=CMD_FN_DEF_TYPE;
    }
}


/** IÇINI SOŇ GOÝARYS*/
int semantic_cmd_fn_def(command *cmd)
{
    command_item *fci = get_cmd_item(cmd->items, 0);
    command_item *ident_item = get_cmd_item(fci->cmd.items, 1);
    token        *ident_tok = &ident_item->tok;
    char         *ident = ident_tok->potentional_types[0].value;
    if (is_ident_used(ident_tok, 0))
    {
        CUR_PART = 4;
        printf("IDENT 12");
        print_err(CODE4_VARS_IDENT_USED, (token *)inf_get_last_token(cmd));
    }
}

/** IÇINI SOŇ GOÝARYS*/
int semantic_cmd_fn_dec(command *cmd)
{
    glob_fns_decl_add(cmd);
}


/** Faýla degişli kody C koda ýazýar **/
void cmd_fn_def_c_code(command *cmd, char **l, int *llen)
{
    command_item *e1 = get_cmd_item(cmd->items,0);
    paren_get_c_code(&e1->paren, l, llen);

    command_item *e2 = get_cmd_item(cmd->items,1);
    int e_class = e2->tok.potentional_types[0].type_class;
    int e_type = e2->tok.potentional_types[0].type_num;
    TOK_GET_C_CODE[e_class][e_type](&e2->tok, l, llen);

    command_item *e3 = get_cmd_item(cmd->items,2);
    e_class = e3->tok.potentional_types[0].type_class;
    e_type = e3->tok.potentional_types[0].type_num;
    TOK_GET_C_CODE[e_class][e_type](&e3->tok, l, llen);

    command_item *e4 = get_cmd_item(cmd->items,3);
    e_class = e4->tok.potentional_types[0].type_class;
    e_type = e4->tok.potentional_types[0].type_num;
    TOK_GET_C_CODE[e_class][e_type](&e4->tok, l, llen);
}

/** Faýla degişli kody C koda ýazýar **/
void cmd_fn_dec_c_code(command *cmd, char **l, int *llen)
{

    command_item *e3 = get_cmd_item(cmd->items,1);
    int e_class = e3->tok.type_class;
    int e_type = e3->tok.potentional_types[0].type_num;

    get_type_c_code(e_class, e_type, l, llen);
    *llen += strlen(" ");
    *l = realloc(*l, *llen);
    strncat(*l, " ", strlen(" "));

    command_item *fn_call = get_cmd_item(cmd->items,0);

    command_item *e2 = get_cmd_item(fn_call->cmd.items,1);
    e_class = e2->tok.potentional_types[0].type_class;
    e_type = e2->tok.potentional_types[0].type_num;
    TOK_GET_C_CODE[e_class][e_type](&e2->tok, l, llen);

    command_item *e1 = get_cmd_item(fn_call->cmd.items, 0);
    paren_get_c_code(&e1->paren, l, llen);
}

