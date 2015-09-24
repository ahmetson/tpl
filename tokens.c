/*
Token Functions
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// TPL.H global ulnileri saklayar
#include "tpl.h"
#include "tokens.h"
#include "token/token_structs.h"
#include "dev_debug.h"
#include "error.h"

// Kompilyatoryn tokenler bolegi dordinji nomerde
static part_code = 3;


is_token_item tok_types[] = {
	   {is_token_def_type},
	   {is_token_def_glob},
	   {is_token_ident},
	   {is_token_var_left_assign}
};



// Sets default parameters
void init_token(token *tok)
{
	tok->ns = 0;							// Namespace of token
	
	int i; token_type tok_type;
	
	if (tok->potentional_types_num>=1 && tok->potentional_types_num<=TOKEN_TYPES_NUM)
		for(i=0; i<(tok->potentional_types_num); i++)
		{
			tok->potentional_types[i] = tok_type;
		}
	tok->potentional_types_num = 0;								// Number of recognized types for token
	//char source_file[MAXLEN];									// source file of token
	tok->type_class = 0;
	tok->value[0] = '\0';
	tok->value_len = 1;
	tok->is_compl = 0;
}
void empty_token(token *tok)
{
	init_token(tok);
}
// =====================================================
// TOKEN TYPE



/*
 * Recognize token types by @val token string and put it into @tok
 *
 * Returns 0, if everything is OK!
 * Otherwise returns error code.
**/
int recognize_token(token *tok, char *val)
{
	int i;
	for (i=0; i<TOKEN_TYPES_NUM; i++)
	{		
		tok_types[i].is_token(tok, val);
	}
	
	
	// Token not recognized
	if (tok->potentional_types_num==0)
	{
		//printf("Token not recognized\t(Inputted text:%s)\n", val);
		return 1;												// Token tanalmady
	}
	//debug_token(tok);
	//printf("Token Recognized\t(Inputted text:%s)\n", val);
	return 0;
}

/*
 * Tokenin bolup biljek tipini tokene goshyar.
 * Eger tokenin eyyam goshuljak gornushinin klasynda gornushi bar bolsa, 
 * we taze gornush gutarylan bolsa, onda onki gornush tazelenyar.
 *
 * 0 - eger hemme zat gowy bolsa!
 * yogsa nadogrylygyn nomeri gaytarylyar.  
**/
int add_potentional_token_type(token *tok, token_type tok_type)
{
	// Eger eyyam shu tipin klasynda bashga gornush bar bolsa, 
	// taze tip bilen chalshyrylyar
	/*if (tok_type.is_compl==1 && tok->type_class==tok_type.type_class)
	{
		int i=0;
	
		for (i=0; i<tok->potentional_types_num; i++)
		{
			if (tok->potentional_types[i].type_class==tok_type.type_class)
			{
				tok->potentional_types[i] = tok_type;
				
				return 0;
			}
			
		}
		
		// Token gornushi name uchindir tapyp bolmady
		return 1;
	}*/

	tok->potentional_types[tok->potentional_types_num++] = tok_type;
		
	return 0;

}


/*
 * Tokeni komanda goshmazdan on, goshmana tayynlayar
**/
int finishize_token(token *tok)
{
	if (tok->potentional_types_num<1)
		return 1;
	
	// dine bir bolup biljek tip goyulyar,
	// in sonky goylan fayl, onki goyulanlary yapyar
	int i;
	for (i=tok->potentional_types_num-1;i>0; i--)
	{
		if (tok->potentional_types[i].is_compl==0)
			continue;
		else
			tok->potentional_types[0] = tok->potentional_types[i];
	}
	tok->potentional_types_num = 1;
	tok->is_compl = tok->potentional_types[0].is_compl;
	
	return 0;
}

/*
 * Token boshmy ya daldigini barlayar
 *
 * Eger bosh bolsa, 0 gaytaryar,
 * yogsa tokenlerin sany gaytarylyar
**/
int is_token_empty(token *tok)
{
	//printf("\n----------Potensial: %d\n",tok->potentional_types_num);
	return tok->potentional_types_num;
}


