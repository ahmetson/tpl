/** Arifmetiki operatorlar
**/
#include <string.h>
#include <stdlib.h>
#include "arif.h"

#include "../cmds.h"
#include "../main/glob.h"
#include "../main/inf.h"
#include "fn_call.h"
#include "../error.h"

/// Iki sany tipleri bolýar
int CMD_ARIF_LOW_PRIOR_TYPE  = 0;   // +, -
int CMD_ARIF_HIGH_PRIOR_TYPE = 1;   // *, :

int is_cmd_arif(command *cmd)
{
    /** Birinji element ýa drob ýa san gaýtarýan maglumat bolmaly.
        Ikinji element haýsy-da bolsa bir operator bolmaly: +, -, *, :.
        Üçünji element ýa drob ýa san gaýtarýan maglumat bolmaly.
    **/

    /// Ähli arifmetiki görnüşleri edil birinji ärifmetiki komandanyň görnüşi ýaly birlikleri kabul etmeli.
    /// Birinji arifmetiki komanda görnüşi bolsa, 3 birligi kabul edýär
    if (cmd->items_num>CMD_MAX_ITEMS[CMD_CLASS_ARIF][CMD_ARIF_LOW_PRIOR_TYPE] || !cmd->items_num)
	{
		return 0;
	}

    int ret_class = -1, ret_type = -1;

    command_item *e1 = get_cmd_item(cmd->items, 0);
    if(e1->type==TOKEN_ITEM)
        return_tok_type(&e1->tok, &ret_class, &ret_type);
    else if (e1->type==CMD_ITEM)
        CMD_RETURN_TYPE[e1->cmd.cmd_class][e1->cmd.cmd_type](&e1->cmd,&ret_class, &ret_type);
    else if (e1->type==PAREN_ITEM)
        PAREN_RETURN_TYPE[e1->paren.type](&e1->paren, &ret_class, &ret_type);

    if (ret_class==TOK_CLASS_CONST_DATA && (ret_type==INT_CONST_DATA_TOK_NUM || ret_type==FLOAT_CONST_DATA_TOK_NUM))
    {
        cmd_arif_mod(cmd, 0);
    }
    else
    {
        return 0;
    }


	// Ikinji birlik, eger birinji ulni yglan edilme bolsa,
	// Baglanmanyn chepe gidyan token gornushi bolmaly, yogsa ikisem bolyar
	if(cmd->items_num>1)
	{
	    command_item *e2 = get_cmd_item(cmd->items,1);
		if (e2->type==TOKEN_ITEM && e2->tok.type_class==TOK_CLASS_ARIF)
		{
			cmd_arif_mod(cmd, 1);
			//printf("Birlik baglanmanynky eken\n");
		}
		else
		{
			return 0;
		}
	}

	// Uchunji token identifikator bolmaly.
	if (cmd->items_num>2)
	{
	    command_item *e3 = get_cmd_item(cmd->items,2);
	    if(e3->type==TOKEN_ITEM)
		   return_tok_type(&e3->tok, &ret_class, &ret_type);
        else if(e3->type==CMD_ITEM)
           CMD_RETURN_TYPE[e3->cmd.cmd_class][e3->cmd.cmd_type](&e3->cmd,&ret_class, &ret_type);
        else if(e3->type==PAREN_ITEM)
            PAREN_RETURN_TYPE[e3->paren.type](&e3->paren, &ret_class, &ret_type);

        if (ret_class==TOK_CLASS_CONST_DATA && (ret_type==INT_CONST_DATA_TOK_NUM || ret_type==FLOAT_CONST_DATA_TOK_NUM))
		{
		    cmd_arif_mod(cmd, 2);
		}
		else
		{
            return 0;
		}
	}

	return 1;
}


