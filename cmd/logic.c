/** Logiki komandalar */
#include <stdlib.h>
#include <string.h>
#include "logic.h"
#include "../cmds.h"
#include "../translator_to_c.h"
#include "../fns.h"

int CMD_LOGIC_BINAR_TYPE = 0;
int CMD_LOGIC_MONO_TYPE  = 1;


int is_cmd_logic(command *cmd)
{
    /** Birinji element deňeşdirme komandasy bolmaly.
        Ikinji element haýsy-da bolsa bir operator bolmaly: &, ?, !.
        Eger ikinji operator DÄL BOLSA bolmasa
            Üçünji element deňedirme komandasy bolmaly.
    **/
    if (cmd->items_num>CMD_MAX_ITEMS[CMD_CLASS_LOGIC][CMD_LOGIC_BINAR_TYPE] || !cmd->items_num)
	{
		return 0;
	}

    int req_class1 = CMD_CLASS_CMP, req_class2 = CMD_CLASS_LOGIC;

    command_item *e1 = get_cmd_item(cmd->items,0);
    if(e1->type==TOKEN_ITEM || e1->type==CMD_ITEM)
    {
        return 0;
    }
    if (e1->type==PAREN_ITEM)
    {
        int paren_item_type = -1, ret_class = -1, ret_type = -1;
        if ((!get_paren_item_type(&e1->paren, &paren_item_type, &ret_class, &ret_type) || paren_item_type!=CMD_ITEM || ret_class!=req_class1) &&
            (!get_paren_item_type(&e1->paren, &paren_item_type, &ret_class, &ret_type) || paren_item_type!=CMD_ITEM || ret_class!=req_class2 ))
            return 0;
    }
    cmd_binar_logic_mod(cmd, 0);


	// Ikinji birlik, eger birinji ulni yglan edilme bolsa,
	// Baglanmanyn chepe gidyan token gornushi bolmaly, yogsa ikisem bolyar
	if(cmd->items_num>1)
	{
	    command_item *e2 = get_cmd_item(cmd->items,1);
		if (e2->type==TOKEN_ITEM && e2->tok.type_class==TOK_CLASS_LOGIC)
		{
		    if (e2->tok.potentional_types[0].type_num==TOK_CLASS_LOGIC_NOT_TYPE)
			{
			    cmd_mono_logic_mod(cmd, 1);
			    return 1;
			}
			cmd_binar_logic_mod(cmd, 1);
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
	    if(e3->type==TOKEN_ITEM || e3->type==CMD_ITEM)
        {
            return 0;
        }
        if (e3->type==PAREN_ITEM)
        {
            int paren_item_type = -1, ret_class = -1, ret_type = -1;
            if ((!get_paren_item_type(&e3->paren, &paren_item_type, &ret_class, &ret_type) || paren_item_type!=CMD_ITEM || ret_class!=req_class1) &&
                (!get_paren_item_type(&e3->paren, &paren_item_type, &ret_class, &ret_type) || paren_item_type!=CMD_ITEM || ret_class!=req_class2 ) )
                return 0;
        }
        cmd_binar_logic_mod(cmd, 2);
	}
	return 1;
}


void cmd_mono_logic_mod(command *cmd, int item_num)
{
    /** Birinji element boýunça:
        Diňe komandanyň klasy bellenilýär.*/
    if (!item_num)
    {
        cmd->cmd_class = CMD_CLASS_LOGIC;
        cmd->is_compl  = 0;
        cmd->ns        = 0;
        cmd->cmd_type  = -1;
    }

    /** Ikinji element boýunça:
        Diňe komandanyň tipi bellenilýär.*/
    else if (item_num==1)
    {
        cmd->cmd_type = CMD_LOGIC_MONO_TYPE;
        cmd->is_compl = 1;
    }
}



void cmd_binar_logic_mod(command *cmd, int item_num)
{
    /** Birinji element boýunça:
        Diňe komandanyň klasy bellenilýär.*/
    if (!item_num)
    {
        cmd->cmd_class = CMD_CLASS_LOGIC;
        cmd->is_compl  = 0;
        cmd->ns        = 0;
        cmd->cmd_type  = -1;
    }

    /** Ikinji element boýunça:
        Diňe komandanyň tipi bellenilýär.*/
    else if (item_num==1)
    {
        cmd->cmd_type = CMD_LOGIC_BINAR_TYPE;
    }
    /** Üçünji element boýunça:
        Komanda gutarandygy bellenilýär.*/
    else if(item_num==2)
    {
        cmd->is_compl = 1;
    }
}



/** Faýla degişli kody C koda ýazýar
**/
void cmd_logic_c_code(command *cmd, wchar_t **l, int *llen)
{
    /** ŞERT #1: Logiki komandalaryň Faýla Ç kody ýazymaly:
        Iki maglumat saklaýan logiki komandalar çep we sag tarapdaky maglumatlaryň daşlarynda ýaý goýulýar,
        Sebäbi komandalar biri biriniň içine salynanda deň derejede bolýalar.

        Bir maglumat saklaýan logiki komandalarda çep maglumat ýaýa alynýar */
    wchar_t *o = L"(",
            *c = L")";

    if (cmd->cmd_type==CMD_LOGIC_MONO_TYPE)
    {
        command_item *e2 = get_cmd_item(cmd->items,1);
        write_cmd_item_c_code( e2, l, llen );

        wcsadd_on_heap( l, llen, o );

        command_item *e1 = get_cmd_item(cmd->items,0);
        write_cmd_item_c_code( e1, l, llen );

        wcsadd_on_heap( l, llen, c );
    }
    else
    {
        wcsadd_on_heap( l, llen, o );

        command_item *e1 = get_cmd_item(cmd->items,0);
        write_cmd_item_c_code( e1, l, llen );

        command_item *e2 = get_cmd_item(cmd->items,1);
        write_cmd_item_c_code(e2, l, llen ); /// Tokeniň Kodyny çap edýär

        command_item *e3 = get_cmd_item(cmd->items,0);
        write_cmd_item_c_code(e3, l, llen );

        wcsadd_on_heap( l, llen, c );
    }
}

