/**
 * Baglanma komandasy
 *
 * Baglanmanyn iki sany gornushi bar:
 * 	   1) Chepindaki ulna baglanma
 *     2) Sagyndaky ulna baglanma
**/

#include "../main/glob.h"
#include "../error.h"
#include "assign.h"

/**
 * Chepindaki ulna baglanma
 * Uch sany birliklerden durup bilyar.
 *
 * a) Birinji birlige uchunji birlikdaki maglumat gechirilyar:
 *    Birlik shu gornushlerde bolup bilyar:
 *     1) (komanda) ulni yglan etme               - taze ulni yglan edilende
 *                                                maglumat baglap bolyar.
 *     2) (token)   identifikator, ulninin ady    - on yglan edilen ulna
 *                                                maglumaty baglap bolyar.
 *
 * b) Ikinji birlik Baglanma token gornushinin chepine baglayan
 *    hilisinin tokeni bolmaly.
 *
 * c) Uchunji birlik birinji birlige baglanyan maglumat bolmaly.
 *    Birlik shu gornushlerden bolup bilyar:
 *    1) (token)  identifikator, ulninin ady    - on yglan edilen ulni taze
 *                                                ulninin maglumaty baglap bolyar.
**/


/**
 * Berlen komandanyň 'assign' komandasymy ýa däldigini barlýar.
 * Eger barlanýan komanda şowly tanalsa, onda komandanyň maglumatlary şuňa görä üýtgedilýär.
**/
int is_cmd_assign(command *cmd)
{
	if (cmd->items_num>ASSIGN_MAX_ITEMS || !cmd->items_num)
	{
		// Komandany saygaryp bolmady
		cmd->cmd_class=0;
		cmd->cmd_type=0;
		return 0;
	}

	//printf("Birinji birlik barlanmaly\n");
	int next_item_class=-1, next_item_type=-1;
	// Birinji token ya ulni yglan etmek bolmaly ya identifikator.
	if (cmd->items[0].type==TOKEN_ITEM &&
		cmd->items[0].tok.type_class==TOK_CLASS_IDENT)
	{
		assign_cmd_mod(cmd, 0);
		next_item_class=TOK_CLASS_ASSIGN;
	}
	else if (cmd->items[0].type==CMD_ITEM &&
		cmd->items[0].cmd.cmd_class==DEF_VAR_CLASS_NUM)
	{
		assign_cmd_mod(cmd, 0);
		next_item_class=TOK_CLASS_ASSIGN;
		// Indiki token dine chepe tarap baglanma bolmaly
		next_item_type= LEFT_ASSIGN_TOK_NUM;

	}
	else if (cmd->items[0].type==TOKEN_ITEM &&
          cmd->items[0].tok.type_class==TOK_CLASS_CONST_DATA)
	{
        //assign_cmd_mod(cmd, 0);
		//next_item_class=TOK_CLASS_ASSIGN;
		// Indiki token dine chepe tarap baglanma bolmaly
		//next_item_type= RIGHT_ASSIGN_TOK_NUM;
    }
    else
    {
		//debug_cmd(cmd);
		//printf("Birinji bolum, tokeninki dal\n");
		// Komandany saygaryp bolmady
		//printf("Umumy ulni bolmaly");
		// Komandany saygaryp bolmady
		//cmd->cmd_class=0;
		//cmd->cmd_type=0;
		return 0;
	}
	//debug_cmd(cmd);
	//printf("Birinji birlik tanaldy\n");

	// Ikinji birlik, eger birinji ulni yglan edilme bolsa,
	// Baglanmanyn chepe gidyan token gornushi bolmaly, yogsa ikisem bolyar
	if(cmd->items_num>=1)
	{
		if (next_item_class>0 &&
			cmd->items[1].type==TOKEN_ITEM &&
			cmd->items[1].tok.type_class==TOK_CLASS_ASSIGN)
		{
			// Token dine chepe tarap baglanma bolmaly
			if (next_item_type>0 && !cmd->items[1].tok.potentional_types[0].type_num==next_item_type)
			{
				//printf("Token chepe tarap bakyan dal\n");
				// Komandany saygaryp bolmady
				//cmd->cmd_class = 0;
				//cmd->cmd_type = 0;
				//printf("Umumy ulni bolmaly");
				return 0;
			}
			assign_cmd_mod(cmd, 1);
			//printf("Birlik baglanmanynky eken\n");
		}
		else
		{
			//printf("Ikinji tapgyrda-da tanalmady\n");
			// Komandany saygaryp bolmady
			//printf("Umumy ulni bolmaly");
			// Komandany saygaryp bolmady
			//cmd->cmd_class=0;
			//cmd->cmd_type=0;
			return 0;
			//debug_cmd(cmd);
		}
	}
	//debug_cmd(cmd);
	//printf("Ikinji bolum tanaldy");

	// Uchunji token identifikator bolmaly.
	if (cmd->items_num>=2)
	{
	    if(cmd->items[2].type==TOKEN_ITEM &&
		   cmd->items[2].tok.type_class==TOK_CLASS_IDENT ||
		   cmd->items[2].type==TOKEN_ITEM &&
           cmd->items[2].tok.type_class==TOK_CLASS_CONST_DATA)
		{
			assign_cmd_mod(cmd, 2);

			// Tokeniň görnüşi barlanmaly.

		//printf("Uchunji birlik identifikator eken\n");
		}
		else
		{
		// Komandany saygaryp bolmady
		//cmd->cmd_class=0;
		//cmd->cmd_type=0;
			return 0;
		}
	}
	return 1;
}


