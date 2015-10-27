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
	char PARSER_DEFAULT_MODE = 0;
	char STRING_PREPARE_MODE = PRAGMA_MODE+1;
	char STRING_MODE         = STRING_PREPARE_MODE+1;
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
	    if (c=='\n')
            inf_next_line(&c_pos, &line);
        else
            inf_next_char(c, &c_pos, &line);

		if (mode==PARSER_DEFAULT_MODE)
		{
			// tokenin sozi 32 harpdan duryp bilyar
			if (strlen(prev_tok_string)+1 >= CONST_MAX_TOKEN_LEN) print_err(CODE2_TOKEN_TOO_BIG, &tok);

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
				    move_to_cmd(&tok);
				    empty_string(prev_tok_string, strlen(prev_tok_string));
				}

				// 2. Pragma modyna geçmeli
				if (c==PRAGMA_START_CHAR && (mode = PRAGMA_MODE)+1)  continue;

				// 3. Boş symwollara seredilenok seredilenok
				if (isspace(c))  continue;

				// 4. Mümkin harp, täze tokeniň başydyr.
				set_token_word_value(new_tok_string, c);
				if (recognize_token(&new_tok, new_tok_string))	// 0 = O
				{
				    inf_add_to_token(&new_tok, c, c_pos, line);
                    tmp_c = new_tok.inf_char_num>0 ? new_tok.inf_char : -1;
                    tmp_c_pos = new_tok.inf_char_num>0 ? new_tok.inf_char_num : -1;
                    tmp_line = new_tok.inf_char_num>0 ? new_tok.inf_line_num : -1;
                    tmp_file = new_tok.inf_char_num>0 ? new_tok.inf_file_num : -1;

					tok = new_tok;

					empty_token(&new_tok);
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
				else if (c==HARPL_OPENER)
                {
                    mode = STRING_PREPARE_MODE;
                }
				else if (c==CMD_END)
				{

                    work_with_cmd();

					//debug_cmd(&cmd);
					init_cmd(&cmd, 1);

					init_token(&tok);

				}
				else
				{
					//printf("harp: %c", c);
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
                //debug_token(&new_tok);
			}
		}
		else if (mode==PRAGMA_MODE)
		{
		    inf_add_to_token(&inf_tok, c, c_pos, line);
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
					if (strlen(prev_prag.name))  print_err(CODE2_PRAGMA_NOT_END, &inf_tok);

					mode = PARSER_DEFAULT_MODE;
					continue;
				}
				else if (isspace(c))  continue;
				else print_err(CODE2_PRAGMA_NOT_IDENT, &inf_tok);
			}
			else
			{
				//printf("pragma tanaldy, pragma '%s', gutarylanmy: %d\n", prag.name, prag.is_compl);
				prev_prag = prag;
			}
		}
		else if (mode==STRING_PREPARE_MODE)
		{
            increment_string_tokens();

            // We tokeniň ýeri arassalanýar.
            // Täze token üçinem ýer arassalanýar
		    init_token(&new_tok);
            init_token(&tok);
            empty_string(prev_tok_string, CONST_MAX_TOKEN_LEN);
            empty_string(new_tok_string,  CONST_MAX_TOKEN_LEN);

            // Taze token yasalvar

            init_token(&string_tok);
            inf_add_to_token(&string_tok, c, c_pos, line);
            string_tok.is_compl = 0;
            string_tok.type_class = TOK_CLASS_CONST_DATA;

            token_type tok_type;
            tok_type.is_compl = 0;
            tok_type.need_value = 1;
            tok_type.type_class = TOK_CLASS_CONST_DATA;
            tok_type.type_num = STRING_CONST_DATA_TOK_NUM;
            tok_type.string_value = &GLOB_STRINGS[GLOB_STRINGS_NUM-1];

            add_potentional_token_type(&string_tok, tok_type);
            // Token inisializasiya edilip bolan son,
            // token tipi we klasy, gutarylany gutarylmadyk diyip goyulyar.
            // tokenin maglumaty diyip bolsa, soz maglumata in sonky soz sanynyn adresi baglanyar

            // add_char_to_last_string(c) atly funksiya arkaly gosha dyrnak tokenin ichine goshulyar
            add_char_to_last_string(HARPL_OPENER);
            add_char_to_last_string(c);

            // eger-de token sozlem tokeni yaly tanalmasa,
            if (!is_token_string_const_data(&string_tok))
            {
                // Nadogry diyip hat chykarmaly
                print_err(CODE2_STRING_IS_WRONG, &string_tok);
            }
            else if (string_tok.potentional_types[0].is_compl==1)
            {
                // Komanda salmaly
                move_to_cmd(&string_tok);
                init_token(&string_tok);

                mode = PARSER_DEFAULT_MODE;
            }
            else
            {
                // Soz moduna bashlasanam bolyar
                mode = STRING_MODE;
            }

		}
		else if (mode==STRING_MODE)
		{
            // add_char_to_last_string(c) atly funksiya arkaly gosha dyrnak tokenin ichine goshulyar
            add_char_to_last_string(c);

            if (!is_token_string_const_data(&string_tok))
            {
                // Nadogry diyip hat chykarmaly
                print_err(CODE2_STRING_IS_WRONG, &string_tok);
            }
            else if (string_tok.potentional_types[0].is_compl==1)
            {
                // Adaty moda gechilyar.
                move_to_cmd(&string_tok);
                init_token(&string_tok);

                mode = PARSER_DEFAULT_MODE;
            }
		}
	}
	// Eger token bar bolsa, diymek komanda salynmandyr

	if (tok.potentional_types_num || cmd.items_num)  print_err(CODE2_REMAIN_TOKEN, &string_tok);

	// TRANSLATOR TO C: algoritmi faýla ýazýar
	work_with_translator('1');

	free_locals();
	CUR_PART = prev_part;

}
