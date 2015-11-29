/** Deňeşdirme komandasy.
    Dolandyrýan komandalarda ulanmak üçin döredildi
**/
#include <stdlib.h>
#include <string.h>
#include "cmp.h"
#include "../main/glob.h"
#include "../error.h"

/**
    Bir görnüşi bolýar.

    Ül birlikden ybarat bolýar.
      Birinji birlik: san gaýtarýan maglumat bolmaly.
      Ikinji birlik:  san gaýtarýan maglumat bolmaly.
      Üçünji birlik:  san gaýtarýan maglumat bolmaly.

    Maglumat gaýtaranok.

**/
int CMD_CMP_TYPE  = 0;

int is_cmd_cmp(command *cmd)
{
    /** Birinji element ýa drob ýa san gaýtarýan maglumat bolmaly.
        Ikinji element haýsy-da bolsa bir operator bolmaly: +, -, *, :.
        Üçünji element ýa drob ýa san gaýtarýan maglumat bolmaly.
    **/
    if (cmd->items_num>CMD_MAX_ITEMS[CMD_CLASS_CMP][CMD_CMP_TYPE] || !cmd->items_num)
	{
		return 0;
	}

    int ret_class = -1, ret_type = -1;

    command_item *e1 = get_cmd_item(cmd->items,0);
    if(e1->type==TOKEN_ITEM)
        TOK_RETURN_TYPE[e1->tok.potentional_types[0].type_class][e1->tok.potentional_types[0].type_num](&e1->tok, &ret_class, &ret_type);
    else if (e1->type==CMD_ITEM)
        CMD_RETURN_TYPE[e1->cmd.cmd_class][e1->cmd.cmd_type](&e1->cmd,&ret_class, &ret_type);
    else if (e1->type==PAREN_ITEM)
        PAREN_RETURN_TYPE[e1->paren.type](&e1->paren, &ret_class, &ret_type);

    if (ret_class==TOK_CLASS_CONST_DATA && ret_type==INT_CONST_DATA_TOK_NUM)
    {
        cmd_cmp_mod(cmd, 0);
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
		if (e2->type==TOKEN_ITEM && e2->tok.type_class==TOK_CLASS_CMP)
		{
			cmd_cmp_mod(cmd, 1);
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
		   TOK_RETURN_TYPE[e3->tok.potentional_types[0].type_class][e3->tok.potentional_types[0].type_num](&e3->tok, &ret_class, &ret_type);
        else if(e3->type==CMD_ITEM)
           CMD_RETURN_TYPE[e3->cmd.cmd_class][e3->cmd.cmd_type](&e3->cmd,&ret_class, &ret_type);
        else if(e3->type==PAREN_ITEM)
            PAREN_RETURN_TYPE[e3->paren.type](&e3->paren, &ret_class, &ret_type);

        if (ret_class==TOK_CLASS_CONST_DATA && ret_type==INT_CONST_DATA_TOK_NUM)
		{
		    cmd_cmp_mod(cmd, 2);
		}
		else
		{
            return 0;
		}
	}

	return 1;
}


void cmd_cmp_mod(command *cmd, int item_num)
{
    /** Birinji element boýunça:
        Diňe komandanyň klasy bellenilýär.*/
    if (!item_num)
    {
        cmd->cmd_class = CMD_CLASS_CMP;
        cmd->is_compl  = 0;
        cmd->ns        = 0;
        cmd->cmd_type  = -1;
    }

    /** Ikinji element boýunça:
        Diňe komandanyň tipi bellenilýär.*/
    else if (item_num==1)
    {
        cmd->cmd_type = CMD_CMP_TYPE;
    }
    /** Üçünji element boýunça:
        Komanda gutarandygy bellenilýär.*/
    else if(item_num==2)
    {
        cmd->is_compl = 1;
    }
}


int semantic_cmd_cmp(command *cmd)
{
    int prev_part = CUR_PART;
    CUR_PART = 7;

    int e1_class = -1, e1_type = -1, e3_class = -1, e3_type = -1;
    command_item *e1 = get_cmd_item(cmd->items,0), *e3 = get_cmd_item(cmd->items,2);

    //      ulninin yglan etme bolsa                      - identifikator hokman goshulandyr shonun uchin barlananok
    //      eyyam ygaln edilen ulnin identifikatory bolsa - identifikator hokman on bir yerde yglan edilmeli
    if((e1->type==TOKEN_ITEM &&
        TOK_RETURN_TYPE[e1->tok.potentional_types[0].type_class][e1->tok.potentional_types[0].type_num]
        (&e1->tok, &e1_class, &e1_type) && e1_class!=TOK_CLASS_UNDEFINED) ||
        (e1->type==CMD_ITEM &&
        CMD_RETURN_TYPE[e1->cmd.cmd_class][e1->cmd.cmd_type](&e1->cmd,&e1_class, &e1_type) &&
        e1_class!=TOK_CLASS_UNDEFINED) ||
        (e1->type==PAREN_ITEM &&
        PAREN_RETURN_TYPE[e1->paren.type](&e1->paren, &e1_class, &e1_type) && e1_class!=TOK_CLASS_UNDEFINED))
    {
        if (e1_type!=INT_CONST_DATA_TOK_NUM)
        {
            printf("SAGBOL 20");
            if (e1->type==TOKEN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_LEFT_DATA, &e1->tok);
            else if(e1->type==CMD_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_LEFT_DATA, (token *)inf_get_last_token(&e1->cmd));
            else if(e1->type==PAREN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_LEFT_DATA, (token *)inf_get_parens_last_token(&e1->paren));
        }
    }

    if((e3->type==TOKEN_ITEM &&
        TOK_RETURN_TYPE[e3->tok.potentional_types[0].type_class][e3->tok.potentional_types[0].type_num]
        (&e3->tok, &e3_class, &e3_type) && e3_class!=TOK_CLASS_UNDEFINED) ||
        (e3->type==CMD_ITEM &&
        CMD_RETURN_TYPE[e3->cmd.cmd_class][e3->cmd.cmd_type](&e3->cmd,&e3_class, &e3_type) &&
        e3_class!=TOK_CLASS_UNDEFINED) ||
        (e3->type==PAREN_ITEM &&
        PAREN_RETURN_TYPE[e3->paren.type](&e3->paren, &e3_class, &e3_type) && e3_class!=TOK_CLASS_UNDEFINED))
    {
        if (e3_type!=INT_CONST_DATA_TOK_NUM)
        {
            printf("SAGBOL 21");
            if (e1->type==TOKEN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, &e1->tok);
            else if(e1->type==CMD_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, (token *)inf_get_last_token(&e1->cmd));
            else if(e1->type==PAREN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, (token *)inf_get_parens_last_token(&e1->paren));
        }
    }

    CUR_PART = prev_part;
    return 1;
}



/** Faýla degişli kody C koda ýazýar
**/
void cmd_cmp_c_code(command *cmd, char **l, int *llen)
{
    if (!(*llen))
    {
        // Çepe baglanma:
        *llen += strlen("\t")+1;
        *l = realloc(*l, *llen);

        // Içki funksiýanyň içinde bolany üçin, tab goýulyp blokdadygy görkezilýär.
        strncpy(*l, "\t", strlen("\t")+1);
    }

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
    char *cmp_c = TOK_CLASS_CMP_CHARS[e2->tok.potentional_types[0].type_num][1];
    *llen += strlen(cmp_c);
    *l = realloc(*l, *llen);
    strncat(*l,cmp_c,strlen(cmp_c));

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

