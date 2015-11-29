/** Global ülňini çagyrýan funksiýa
**/
#include <string.h>
#include <stdlib.h>
#include "../main/glob.h"
#include "../error.h"
#include "call_glob_var.h"
#include "../cmds.h"

int CMD_CALL_GLOB_VAR_TYPE_NUM = 0;


int is_cmd_call_glob_var(command *cmd)
{
    if (cmd->items_num>CMD_MAX_ITEMS[CMD_CLASS_CALL_GLOB_VAR][CMD_CALL_GLOB_VAR_TYPE_NUM] || !cmd->items_num)
	{
		// Komandany saygaryp bolmady
		return 0;
	}

    /** Birinji birlik token bolup, global ülňiniň başlangyjy bolmaly**/
    command_item *fci = get_cmd_item(cmd->items,0);
    if (fci->type!=TOKEN_ITEM)
        return 0;
    token *t1 = &fci->tok;
    if (t1->potentional_types[0].type_class!=TOK_CLASS_GLOB)
        return 0;
    else
        cmd_call_glob_var_mod(cmd, 0);

    /** Ikinji birlik token bolup, ülňiniň identifikatory bolmaly**/
    if (cmd->items_num>1)
    {
        command_item *sci = get_cmd_item(cmd->items,1);
        if (sci->type==TOKEN_ITEM)
        {
            token *t2 = &sci->tok;
            if (t2->potentional_types[0].type_class==TOK_CLASS_IDENT)
                cmd_call_glob_var_mod(cmd, 1);
            else
                return 0;
        }
        else
            return 0;
    }
    return 1;
}


void cmd_call_glob_var_mod(command *cmd, int tok_num)
{
    if (!tok_num)
    {
        cmd->is_compl = 0;
        cmd->cmd_class = CMD_CLASS_CALL_GLOB_VAR;
        cmd->cmd_type = CMD_CALL_GLOB_VAR_TYPE_NUM;
    }
    else if (tok_num==1)
    {
        cmd->is_compl = 1;
        cmd->cmd_class = CMD_CLASS_CALL_GLOB_VAR;
        cmd->cmd_type = CMD_CALL_GLOB_VAR_TYPE_NUM;
    }
}



/** Global ülňini çagyrýan komandasynyň semantikasy
**/
int semantic_cmd_call_glob_var(command *cmd)
{
    // KOMANDANYN CHEPE BAGLANMA GORNUSHI UCHIN
    command_item *sci = get_cmd_item(cmd->items,1);
    token *item = &sci->tok; // Gysgaltmak uchin ulanlyar

    if (!is_glob_var_def_exist(item->potentional_types[0].value) && !is_glob_var_dec_exist(item->potentional_types[0].value))
    {
        CUR_PART = 7;
        print_err(CODE7_UNKNOWN_GLOB_VAR_CALLED, item);
    }
    return 1;
}


int cmd_call_glob_var_return_type(command *cmd, int *return_class, int *ret_type)
{
    /** Eger identifikator global ülňileriň arasynda bar bolsa,
            global ülňileriň sanawyndan identifikatoryň maglumaty gaýtarylýar.
        ýogsa
            global ülňileriň maglumatlarynyň sanawyndan identifikatoryň maglumaty gaýtarylýar.
    **/
    command_item *sci = get_cmd_item(cmd->items,1);
    if (is_glob_var_def_exist(sci->tok.potentional_types[0].value))
    {
        get_var_def_value_type(sci->tok.potentional_types[0].value, return_class, ret_type, 1);
        return 1;
    }
    else if (is_glob_var_dec_exist(sci->tok.potentional_types[0].value))
    {
        get_glob_var_dec_value_type(sci->tok.potentional_types[0].value, return_class, ret_type);
        return 1;
    }
    CUR_PART = 7;
    print_err(CODE7_UNKNOWN_GLOB_VAR_CALLED, (token *)inf_get_last_token(cmd));
    return 0;
}



/** Faýla degişli kody C koda ýazýar
**/
void cmd_call_glob_var_c_code(command *cmd, char **line, int *llen)
{
    if (!(*llen))
    {
        // Çepe baglanma:
        *llen += strlen("\t")+1;
        *line = realloc(*line, *llen);

        // Içki funksiýanyň içinde bolany üçin, tab goýulyp blokdadygy görkezilýär.
        strncpy(*line, "\t", strlen("\t")+1);
    }

    // Eger birinji birlik ülňi yglan etmek bolsa, komandanyň içinden tokeniň ady alynýar
    // Eger birinji ülňi identifikator bolsa, özi alynýar.
    command_item *sci = get_cmd_item(cmd->items,1);
    token_type *ty = &sci->tok.potentional_types[0];

    *llen += strlen(ty->value);
    *line = realloc(*line, *llen);
    strncat(*line,ty->value,strlen(ty->value));

    // Setir faýla ýazylan soň, setir üçin berlen ýer boşadylýar.
}

