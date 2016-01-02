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
	command_item *fci = get_cmd_item(cmd->items,0);
	// Birinji token ya ulni yglan etmek bolmaly ya identifikator.
	if (fci->type==TOKEN_ITEM &&
		fci->tok.type_class==TOK_CLASS_IDENT)
	{
	    assign_cmd_mod(cmd, 0);
		next_item_class=TOK_CLASS_ASSIGN;
	}
	else if (fci->type==CMD_ITEM &&
		fci->cmd.cmd_class==CMD_CLASS_DEF_VAR)
	{
	    assign_cmd_mod(cmd, 0);
		next_item_class=TOK_CLASS_ASSIGN;
		// Indiki token dine chepe tarap baglanma bolmaly
		next_item_type= LEFT_ASSIGN_TOK_NUM;

	}
	else if (fci->type==CMD_ITEM &&
            fci->cmd.cmd_class==CMD_CLASS_CALL_GLOB_VAR)
    {
        assign_cmd_mod(cmd, 0);
		next_item_class=TOK_CLASS_ASSIGN;
		// Indiki token dine chepe tarap baglanma bolmaly
		next_item_type= LEFT_ASSIGN_TOK_NUM;
    }
    else if (fci->type==CMD_ITEM &&
            fci->cmd.cmd_class==CMD_CLASS_ARR && fci->cmd.cmd_type==CMD_CLASS_ARR_CON)
    {
        assign_cmd_mod(cmd, 0);
		next_item_class=TOK_CLASS_ASSIGN;
		// Indiki token dine chepe tarap baglanma bolmaly
		next_item_type= LEFT_ASSIGN_TOK_NUM;
    }
	/*else if (fci->type==TOKEN_ITEM &&
          fci->tok.type_class==TOK_CLASS_CONST_DATA)
	{

        //assign_cmd_mod(cmd, 0);
		//next_item_class=TOK_CLASS_ASSIGN;
		// Indiki token dine chepe tarap baglanma bolmaly
		//next_item_type= RIGHT_ASSIGN_TOK_NUM;
    }*/
    else
    {
		return 0;
	}

	// Ikinji birlik, eger birinji ulni yglan edilme bolsa,
	// Baglanmanyn chepe gidyan token gornushi bolmaly, yogsa ikisem bolyar
	if(cmd->items_num>1)
	{
	    command_item *sci = get_cmd_item(cmd->items,1);
		if (next_item_class>0 && sci->type==TOKEN_ITEM && sci->tok.type_class==TOK_CLASS_ASSIGN)
		{
			// Token dine chepe tarap baglanma bolmaly
			if (next_item_type>0 && !sci->tok.potentional_types[0].type_num==next_item_type)
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
	    command_item *tci = get_cmd_item(cmd->items,2); // tci - third command item
	    int ret_class = -1, ret_type = -1;
	    if((tci->type==TOKEN_ITEM &&
		   return_tok_type(&tci->tok, &ret_class, &ret_type) && ret_class!=TOK_CLASS_UNDEFINED) ||
		   (tci->type==CMD_ITEM && CMD_RETURN_TYPE[tci->cmd.cmd_class][tci->cmd.cmd_type](&tci->cmd,&ret_class, &ret_type) &&
                ret_class!=TOK_CLASS_UNDEFINED) ||
           (tci->type==PAREN_ITEM &&
                PAREN_RETURN_TYPE[tci->paren.type](&tci->paren, &ret_class, &ret_type) && ret_class!=TOK_CLASS_UNDEFINED))
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
void assign_cmd_mod(command *cmd, int tok_num)
{
	// Birinji birlik
	if (tok_num==0)
	{
	    command_item *fci = get_cmd_item(cmd->items, 0);
		cmd->cmd_class = CMD_CLASS_ASSIGN;
		cmd->is_compl = 0;

		// Eger birlik global ulni yglan etme bolsa
		// Bu token in sonky. Shonun uchin lokalmy ya yokdugyny barlap bolyar
		if (!(fci->type==CMD_ITEM && is_glob_def_var_cmd(&fci->cmd)) )
		{
			cmd->ns = LOCAL;
		}
	}
	else if (tok_num==1)
	{
	    command_item *sci = get_cmd_item(cmd->items, 1);
		cmd->cmd_type = sci->tok.potentional_types[0].type_num-1;
		cmd->cmd_class = CMD_CLASS_ASSIGN;

		cmd->is_compl = 0;
	}
	else if (tok_num==2)
	{
		cmd->cmd_class = CMD_CLASS_ASSIGN;
		cmd->is_compl = 1;

        // TODO, eger birinji birlik maglumat bolup ikinjisi identifikator bolsa.
	}
}

/** Baglama komandasynyň semantikasy **/
int semantic_cmd_assign(command *cmd)
{
    int prev_part = CUR_PART;
    CUR_PART = 7;
    command_item *sci = get_cmd_item(cmd->items,1);
    // KOMANDANYN CHEPE BAGLANMA GORNUSHI UCHIN
    if (sci->tok.potentional_types[0].type_num==LEFT_ASSIGN_TOK_NUM)
    {
        command_item *fci = get_cmd_item(cmd->items,0);
        if (fci->type==TOKEN_ITEM && fci->tok.type_class==TOK_CLASS_IDENT)
        {
            token *item = &fci->tok; // Gysgaltmak uchin ulanlyar
            if (!is_local_var_def_exist(item->potentional_types[0].value))
            {
                print_err(CODE7_LEFT_IDENT_NOT_DEFINED, item);
            }
        }
        else if (fci->type==CMD_ITEM)
        {
            check_semantics(&fci->cmd);
        }

        command_item *tci = get_cmd_item(cmd->items,2);
        // IKINJI BIRLIGIN BARLANMASY
        //      eyyam yglan edilen ulnin identifikatory bolsa - identifikator hokman on bir yerde yglan edilmeli
        if (tci->type==TOKEN_ITEM && tci->tok.type_class==TOK_CLASS_IDENT)
        {
            token *item = &tci->tok; // Gysgaltmak uchin ulanlyar

            if (!is_local_var_def_exist(item->potentional_types[0].value))
            {
                print_err(CODE7_RIGHT_IDENT_NOT_DEFINED, item);
            }
        }
        else if (tci->type==CMD_ITEM)
        {
            check_semantics(&tci->cmd);
        }


        /// Ikinji masagaraçylyk: ikisiniňem tipleri biri birine gabat gelmeli
        command_item *ci = tci;
        int class1 = -1, type1 = -1, class2 = -1, type2 = -1;
	    if((ci->type==TOKEN_ITEM &&
		   return_tok_type(&ci->tok, &class2, &type2) && class2!=TOK_CLASS_UNDEFINED) ||
		   (ci->type==CMD_ITEM &&
           CMD_RETURN_TYPE[ci->cmd.cmd_class][ci->cmd.cmd_type](&ci->cmd,&class2, &type2) &&
        class2!=TOK_CLASS_UNDEFINED) ||
           (ci->type==PAREN_ITEM &&
            PAREN_RETURN_TYPE[ci->paren.type](&ci->paren, &class2, &type2) && class2!=TOK_CLASS_UNDEFINED))
		{
		    command_item *f = get_cmd_item(cmd->items,0);
            if((f->type==TOKEN_ITEM &&
               return_tok_type(&f->tok, &class1, &type1) &&
                class1!=TOK_CLASS_UNDEFINED) ||
               (f->type==CMD_ITEM &&
               CMD_RETURN_TYPE[f->cmd.cmd_class][f->cmd.cmd_type](&f->cmd,&class1, &type1) &&
                class1!=TOK_CLASS_UNDEFINED) ||
               (f->type==PAREN_ITEM &&
                PAREN_RETURN_TYPE[f->paren.type](&f->paren, &class1, &type1) && class1!=TOK_CLASS_UNDEFINED))
            {
                if (!(class1==class2 && type1==type2))
                {
                     printf("BUNAME\n");
                     if(f->type==TOKEN_ITEM)
                        print_err(CODE7_TYPES_NOT_MATCH_BOTH_IDENT, &f->tok);
                     if(f->type==CMD_ITEM)
                       print_err(CODE7_TYPES_NOT_MATCH_BOTH_IDENT, (token *)inf_get_last_token(&f->cmd));
                     if(f->type==PAREN_ITEM)
                        print_err(CODE7_TYPES_NOT_MATCH_BOTH_IDENT, (token *)inf_get_parens_last_token(&f->paren));
                }
            }
            else
            {
                printf("CHAP EDIL\n");
                 if(f->type==TOKEN_ITEM)
                    print_err(CODE7_TYPES_NOT_MATCH_LEFT_DATA, &f->tok);
                 if(f->type==CMD_ITEM)
                   print_err(CODE7_TYPES_NOT_MATCH_LEFT_DATA, (token *)inf_get_last_token(&f->cmd));
                 if(f->type==PAREN_ITEM)
                    print_err(CODE7_TYPES_NOT_MATCH_LEFT_DATA, (token *)inf_get_parens_last_token(&f->paren));
            }
		}
		else
        {
            if(ci->type==TOKEN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, &ci->tok);
            if(ci->type==CMD_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, (token *)inf_get_last_token(&ci->cmd));
            if(ci->type==PAREN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, (token *)inf_get_parens_last_token(&ci->paren));
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
    command_item *first =  get_cmd_item(cmd->items,0);
    command_item *second = get_cmd_item(cmd->items,1);
    command_item *third =  get_cmd_item(cmd->items,2);

    if (second->tok.potentional_types[0].type_num==LEFT_ASSIGN_TOK_NUM)
    {
        // Eger birinji birlik ülňi yglan etmek bolsa, komandanyň içinden tokeniň ady alynýar
        // Eger birinji ülňi identifikator bolsa, özi alynýar.
        if (first->type==CMD_ITEM)
        {
            command *c = &first->cmd;
            CMD_GET_C_CODE[c->cmd_class][c->cmd_type](c, l, llen);
        }
        else if (first->type==TOKEN_ITEM)
        {
            token *t = &first->tok;
            TOK_GET_C_CODE[t->potentional_types[0].type_class][t->potentional_types[0].type_num](t, l, llen);
        }

        // baglanma ülňiniň c dili üçin warianty goýulýar
        char *assign_c = " = ";

        *llen += strlen(assign_c);
        *l = realloc(*l, *llen);

        strncat(*l,assign_c,strlen(assign_c));

        // üçünji ülňi maglumat.
        // Üçünji birlik identifikator bolsa, özüni geçirmeli.
        if (third->type==TOKEN_ITEM && third->tok.potentional_types[0].type_class==TOK_CLASS_IDENT)
        {
            char *rvalue = third->tok.potentional_types[0].value;

            *llen += strlen(rvalue);
            *l = realloc(*l, *llen);

            strncat(*l,rvalue,strlen(rvalue));
        }
        // Üçünji birlik, konstanta maglumat bolsa, onda gerekli funksiýa arkaly maglumat içine salynýar.
        else if (third->type==TOKEN_ITEM && third->tok.potentional_types[0].type_class==TOK_CLASS_CONST_DATA)
        {
            char *rvalue = get_const_data_string(&third->tok);

            *llen += strlen(rvalue);
            *l = realloc(*l, *llen);

            strncat(*l,rvalue,strlen(rvalue));
        }
        else if (third->type==CMD_ITEM)
        {
            CMD_GET_C_CODE[third->cmd.cmd_class][third->cmd.cmd_type](&third->cmd, l, llen);
        }
        else if (third->type==PAREN_ITEM)
        {
            paren_get_c_code(&third->paren, l, llen);
        }
    }
}