/*
 * Tokeni komanda gechiryar. Ichini boshadyar
 * Komanda goshulan son, ichini barlayar
**/
int move_to_cmd(token *tok, char *tok_string)
{
	int prev_part = CUR_PART;
	CUR_PART = 4;

	// Komandadaky onki token gutarylan bolmaly 
	if (cmd.items_num!=0 &&
		(cmd.items[cmd.items_num-1].type==1 && cmd.items[cmd.items_num-1].tok.is_compl==0 ||
		 cmd.items[cmd.items_num-1].type==2 && cmd.items[cmd.items_num-1].cmd.is_compl==0)
		)
	{
		//printf("hawa dogry");
		print_err(CODE3_PREV_TOK_INCORRECT);
	}
	//printf("Komanda goshuljak bolyar. Token gutarylan eken\n");

	finishize_token(tok);
	//printf("Token komanda goshulmana tayynlandy\n");
	if (!add_to_cmd(&cmd, tok))
	{
		//debug_token(tok);
		//printf("Hawa dogry, gaty kan token");
		print_err(CODE4_TOO_MANY_TOKENS);
	}
				//printf("Token goshulan son:\n");
				//debug_cmd(&cmd);
	// Eger token birinji bolsa, ol komandanyn bashlayan tokenlerinden bolmaly
	// komanda-da tokenin nomeri
	if (cmd.items_num==1)
	{
		int i; char first_tok = 0;
		for (i=0; i<CMD_FIRST_ITEMS_NUM; i++)
		{
			if (cmd.items[0].type==cmd_first_items_classes[i].type &&
				cmd_first_items_classes[i].item_class==cmd.items[0].tok.type_class)
			{
				first_tok=1;
				break;
			}
			
		}
		if (!first_tok)
		{
			print_err(CODE4_CMD_HASNT_FIRST_TOKEN);
		}
	}
	//printf("komanda barlanmana gechmeli\n");
	if (!parse_cmd(&cmd))
	{
		//printf("Komanda saygaryldy\n");
		cmd.items_num--;
		if (!cmd.items_num || !parse_cmd(&cmd))
			print_err(CODE4_CANT_IDENT_CMD);
		else
		{
			// Ichki komandalaryn sanyny kopeltmeli
			inline_cmds_num++;
			inline_cmd_items = realloc(inline_cmd_items, sizeof(command_item*)*inline_cmds_num);
			if (inline_cmd_items==NULL)
			{
				//printf("Ichki komandalar uchin yer taplymady\n");
				print_err(CODE4_CANT_IDENT_CMD);
			}
			else
			{
				//printf("Ichki komandalar uchin yer bar\n");

				// Ichki komanda uchin yer.
				long size = cmd.items_num * sizeof(command_item);
				inline_cmd_items[inline_cmds_num-1] = malloc(size);

				// Ichki komanda uchin yere, birlikler gechirilyar.
				int i=0;
				for (i=0; i<cmd.items_num; ++i)
				{
					inline_cmd_items[inline_cmds_num-1][i] = cmd.items[i];
				}
				
				command new_cmd;
				new_cmd = cmd;
				new_cmd.items = inline_cmd_items[inline_cmds_num-1];
				init_cmd(&cmd, 0);
				cmd.items_num = 2;
				command_item *tmp = realloc(cmd.items, cmd.items_num*sizeof(command_item));
				if (tmp==NULL)
				{
					//printf("Komanda ichki komandany goshmak uchin yer tapylmady\n");
					print_err(CODE4_CANT_IDENT_CMD);
				}
				else
				{
					//printf("Komanda ichki komandany goshmak uchin yer bar\n");
					cmd.items = tmp;
					
					// Onki komanda goshulyar
					command_item cmd_item = {};
					cmd_item.type = 2;
					cmd_item.cmd = new_cmd;
					
					cmd.items[0] = cmd_item;
					
					// Taze token goshulyar
					command_item cmd_tok_item;
					cmd_tok_item.type = 1;
					cmd_tok_item.tok = *tok;
					cmd.items[1] = cmd_tok_item;
					//debug_cmd(&cmd);
					if (!parse_cmd(&cmd))
					{
						//printf("Taze komanda tanalmady\n");
						print_err(CODE4_CANT_IDENT_CMD);
					}
					else
					{
						//printf("Taze komanda tanaldy\n");
					}
					//debug_cmd(&cmd);
				}
			}
		}
		//new_cmd.items = malloc(sizeof(command_item)*CMD_MAX_TOKENS);
		//new_cmd.items[cmd.items_num++] = cmd;
		//   sonky token pozulyar
		//   taze komanda yasalyar
		//   onki komanda taze komandanyn birinji komandasy diyilip yazylyar
		//   hazirki token komandanyn itemi diyilip yazylyar
		//   Eger komanda saygarylmasa
		//     komanda saygarylmady diyilip yalnyshlyk gaytarylyar.
		
	}else
	{
		//printf("Komanda saygaryldy\n");
	}
	//if (!parse_cmd(cmd))
		{
			//printf("Komanda saygarylmady\n");
			//debug_cmd(cmd);
			//init_token(cmd->tokens_num--;
		}
		
		
		

		//printf("hawa");

	empty_token(tok);
	//printf("Token boshadyldy\n");
	empty_string(tok_string, strlen(tok_string));
	
	CUR_PART = prev_part;
	return 1;
}