int cmd_arif_mod(command *cmd, int item_num)
{
    /** Birinji element boýunça:
            Diňe komandanyň klasy bellenilýär.
    **/
    if (!item_num)
    {
        cmd->cmd_class = CMD_CLASS_ARIF;
        cmd->is_compl  = 0;
        cmd->ns        = 0;
        cmd->cmd_type  = -1;
    }

    /** Ikinji element boýunça:
            Diňe komandanyň tipi bellenilýär.
    **/
    else if (item_num==1)
    {
        command_item *sci = get_cmd_item(cmd->items, 1);
        if (sci->tok.potentional_types[0].type_num==0 || sci->tok.potentional_types[0].type_num==1)
            cmd->cmd_type = CMD_ARIF_LOW_PRIOR_TYPE;
        else if (sci->tok.potentional_types[0].type_num==2 || sci->tok.potentional_types[0].type_num==3)
            cmd->cmd_type = CMD_ARIF_HIGH_PRIOR_TYPE;
    }
    /**
        Üçünji element boýunça:
            Komanda gutarandygy bellenilýär.
    **/
    else if(item_num==2)
    {
        cmd->is_compl = 1;
    }
    return 1;
}


int semantic_cmd_arif(command *cmd)
{
    int prev_part = CUR_PART;
    CUR_PART = 7;
    // KOMANDANYN CHEPE BAGLANMA GORNUSHI UCHIN
    //printf("Arifmetiki komandanyn semantikasy barlanmaly\n");

    /** BIRINJI ELEMENT
        1) Garaşylýan tipi SAN ýa DROB.
        2) Eger element näbelli tip gaýtarsa,
            a)"Nädogry tip geldi" diýen ýalňyşlyk gaýtarylýar.
        Ýogsa
            b)Birinji elementiň tipi bellenilýär.

        IKINJI ELEMENT
        3)Eger element näbelli tip gaýtarsa
            a)"NÄDOGRY tip geldi" diýen ýalňyşlyk gaýtarylýar.
        Ýa eger birinji element hem gaýtarýan tipi belli bolup, iki elementiň tipleri gabat gelmese
            b)"Nädogry tip geldi" diýen ýalňyşlyk gaýtarylýar.

        4)Şowlulyk gaýtarylýar.
    */

    int e1_class = -1, e1_type = -1, e3_class = -1, e3_type = -1;
    command_item *e1 = get_cmd_item(cmd->items, 0), *e3 = get_cmd_item(cmd->items, 2);

    //      ulninin yglan etme bolsa                      - identifikator hokman goshulandyr shonun uchin barlananok
    //      eyyam ygaln edilen ulnin identifikatory bolsa - identifikator hokman on bir yerde yglan edilmeli
    if((e1->type==TOKEN_ITEM &&
        return_tok_type(&e1->tok, &e1_class, &e1_type) && e1_class!=TOK_CLASS_UNDEFINED) ||
        (e1->type==CMD_ITEM &&
        CMD_RETURN_TYPE[e1->cmd.cmd_class][e1->cmd.cmd_type](&e1->cmd,&e1_class, &e1_type) &&
        e1_class!=TOK_CLASS_UNDEFINED) ||
        (e1->type==PAREN_ITEM &&
        PAREN_RETURN_TYPE[e1->paren.type](&e1->paren, &e1_class, &e1_type) && e1_class!=TOK_CLASS_UNDEFINED))
    {
        if (e1_type!=INT_CONST_DATA_TOK_NUM && e1_type!=FLOAT_CONST_DATA_TOK_NUM)
        {
            printf("SAGBOL 1");
            if (e1->type==TOKEN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_LEFT_DATA, &e1->tok);
            else if(e1->type==CMD_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_LEFT_DATA, (token *)inf_get_last_token(&e1->cmd));
            else if(e1->type==PAREN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_LEFT_DATA, (token *)inf_get_parens_last_token(&e1->paren));
        }
        else
        {

        }
    }

    if((e3->type==TOKEN_ITEM &&
        return_tok_type(&e3->tok, &e3_class, &e3_type) && e3_class!=TOK_CLASS_UNDEFINED) ||
        (e3->type==CMD_ITEM &&
        CMD_RETURN_TYPE[e3->cmd.cmd_class][e3->cmd.cmd_type](&e3->cmd,&e3_class, &e3_type) &&
        e3_class!=TOK_CLASS_UNDEFINED) ||
        (e3->type==PAREN_ITEM &&
        PAREN_RETURN_TYPE[e3->paren.type](&e3->paren, &e3_class, &e3_type) && e3_class!=TOK_CLASS_UNDEFINED))
    {
        if (e3_type!=INT_CONST_DATA_TOK_NUM && e3_type!=FLOAT_CONST_DATA_TOK_NUM)
        {
            printf("SAGBOL 2");
            if (e1->type==TOKEN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, &e1->tok);
            else if(e1->type==CMD_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, (token *)inf_get_last_token(&e1->cmd));
            else if(e1->type==PAREN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, (token *)inf_get_parens_last_token(&e1->paren));
        }
        else if (!(e1_class==e3_class && e1_type==e3_type))
        {
            printf("SAGBOL 3");
            if (e1->type==TOKEN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_BOTH_IDENT, &e1->tok);
            else if(e1->type==CMD_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_BOTH_IDENT, (token *)inf_get_last_token(&e1->cmd));
            else if(e1->type==PAREN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_BOTH_IDENT, (token *)inf_get_parens_last_token(&e1->paren));
        }
    }

    CUR_PART = prev_part;
    return 1;
}


