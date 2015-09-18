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
					// Komanda saygarylyp showly gutardy
					if (recognize_cmd(&cmd))
					{
						
						//printf("Komanda showly saygaryldy\n");
	
						if (is_glob_def_var_cmd(&cmd))
						{
							// Komandany global funksiyalara goshjak bolyas
							glob_vars_add_cmd(cmd);
							//printf("global ulni yglan edilipdir\n");
						}
						else if (is_def_var_cmd(&cmd))
						{
							//printf("Lokal ulni yglan edilipdir\n");
							//debug_cmd(&cmd);
							loc_vars_add_cmd(cmd);
						}
						else
						{
							// Komandany algoritme goshulyar
							algor_add_cmd(cmd);
							//printf("Komanda algoritme goshuldy\n");
						}
						
						init_cmd(&cmd);
						init_token(&tok);
					}
					else
					{
						print_err(CODE4_CANT_IDENT_CMD);
					}
						
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
	char f_name[MAX_FILE_LEN] = {0};// = cur_parse_file_name;
	strncpy(f_name, cur_parse_file_name, strlen(cur_parse_file_name)+1);

	remove_dirnames(f_name);
	remove_ext(f_name, ".tepl");

	
	
	// TODO. Eger faylyn ady fayllaryn sanawynda eyyam bar bolsa
	//		Onda yalnyshlyk chykarylyar.
	
	// .h faylyn boljak adresi
	char h_path[MAX_FILE_LEN] = {0};
	strncat(h_path, C_SOURCE_FOLDER, strlen(C_SOURCE_FOLDER));
	strncat(h_path, "\\", strlen("\\"));
	strncat(h_path, f_name, strlen(f_name));
	strncat(h_path, ".h", strlen(".h"));

	// .c faylyn boljak adresi
	char c_path[MAX_FILE_LEN] = {0};
	strncat(c_path, C_SOURCE_FOLDER, strlen(C_SOURCE_FOLDER));
	strncat(c_path, "\\", strlen("\\"));
	strncat(c_path, f_name, strlen(f_name));
	strncat(c_path, ".c", strlen(".c"));

	// Fayllar achylyar:
	FILE *c_source = fopen(c_path, "w");
	FILE *h_source = fopen(h_path, "w");
		
	// Faylyn ichine maglumat yazylyar:
	//printf("%s-%s-\n\n", cur_parse_file_name);
	//printf("%s\n", f_name);
	
	prepare_h_source(h_source, f_name);
	prepare_c_source(c_source, strncat(f_name, ".h", strlen(".h")));
	
	if (glob_def_vars_cmds && is_glob_def_var_in_cur())
	{
		//printf("Global ulni yglan edilipdir\n");
		c_trans_header_add_glob_def_var(h_source);
		c_trans_source_add_glob_def_var(c_source);
	}
	
	// Bash fayl bolsa, bash fayla degishli maglumatlar salynyar
	if (strlen(main_file_name)==strlen(cur_parse_file_name) &&
	    !strncmp(main_file_name, cur_parse_file_name, strlen(main_file_name)))
	{
		main_file = 1;
		// Bash sahypada esasy funksiya achylyar.
		prepare_main_func(c_source);
	}
	
	// Lokal yglan edilen funksiyalar
	c_trans_source_add_loc_def_var(c_source, main_file);
	
	if (main_file)
		finishize_main_func(c_source);
	
	finishize_h_source(h_source);
	
	// Fayllar yapylyar
	fclose(c_source);
	fclose(h_source);
	
	free_locals();
	
	CUR_PART = prev_part;
}
