/*
 * 2. Kodly faylyn harplary boyuncha gechip token emele getiryar
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tpl.h"
#include "parser.h"
#include "tokens.h"
#include "token/harpl.h"
#include "pragma/pragma.h"
#include "main/inf.h"
#include "parenthesis.h"
#include "algor.h"
#include "translator_to_c.h"
#include "error.h"

char PARSER_DEFAULT_MODE = 0;
char STRING_PREPARE_MODE = 2;
char STRING_MODE         = 3;
char PARENTHESIS_MODE    = 4;

int parse(FILE *source)
{
	int prev_part = CUR_PART;
	CUR_PART = 2;

    // Adaty parser komandalary saýgarýar
	//printf("Parsinge başlandy\n");
	while((CUR_CHAR=fgetc(source))!=EOF)
	{
	    // Maglumatlar üçin
	    update_inf();

	    if(!is_valid_char())
            print_err(CODE2_UNKNOWN_TOKENS_CHAR, &inf_tok);


        // 2. Pragma modyna geçmeli
        if(CUR_CHAR==PRAGMA_START_CHAR)
        {
            parse_pragma(source);
        }
        else if (CUR_CHAR==PARENTHESIS_OPEN)
        {
            parenthesis par = parse_paren(source);
            cmd_add_item(&cmd, PAREN_ITEM, par, get_empty_cmd(), get_empty_tok());

        }
        else if (isspace(CUR_CHAR))
        {
            continue;
        }
        else if (CUR_CHAR==HARPL_OPENER )
        {
            token tok = parse_string(source);
            if (!work_with_token(&tok, &cmd))
            {
                // TODO
                // Yalnyshlyk peyda boldy, komanda tokeni goshup bolmady
            }
        }
        else if (CUR_CHAR==CMD_END)
        {
            //work_with_token(&tok, prev_tok_string);
            work_with_cmd();
            init_cmd(&cmd, 0);
        }
        else
        {
            CUR_CMD = &cmd;
            token tok = parse_token(source);
            // Komanda goşulýar
            if (!work_with_token(&tok, &cmd))
            {
                // TODO
                // Yalnyshlyk peyda boldy, komanda tokeni goshup bolmady
            }
        }

	}
	// Eger token bar bolsa, diymek komanda salynmandyr
	if (cmd.items_num)  print_err(CODE2_REMAIN_TOKEN, (token *)inf_get_last_token(&cmd));

	// TRANSLATOR TO C: algoritmi faýla ýazýar
	work_with_translator('1');

	free_locals();
	CUR_PART = prev_part;
    CUR_LINE = 1;
    CUR_CHAR_POS = 1;
    return 1;
}


/** TPL programma diliniň goldaýan harpymy ýa däldigini barlaýar **/
int is_valid_char()
{
    return (isspace(CUR_CHAR) ||                    // boshluk
        isalnum(CUR_CHAR) ||                        // harp we san
        CUR_CHAR==CMD_END ||                        // .
        CUR_CHAR==PRAGMA_START_CHAR ||              // #
        CUR_CHAR==PARENTHESIS_OPEN ||               // (; Ýaýyň açyjysy
        CUR_CHAR==PARENTHESIS_ELEM_SEPARATOR ||     // ,; Skobkanyň içindäki elementleri aýyryjy
        CUR_CHAR==PARENTHESIS_CLOSE ||              // ); Ýaýyň ýapyjysy
        CUR_CHAR==HARPL_OPENER ||                   // "
        CUR_CHAR==LEFT_ASSIGN_TOKEN_VALUE[0] ||     // <
        CUR_CHAR==CHAR_CONST_DATA_OPENER ||         // '
        CUR_CHAR==CHAR_MINUS ||                     // -
        CUR_CHAR==CHAR_UNDERSCORE ||                // _; drob sanlar başlap bilýär
        CUR_CHAR==GLOB_IDENT_OPENER ||              // @; global ülňileriň birinji harpy
        CUR_CHAR==ARIF_PLUS_CHAR  ||                /// ARIFMETIKI OPERATORLAR: +
        CUR_CHAR==ARIF_MINUS_CHAR ||                // -; aýyrmak
        CUR_CHAR==ARIF_MULTI_CHAR ||                // *; köpeltmek
        CUR_CHAR==ARIF_DIV_CHAR   ||                // :; bölmek
        CUR_CHAR==CMP_EQ_CHAR   ||                  /// DEŇEŞDIRME OPERATORLAR: =
        CUR_CHAR==CMP_LT_CHAR   ||                  // <; kiçi
        CUR_CHAR==CMP_GT_CHAR   ||                  // >; uly
        CUR_CHAR==LOGIC_AND_CHAR  ||                /// LOGIKI OPERATORLAR: &
        CUR_CHAR==LOGIC_OR_CHAR   ||                // ?; ýa
        CUR_CHAR==LOGIC_NOT_CHAR);                  // !; däl bolsa
}