int cmd_arif_return_type(command *cmd, int *ret_class, int *ret_type)
{
    /** Birinji elementiň tipi gaýtarylýar.
    **/
    command_item *e1 = get_cmd_item(cmd->items,0);

    if(e1->type==TOKEN_ITEM)
        return_tok_type(&e1->tok, ret_class, ret_type);
    else if(e1->type==CMD_ITEM)
        CMD_RETURN_TYPE[e1->cmd.cmd_class][e1->cmd.cmd_type](&e1->cmd, ret_class, ret_type);
    else if(e1->type==PAREN_ITEM)
        PAREN_RETURN_TYPE[e1->paren.type](&e1->paren, ret_class, ret_type);
    return 1;
}


/** Faýla degişli kody C koda ýazýar
**/
void cmd_arif_c_code(command *cmd, char **l, int *llen)
{
    // Eger birinji birlik ülňi yglan etmek bolsa, komandanyň içinden tokeniň ady alynýar
    // Eger birinji ülňi identifikator bolsa, özi alynýar.
    command_item *e1 = get_cmd_item(cmd->items,0), *e2 = get_cmd_item(cmd->items,1), *e3 = get_cmd_item(cmd->items,2);
    if (e1->type==CMD_ITEM)
    {
        CMD_GET_C_CODE[e1->cmd.cmd_class][e1->cmd.cmd_type](&e1->cmd, l, llen);
    }
    else if (e1->type==TOKEN_ITEM)
    {
        TOK_GET_C_CODE[e1->tok.potentional_types[0].type_class][e1->tok.potentional_types[0].type_num](&e1->tok, l, llen);
    }
    else if (e1->type==PAREN_ITEM)
    {
        paren_get_c_code(&e1->paren, l, llen);
    }

    // baglanma ülňiniň c dili üçin warianty goýulýar
    char *arif_c = TOK_CLASS_ARIF_CHARS[e2->tok.potentional_types[0].type_num][1];
    *llen += strlen(arif_c);
    *l = realloc(*l, *llen);
    strncat(*l,arif_c,strlen(arif_c));


    if (e3->type==CMD_ITEM)
    {
        CMD_GET_C_CODE[e3->cmd.cmd_class][e3->cmd.cmd_type](&e3->cmd, l, llen);
    }
    else if (e3->type==TOKEN_ITEM)
    {
        TOK_GET_C_CODE[e3->tok.potentional_types[0].type_class][e3->tok.potentional_types[0].type_num](&e3->tok, l, llen);
    }
    else if (e3->type==PAREN_ITEM)
    {
        paren_get_c_code(&e3->paren, l, llen);
    }
    // Üç birligi birikdirip täze setir ýasalýar.
    // Setire üç birlik we komandany gutaryjy çatylýar.
    // Setir faýla ýazylan soň, setir üçin berlen ýer boşadylýar.
    //printf("%s\n", *l);
}