/**
 * Tokene görä komandanyň maglumatlaryny üýtgedýär.
 *
 * @cmd      - üýtgedilmeli komanda
 * @tokNum   - tokene görä üýtgedilmeli.
**/
int assign_cmd_mod(command *cmd, int tok_num)
{
	// Birinji birlik
	if (tok_num==0)
	{
		cmd->cmd_class = ASSIGN_CLASS_NUM;
		cmd->is_compl = 0;

		// Eger birlik global ulni yglan etme bolsa
		// Bu token in sonky. Shonun uchin lokalmy ya yokdugyny barlap bolyar
		if (!(cmd->items[0].type==CMD_ITEM && is_glob_def_var_cmd(&cmd->items[0].cmd)) )
		{
			cmd->ns = LOCAL;
		}
		return 1;
	}
	else if (tok_num==1)
	{
		cmd->cmd_type = cmd->items[1].tok.potentional_types[0].type_num;
		cmd->cmd_class = ASSIGN_CLASS_NUM;

		cmd->is_compl = 0;

		return 1;
	}
	else if (tok_num==2)
	{
		cmd->cmd_class = ASSIGN_CLASS_NUM;

		cmd->is_compl = 1;

		// Semantika barlanýar
		/** TODO
		 * Eger baglanmaly tokeniň görnüşi konstanta maglumat bolmasa,
		 * Mysal üçin: san, drob, harp ýa harp düzümi bolmasa.
		 * Onda tokeniň görnüşi barlanmaly (token.token_type.type_must_check = 1)
		**/


		if (cmd->items[tok_num].type==TOKEN_ITEM &&
            cmd->items[tok_num].tok.type_class==TOK_CLASS_CONST_DATA)
        {
            //printf();
            if (!add_global_right_data_item(cmd))
            {
                // Hazir TPL-in fayly komandalar bilen ishleyan boluminde
                int prev_part = CUR_PART;
                CUR_PART = 4;
                print_err(CODE4_TOK_TYPES_NOT_MATCH);
            }
        }
        else if (cmd->items[tok_num].type==TOKEN_ITEM &&
            cmd->items[tok_num].tok.type_class==TOK_CLASS_IDENT)
        {
            // TODO
            /*if (!add_global_compare_both_ident_item(cmd))
            {
                // Hazir TPL-in fayly komandalar bilen ishleyan boluminde
                int prev_part = CUR_PART;
                CUR_PART = 4;
                print_err(CODE4_TOK_TYPES_NOT_MATCH);
            }*/
        }
        // TODO, eger birinji birlik maglumat bolup ikinjisi identifikator bolsa.


		return 1;
	}
	return 0;
}


