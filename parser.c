/*
 * 2. Kodly faylyn harplary boyuncha gechip token emele getiryar
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "parser.h"
#include "tokens.h"
#include "token/harpl.h"
#include "pragma/pragma.h"
#include "algor.h"
#include "translator_to_c.h"
#include "error.h"

char PARSER_DEFAULT_MODE = 0;
char STRING_PREPARE_MODE = 2;
char STRING_MODE         = 3;

int parser(FILE *source)
{
	int prev_part = CUR_PART;
	CUR_PART = 2;

	// Baş faýly parsin edilýär
	char main_file=0;

    // tok_num - parsing edilip durka, komanda goshmaly tokenin nomerini ýatda saklamana gerek
	int c, i, tok_num = 0, c_pos=0, line=0, tmp_c, tmp_c_pos, tmp_line, tmp_file;

	// Modalar: parsering name kody yzarlayanlygyny barlayar
	// Ulni, kodda has dushnikli bolar yaly ulanylyar.
	char mode = PARSER_DEFAULT_MODE;

	// Adaty parseriň modynda, tokenleri saýgarmak üçin.
	char prev_tok_string[CONST_MAX_TOKEN_LEN] = {0};
	char new_tok_string [CONST_MAX_TOKEN_LEN] = {0};
	// Bular bolsa, tokenler saýgarylanda, ýatda saklamak üçin
	token tok;      init_token(&tok);
	token new_tok;  init_token(&new_tok);
    token string_tok;

	// Parseriň Pragma modynda, pragmalary saýgarmak üçin
	pragma prev_prag; init_pragma(&prev_prag);
	pragma prag;	  init_pragma(&prag);


	//printf("Parsinge başlandy\n");
	while((c=fgetc(source))!=EOF)
	{
	    // Maglumatlar üçin
	    if (c=='\n')
            inf_next_line(&c_pos, &line);
        else
            inf_next_char(c, &c_pos, &line);

		if (mode==PARSER_DEFAULT_MODE)
		{
		    //printf("Öňki harplar täze harp bilen tokeni emele getirenok.\n");
            empty_token(&new_tok);
            empty_string(new_tok_string, CONST_MAX_TOKEN_LEN);

            // 2. Pragma modyna geçmeli
            if      (c==PRAGMA_START_CHAR && (mode = PRAGMA_MODE)+1)
            {
                //work_with_token(&tok, prev_tok_string);
                //work_with_token(&new_tok, prev_tok_string);
                continue;
            }
            else if (isspace(c))
            {
                work_with_token(&tok, prev_tok_string);
                continue;
            }
            else if (c==HARPL_OPENER && (mode = STRING_PREPARE_MODE)+1)
            {
                // Täze token üçinem ýer arassalanýar
                work_with_token(&tok, prev_tok_string);
                work_with_token(&new_tok, new_tok_string);
                continue;
            }
            else if (c==CMD_END)
            {
                work_with_token(&tok, prev_tok_string);
                work_with_cmd();

                init_cmd(&cmd, 1);

                continue;
            }
            else
            {
                if (strlen(prev_tok_string)+1 >= CONST_MAX_TOKEN_LEN)
                    print_err(CODE2_TOKEN_TOO_BIG, &tok);

                // Häzirki harp öňki tokeniň yzy bolsa
                if ( strstrchcat(new_tok_string, prev_tok_string, c) &&
                     !recognize_token(&new_tok, new_tok_string))
                {

                    // 1. Onki token bar
                    work_with_token(&tok, prev_tok_string);

                    // Mümkin harp, täze tokeniň başydyr.
                    if (!strlen(prev_tok_string) &&
                        set_token_word_value(new_tok_string, c) &&
                        recognize_token(&new_tok, new_tok_string))	// 0 = O
                    {
                        inf_add_to_token(&new_tok, c, c_pos, line);
                        tmp_c = new_tok.inf_char_num>0 ? new_tok.inf_char : -1;
                        tmp_c_pos = new_tok.inf_char_num>0 ? new_tok.inf_char_num : -1;
                        tmp_line = new_tok.inf_char_num>0 ? new_tok.inf_line_num : -1;
                        tmp_file = new_tok.inf_char_num>0 ? new_tok.inf_file_num : -1;

                        tok = new_tok;

                        // Tokene maglumatlary goýmaly
                        if (tmp_c_pos<1)
                            inf_add_to_token(&new_tok, c, c_pos, line);
                        else
                        {
                            new_tok.inf_char     = tmp_c;
                            new_tok.inf_char_num = tmp_c_pos;
                            new_tok.inf_line_num = tmp_line;
                            new_tok.inf_file_num = tmp_file;
                        }
                    }
                    else
                    {
                        print_err(CODE2_UNKNOWN_TOKENS_CHAR, &new_tok);
                    }
				}
                else
                {
                    inf_add_to_token(&new_tok, c, c_pos, line);
                    tmp_c = new_tok.inf_char_num>0 ? new_tok.inf_char : -1;
                    tmp_c_pos = new_tok.inf_char_num>0 ? new_tok.inf_char_num : -1;
                    tmp_line = new_tok.inf_char_num>0 ? new_tok.inf_line_num : -1;
                    tmp_file = new_tok.inf_char_num>0 ? new_tok.inf_file_num : -1;

                    empty_string(prev_tok_string, CONST_MAX_TOKEN_LEN);
                    strncpy(prev_tok_string, new_tok_string, strlen(new_tok_string)+1);

                    empty_string(new_tok_string, CONST_MAX_TOKEN_LEN);
                    tok = new_tok;
                    init_token(&new_tok);
                    // Tokene maglumatlary goýmaly
                    if (tmp_c_pos<1)
                        inf_add_to_token(&new_tok, c, c_pos, line);
                    else
                    {
                        new_tok.inf_char = tmp_c;
                        new_tok.inf_char_num = tmp_c_pos;
                        new_tok.inf_line_num = tmp_line;
                        new_tok.inf_file_num = tmp_file;
                    }
                }
            }
		}
		else if (mode==PRAGMA_MODE)
		{
		    if (work_with_pragma(&prag, &prev_prag, &mode, c, c_pos, line)==2)
                continue;
		}
		else if (mode==STRING_PREPARE_MODE)
		{
            prepare_to_work_with_string(&string_tok, &mode, c, c_pos, line);
		}
		else if (mode==STRING_MODE)
		{
            work_with_string(&string_tok, &mode, c);
		}
	}
	// Eger token bar bolsa, diymek komanda salynmandyr

	if (tok.potentional_types_num || cmd.items_num)  print_err(CODE2_REMAIN_TOKEN, &string_tok);

	// TRANSLATOR TO C: algoritmi faýla ýazýar
	work_with_translator('1');

	free_locals();
	CUR_PART = prev_part;

}
