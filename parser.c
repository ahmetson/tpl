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

int parser(FILE *source)
{
	int prev_part = CUR_PART;
	CUR_PART = 2;

	// Baş faýly parsin edilýär
	char main_file=0;

    // tok_num - parsing edilip durka, komanda goshmaly tokenin nomerini ýatda saklamana gerek
	int c, i, tok_num = 0;

	// Modalar: parsering name kody yzarlayanlygyny barlayar
	// Ulni, kodda has dushnikli bolar yaly ulanylyar.
	char PARSER_DEFAULT_MODE = 0;
	char mode = PARSER_DEFAULT_MODE;

	// Adaty parseriň modynda, tokenleri saýgarmak üçin.
	char prev_tok_string[CONST_MAX_TOKEN_LEN] = {0};
	char new_tok_string [CONST_MAX_TOKEN_LEN] = {0};
	// Bular bolsa, tokenler saýgarylanda, ýatda saklamak üçin
	token tok;      init_token(&tok);
	token new_tok;  init_token(&new_tok);

	// Parseriň Pragma modynda, pragmalary saýgarmak üçin
	pragma prev_prag; init_pragma(&prev_prag);
	pragma prag;	  init_pragma(&prag);


	//printf("Parsinge başlandy\n");
	while((c=fgetc(source))!=EOF)
	{
		if (mode==PARSER_DEFAULT_MODE)
		{
			// tokenin sozi 32 harpdan duryp bilyar
			if (strlen(prev_tok_string)+1 >= CONST_MAX_TOKEN_LEN) print_err(CODE2_TOKEN_TOO_BIG);

			// Öňki harplar täze harp bilen birleşdirilýär
			strstrchcat(new_tok_string, prev_tok_string, c);

			if (!recognize_token(&new_tok, new_tok_string))	// 0 = OK
			{
			    //printf("Öňki harplar täze harp bilen tokeni emele getirenok.\n");
				empty_token(&new_tok);
				empty_string(new_tok_string, CONST_MAX_TOKEN_LEN);
				// 1. Onki token bar
				if (!is_token_empty(&tok))
				{
					move_to_cmd(&tok, prev_tok_string);

				}
				//printf("Eger onki token bosh bolmasa, komanda gechirilyar\n");

				// 2. Pragma modyna geçmeli
				if (c==PRAGMA_START_CHAR && (mode = PRAGMA_MODE)+1)  continue;

				// 3. Boş symwollara seredilenok seredilenok
				if (isspace(c))  continue;

				// 4. Mümkin harp, täze tokeniň başydyr.
				set_token_word_value(new_tok_string, c);
				if (recognize_token(&new_tok, new_tok_string))	// 0 = O
				{
					tok = new_tok;
					empty_token(&new_tok);
				}
				else if (c==CMD_END)
				{
                        //printf("Komanda gutardy ++++++++++++++++++++++++++++++++++++++++=\n");
                        //debug_cmd(&cmd);

                        // Komandany algoritme goshulyar
                        work_with_cmd();
                        //if (CUR_ALGOR_ITEMS_NUM)
                        //{printf("KOMANDA BILEN ISHLENILENDEN SON-------------------------------\n");
                        //debug_cmd(&CUR_ALGOR[CUR_ALGOR_ITEMS_NUM-1]);
                       // printf("WE========================================\n");
                        //}// Komanda bilen ishleyan bolume gechilyar

					//debug_cmd(&cmd);
					init_cmd(&cmd, 1);

					init_token(&tok);

				}
				else
				{
					//printf("harp: %c", c);
					print_err(CODE2_UNKNOWN_TOKENS_CHAR);
				}
			}
			else
			{
				//printf("Token tanaldy\n");
				empty_string(prev_tok_string, CONST_MAX_TOKEN_LEN);
				strncpy(prev_tok_string, new_tok_string, strlen(new_tok_string)+1);

				empty_string(new_tok_string, CONST_MAX_TOKEN_LEN);
				tok = new_tok;
				init_token(&new_tok);
			}
		}
		else if (mode==PRAGMA_MODE)
		{
			//printf("Pragma moda gechildi. Indi parser pragmalary saygarmana chalshar.\n");
			strstrchcat(prag.name, prev_prag.name, c);

			if (!recognise_pragma(&prag))
			{
				//printf("pragma tanalmady\n");

				if (prev_prag.is_compl)
				{
					//printf("Onki pragma: '%s', gutarylanmy: %d\n", prev_prag.name, prev_prag.is_compl);
					act_pragma(&prev_prag);
                    //printf("Hawan");
					init_pragma(&prag);
					init_pragma(&prev_prag);
				}

				if (c==PRAGMA_END_CHAR)
				{
				    // Pragma tanalmady.
					if (strlen(prev_prag.name))  print_err(CODE2_PRAGMA_NOT_END);

					mode = PARSER_DEFAULT_MODE;
					continue;
				}
				else if (isspace(c))  continue;
				else print_err(CODE2_PRAGMA_NOT_IDENT);
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
	if (tok.potentional_types_num || cmd.items_num)  print_err(CODE2_REMAIN_TOKEN);

	// TRANSLATOR TO C: algoritmi faýla ýazýar
	work_with_translator('1');
	free_locals();
	CUR_PART = prev_part;

}
