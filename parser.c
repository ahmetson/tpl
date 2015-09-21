/*
 * 2. Kodly faylyn harplary boyuncha gechip token emele getiryar
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "parser.h"
#include "tokens.h"
#include "pragma/pragma.h"
#include "algor.h"
#include "translator_to_c.h"
#include "error.h"

#define MAX_TOK_LEN 32

int parser(FILE *source)
{
	// Hazir TPL-in fayly parsing edyan boluminde
	int prev_part = CUR_PART;
	CUR_PART = 2;
	char main_file=0;

	int c, i, tok_num = 0;

	// Modalar: parsering name kody yzarlayanlygyny barlayar
	// Ulni, kodda has dushnikli bolar yaly ulanylyar.
	char PARSER_DEFAULT_MODE = 0;
	char mode = PARSER_DEFAULT_MODE;

	// Adaty komandalary saygarmaly bolanda, parserin ishlejek ulnileri
	char prev_tok_string[MAX_TOK_LEN] = {0};    
	char new_tok_string[MAX_TOK_LEN] = {0};

	token tok;
		init_token(&tok);
	token new_tok;
		init_token(&new_tok);
	
	loc_def_vars = malloc(loc_def_vars_size);
		
	// Pargma modunda, parserin ishlejek ulnileri
	pragma prev_prag; init_pragma(&prev_prag);
	pragma prag;	  init_pragma(&prag);

	while((c=fgetc(source))!=EOF) 
	{
		
		if (mode==PARSER_DEFAULT_MODE)
		{
			// tokenin sozi 32 harpdan duryp bilyar
			if (strlen(prev_tok_string)+1 >= MAX_TOK_LEN)
			{
				print_err(CODE2_TOKEN_TOO_BIG);
			}
			strstrchcat(new_tok_string, prev_tok_string, c);

			// Hazirki harp onki tokenin bolegi DAL!
			if (recognize_token(&new_tok, new_tok_string))	// 0 = OK
			{
				//printf("Token tanalmady %s\n", new_tok_string);
				empty_token(&new_tok);
				empty_string(new_tok_string, MAX_TOK_LEN);
				
				// 1. Onki token bar
				if (is_token_empty(&tok))
				{
					move_to_cmd(&tok, prev_tok_string);
				}
	
				// 2. Eger pragmanyn bashy diyen nushana bolsa
				if (c==PRAGMA_START_CHAR)
				{
					mode = PRAGMA_MODE;
					continue;
				}
				
				// 3. Boshluk harplara seredilenok
				if (isspace(c))
				{
					continue;
				}
	
				// 4. Mumkin harp taze tokenin bashydyr
				set_token_word_value(new_tok_string, c);
				if (!recognize_token(&new_tok, new_tok_string))	// 0 = O
				{
					tok = new_tok;
					empty_token(&new_tok);
				}			
				else if (c==cmd_end)
				{
					// Komanda bilen ishleyan bolume gechilyar
					work_with_cmd();
					init_cmd(&cmd);
					init_token(&tok);
				}
				else
				{
					//printf("harp: %c", c);
					print_err(CODE2_UNKNOWN_TOKENS_CHAR);
				}
			//ty_token(token);
			}
			else
			{	empty_string(prev_tok_string, MAX_TOK_LEN);
				strncpy(prev_tok_string, new_tok_string, strlen(new_tok_string)+1);
				
				empty_string(new_tok_string, MAX_TOK_LEN);
				//debug_token(&new_tok);
				tok = new_tok;
				//printf("Onki token diyilip goyulan son:\n");
				//debug_token(&tok);
				init_token(&new_tok);
			}	
		}
		else if (mode==PRAGMA_MODE)
		{
			//printf("Pragma moda gechildi. Indi parser pragmalary saygarmana chalshar.\n");
			
			strcat_ch2(&prag.name, &prev_prag.name, c);

			//printf("%s - goshuldy\n", prag.name);
			
			if (!recognise_pragma(&prag))
			{
				//printf("pragma tanalmady\n");
				
				if (prev_prag.is_compl)
				{
					//printf("Onki pragma: '%s', gutarylanmy: %d\n", prev_prag.name, prev_prag.is_compl);
					act_pragma(&prev_prag);
					
					init_pragma(&prag);
					init_pragma(&prev_prag);
				}
				
				if (c==PRAGMA_END_CHAR)
				{
					mode = PARSER_DEFAULT_MODE;
					
					// Gutarylmadyk pragma barka, nira gidyan.
					if (strlen(prev_prag.name))
					{
						//printf("Pragmada yalnyshlyk bar\n");
						print_err(CODE2_PRAGMA_NOT_END);
					}
					
					continue;
				}
				else if (isspace(c))
				{
					continue;
				}
				else
				{
					print_err(CODE2_PRAGMA_NOT_IDENT);
				}
			}
			else
			{
				//printf("pragma tanaldy, pragma '%s', gutarylanmy: %d\n", prag.name, prag.is_compl);
				prev_prag = prag;
			}
			
			
		}
	}
	// Eger token bar bolsa, diymek komanda salynmandyr
	//debug_token(&tok);
	//printf("After end of parsing\n");
	if (tok.potentional_types_num || cmd.tokens_num)
	{
		print_err(CODE2_REMAIN_TOKEN);
	}
	
	// TRANSLATOR TO C
	// Algoritmleri fayla yazylyar
	work_with_translator('1');
	
	free_locals();
	
	CUR_PART = prev_part;
}
