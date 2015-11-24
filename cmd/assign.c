/**
 * Baglanma komandasy
 *
 * Baglanmanyn iki sany gornushi bar:
 * 	   1) Chepindaki ulna baglanma
 *     2) Sagyndaky ulna baglanma
**/
#include <stdlib.h>
#include <string.h>

#include "../main/glob.h"
#include "../error.h"
#include "def_var.h"
#include "assign.h"
#include "../semantic.h"
#include "../cmds.h"
#include "../main/inf.h"

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


/** Berlen komandanyň 'assign' komandasymy ýa däldigini barlýar.
 *  Eger barlanýan komanda şowly tanalsa, onda komandanyň maglumatlary şuňa görä üýtgedilýär.
**/
int is_cmd_assign(command *cmd)
{

	if (cmd->items_num>CMD_MAX_ITEMS[cmd->cmd_class][cmd->cmd_type] || !cmd->items_num)
	{
		return 0;
	}
	int next_item_class=-1, next_item_type=-1;
	// Birinji token ya ulni yglan etmek bolmaly ya identifikator.
	if (cmd->items[0].type==TOKEN_ITEM &&
		cmd->items[0].tok.type_class==TOK_CLASS_IDENT)
	{
		assign_cmd_mod(cmd, 0);
		next_item_class=TOK_CLASS_ASSIGN;
	}
	else if (cmd->items[0].type==CMD_ITEM &&
		cmd->items[0].cmd.cmd_class==CMD_CLASS_DEF_VAR)
	{
		assign_cmd_mod(cmd, 0);
		next_item_class=TOK_CLASS_ASSIGN;
		// Indiki token dine chepe tarap baglanma bolmaly
		next_item_type= LEFT_ASSIGN_TOK_NUM;

	}
	else if (cmd->items[0].type==CMD_ITEM &&
		cmd->items[0].cmd.cmd_class==CMD_CLASS_CALL_GLOB_VAR)
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
		return 0;
	}

	// Ikinji birlik, eger birinji ulni yglan edilme bolsa,
	// Baglanmanyn chepe gidyan token gornushi bolmaly, yogsa ikisem bolyar
	if(cmd->items_num>1)
	{
		if (next_item_class>0 &&
			cmd->items[1].type==TOKEN_ITEM &&
			cmd->items[1].tok.type_class==TOK_CLASS_ASSIGN)
		{
			// Token dine chepe tarap baglanma bolmaly
			if (next_item_type>0 && !cmd->items[1].tok.potentional_types[0].type_num==next_item_type)
			{
				return 0;
			}
			assign_cmd_mod(cmd, 1);
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
	    int ret_class = -1, ret_type = -1;
	    if((cmd->items[2].type==TOKEN_ITEM &&
		   TOK_RETURN_TYPE[cmd->items[2].tok.potentional_types[0].type_class][cmd->items[2].tok.potentional_types[0].type_num]
		   (&cmd->items[2].tok, &ret_class, &ret_type) && ret_class!=TOK_CLASS_UNDEFINED) ||
		   (cmd->items[2].type==CMD_ITEM &&
           CMD_RETURN_TYPE[cmd->items[2].cmd.cmd_class][cmd->items[2].cmd.cmd_type](&cmd->items[2].cmd,&ret_class, &ret_type) &&
        ret_class!=TOK_CLASS_UNDEFINED) ||
           (cmd->items[2].type==PAREN_ITEM &&
            PAREN_RETURN_TYPE[cmd->items[2].paren.type](&cmd->items[2].paren, &ret_class, &ret_type) && ret_class!=TOK_CLASS_UNDEFINED))
		{

		    assign_cmd_mod(cmd, 2);
		}
		else
		{
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
		cmd->cmd_class = CMD_CLASS_ASSIGN;
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
		cmd->cmd_type = cmd->items[1].tok.potentional_types[0].type_num-1;
		cmd->cmd_class = CMD_CLASS_ASSIGN;

		cmd->is_compl = 0;

		return 1;
	}
	else if (tok_num==2)
	{
		cmd->cmd_class = CMD_CLASS_ASSIGN;
		cmd->is_compl = 1;

        // TODO, eger birinji birlik maglumat bolup ikinjisi identifikator bolsa.

		return 1;
	}
	return 0;
}

/** Baglama komandasynyň semantikasy **/
int semantic_cmd_assign(command *cmd)
{
    int prev_part = CUR_PART;
    CUR_PART = 7;
    // KOMANDANYN CHEPE BAGLANMA GORNUSHI UCHIN
    if (cmd->items[1].tok.potentional_types[0].type_num==LEFT_ASSIGN_TOK_NUM)
    {
        //printf("Chepe baglanyan komandanyn semantikasy barlanmaly\n");

        // BIRINJI BIRLIK BARLANYAR
        //      ulninin yglan etme bolsa                      - identifikator hokman goshulandyr shonun uchin barlananok
        //      eyyam ygaln edilen ulnin identifikatory bolsa - identifikator hokman on bir yerde yglan edilmeli
        if (cmd->items[0].type==TOKEN_ITEM && cmd->items[0].tok.type_class==TOK_CLASS_IDENT)
        {
            //debug_cmd(cmd);
            //debug_GLOB_VAR_DEFS(GLOB_VAR_DEFS);
            //debug_LOCAL_VAR_DEFS(LOCAL_VAR_DEFS);
            token *item = &cmd->items[0].tok; // Gysgaltmak uchin ulanlyar

            if (!is_var_def_exist(item->potentional_types[0].value))
            {
                print_err(CODE7_LEFT_IDENT_NOT_DEFINED, item);
            }
        }
        else if (cmd->items[0].type==CMD_ITEM)
        {
            check_semantics(&cmd->items[0].cmd);
        }


        // IKINJI BIRLIGIN BARLANMASY
        //      eyyam yglan edilen ulnin identifikatory bolsa - identifikator hokman on bir yerde yglan edilmeli
        if (cmd->items[2].type==TOKEN_ITEM && cmd->items[2].tok.type_class==TOK_CLASS_IDENT)
        {
            //debug_cmd(cmd);
            //debug_GLOB_VAR_DEFS(GLOB_VAR_DEFS);
            //debug_LOCAL_VAR_DEFS(LOCAL_VAR_DEFS);
            token *item = &cmd->items[2].tok; // Gysgaltmak uchin ulanlyar

            // A->B, B->A yagdayy bolmaz yaly
            //compare_idents_add_new(cmd->items[0].tok.potentional_types[0].value, *item);

            if (!is_var_def_exist(item->potentional_types[0].value))
            {
                print_err(CODE7_RIGHT_IDENT_NOT_DEFINED, item);
            }
        }
        else if (cmd->items[2].type==CMD_ITEM)
        {
            check_semantics(&cmd->items[0].cmd);
        }


        /// Ikinji masagaraçylyk: ikisiniňem tipleri biri birine gabat gelmeli
        int class1 = -1, type1 = -1, class2 = -1, type2 = -1;
	    if((cmd->items[2].type==TOKEN_ITEM &&
		   TOK_RETURN_TYPE[cmd->items[2].tok.potentional_types[0].type_class][cmd->items[2].tok.potentional_types[0].type_num]
		   (&cmd->items[2].tok, &class2, &type2) && class2!=TOK_CLASS_UNDEFINED) ||
		   (cmd->items[2].type==CMD_ITEM &&
           CMD_RETURN_TYPE[cmd->items[2].cmd.cmd_class][cmd->items[2].cmd.cmd_type](&cmd->items[2].cmd,&class2, &type2) &&
        class2!=TOK_CLASS_UNDEFINED) ||
           (cmd->items[2].type==PAREN_ITEM &&
            PAREN_RETURN_TYPE[cmd->items[2].paren.type](&cmd->items[2].paren, &class2, &type2) && class2!=TOK_CLASS_UNDEFINED))
		{
            if((cmd->items[0].type==TOKEN_ITEM &&
               TOK_RETURN_TYPE[cmd->items[0].tok.potentional_types[0].type_class][cmd->items[0].tok.potentional_types[0].type_num]
               (&cmd->items[0].tok, &class1, &type1) && class1!=TOK_CLASS_UNDEFINED) ||
               (cmd->items[0].type==CMD_ITEM &&
               CMD_RETURN_TYPE[cmd->items[0].cmd.cmd_class][cmd->items[0].cmd.cmd_type](&cmd->items[0].cmd,&class1, &type1) &&
                class1!=TOK_CLASS_UNDEFINED) ||
               (cmd->items[0].type==PAREN_ITEM &&
                PAREN_RETURN_TYPE[cmd->items[0].paren.type](&cmd->items[0].paren, &class1, &type1) && class1!=TOK_CLASS_UNDEFINED))
            {
                if (!(class1==class2 && type1==type2))
                {
                     if(cmd->items[0].type==TOKEN_ITEM)
                        print_err(CODE7_TYPES_NOT_MATCH_BOTH_IDENT, &cmd->items[0].tok);
                     if(cmd->items[0].type==CMD_ITEM)
                       print_err(CODE7_TYPES_NOT_MATCH_BOTH_IDENT, (token *)inf_get_last_token(&cmd->items[0].cmd));
                     if(cmd->items[0].type==PAREN_ITEM)
                        print_err(CODE7_TYPES_NOT_MATCH_BOTH_IDENT, (token *)inf_get_parens_last_token(&cmd->items[0].paren));
                }
            }
            else
            {
                 if(cmd->items[0].type==TOKEN_ITEM)
                    print_err(CODE7_TYPES_NOT_MATCH_LEFT_DATA, &cmd->items[0].tok);
                 if(cmd->items[0].type==CMD_ITEM)
                   print_err(CODE7_TYPES_NOT_MATCH_LEFT_DATA, (token *)inf_get_last_token(&cmd->items[0].cmd));
                 if(cmd->items[0].type==PAREN_ITEM)
                    print_err(CODE7_TYPES_NOT_MATCH_LEFT_DATA, (token *)inf_get_parens_last_token(&cmd->items[0].paren));
            }
		}
		else
        {
            if(cmd->items[2].type==TOKEN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, &cmd->items[2].tok);
            if(cmd->items[2].type==CMD_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, (token *)inf_get_last_token(&cmd->items[2].cmd));
            if(cmd->items[2].type==PAREN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, (token *)inf_get_parens_last_token(&cmd->items[2].paren));
        }
    }
    // KOMANDANYN CHEPE BAGLANMA GORNUSHI UCHIN

    CUR_PART = prev_part;
    return 1;
}




/** Faýla degişli kody C koda ýazýar
**/
void cmd_assign_c_code(command *cmd, char **l, int *llen)
{
    // Çepe baglanma:
    if (cmd->items[1].tok.potentional_types[0].type_num==LEFT_ASSIGN_TOK_NUM)
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
        if (cmd->items[0].type==CMD_ITEM)
        {
            command *c = &cmd->items[0].cmd;
            CMD_GET_C_CODE[c->cmd_class][c->cmd_type](c, l, llen);
        }
        else if (cmd->items[0].type==TOKEN_ITEM)
        {
            token *t = &cmd->items[0].tok;
            TOK_GET_C_CODE[t->potentional_types[0].type_class][t->potentional_types[0].type_class](t, l, llen);
        }

        // baglanma ülňiniň c dili üçin warianty goýulýar
        char *assign_c = " = ";

        *llen += strlen(assign_c);
        *l = realloc(*l, *llen);

        strncat(*l,assign_c,strlen(assign_c));

        // üçünji ülňi maglumat.
        // Üçünji birlik identifikator bolsa, özüni geçirmeli.
        if (cmd->items[2].type==TOKEN_ITEM && cmd->items[2].tok.potentional_types[0].type_class==TOK_CLASS_IDENT)
        {
            char *rvalue = cmd->items[2].tok.potentional_types[0].value;

            *llen += strlen(rvalue);
            *l = realloc(*l, *llen);

            strncat(*l,rvalue,strlen(rvalue));
        }
        // Üçünji birlik, konstanta maglumat bolsa, onda gerekli funksiýa arkaly maglumat içine salynýar.
        else if (cmd->items[2].type==TOKEN_ITEM && cmd->items[2].tok.potentional_types[0].type_class==TOK_CLASS_CONST_DATA)
        {
            char *rvalue = get_const_data_string(&cmd->items[2].tok);

            *llen += strlen(rvalue);
            *l = realloc(*l, *llen);

            strncat(*l,rvalue,strlen(rvalue));
        }
        else if (cmd->items[2].type==CMD_ITEM)
        {
            CMD_GET_C_CODE[cmd->items[2].cmd.cmd_class][cmd->items[2].cmd.cmd_type](&cmd->items[2].cmd, l, llen);
        }
    }
}

