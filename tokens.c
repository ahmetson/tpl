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
	   {is_token_ident}
};



// Sets default parameters
void init_token(token *tok)
{
	tok->ns = 0;												// Namespace of token
	
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
int recognize_token(token *tok, token_word *val)
{
	int i;
	for (i=0; i<TOKEN_TYPES_NUM; i++)
	{		
		tok_types[i].is_token(tok, val);
	}
	
	// Token not recognized
	if (tok->potentional_types_num==0)
	{
		//printf("Token not recognized\t(Inputted text:%s len:%d)\n", val->val, val->len);
		return 1;												// Token tanalmady
	}
	//debug_token(tok);
	//printf("Token Recognized\t(Inputted text:%s len:%d)\n", val->val, val->len);
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
**/
int move_to_cmd(token *tok, token_word *tok_word)
{
	// Komandadaky onki token gutarylan bolmaly 
	if (cmd.tokens_num!=0 && cmd.tokens[cmd.tokens_num-1].is_compl==0)
	{
		print_err(part_code, CODE3_PREV_TOK_INCORRECT);
	}

	finishize_token(tok);
				//printf("---------Finisizasiyadan son\n");
				//debug_token(&tok);
				
				//printf("Komanda goshuljak token tipi: %d, bolup biljek gornusherinin sany: %d\n",tok.type_class, tok.potentional_types_num);
				//printf("Tokenin gornushi: %d\n",tok.potentional_types[0].type_num);
				//debug_cmd(&cmd);
				//cmd.tokens[cmd.tokens_num++] = tok;
	if (!add_to_cmd(&cmd, tok))
	{
		print_err(4, CODE4_TOO_MANY_TOKENS);
	}
				//printf("Token goshulan son:\n");
				//debug_cmd(&cmd);

	empty_token(tok);
	init_token_word(tok_word);
	
	return 1;
}



