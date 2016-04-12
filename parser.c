/** 2. Kodly faylyn harplary boyuncha gechip token emele getiryar*/

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
#include "main/user_def_type.h"
#include "parenthesis.h"
#include "algor.h"
#include "semantic.h"
#include "translator_to_c.h"
#include "error.h"
#include "dev_debug.h"

wchar_t PARSER_DEFAULT_MODE = 0;
wchar_t STRING_PREPARE_MODE = 2;
wchar_t STRING_MODE         = 3;
wchar_t PARENTHESIS_MODE    = 4;

int parse(FILE *source)
{
	int prev_part = CUR_PART;
	CUR_PART = 2;

    /** The reaseon why loop is endless - Character that must be recognized
        can be also previous one. There for in some situations getting character
        from file doesnt required.*/
	while( 1 )
	{
	    /** Indiki harpy alýar.
            Parser boýunça umumy kömek üçin gerek bolan maglumatlary üýtgedýär*/
	    if ( !process_char( source, CHECK_VALID_CHAR ) )
            break;

        if ( !iswspace( CUR_CHAR ) &&
             !parser_mode_pragma( source ) &&
             !parser_mode_paren( source, &cmd ) &&
             !parser_mode_string( source, &cmd ) &&
             !parser_mode_end_cmd( source ) )
        {
            /** PARSERIŇ TOKEN MODASYNDA */
            CUR_CMD = &cmd;
            token tok = parse_token( source );

            work_with_token( &tok, &cmd );

            if (tok.type_class==TOK_CLASS_TRIANGLE_BLOCK && tok.potentional_types[0].type_num==TOKEN_TRIANGLE_BLOCK_OPEN_TYPE)
            {
                parse_triangle_block_inside(source);
            }
        }

	}

	check_semantics_after_parsing(&cmd);

	/// TRANSLATOR TO C: algoritmi faýla ýazýar
	work_with_translator();

    free_inf();
	free_locals();
	CUR_PART = prev_part;

    return 1;
}


/** TPL programma diliniň goldaýan harpymy ýa däldigini barlaýar **/
int is_valid_wchar_t()
{
    return (iswspace(CUR_CHAR) ||                                // boshluk
        iswalnum(CUR_CHAR) ||                                    // harp we san
        CUR_CHAR==CMD_END ||                                    // .
        CUR_CHAR==PRAGMA_START_CHAR ||                          // #
        CUR_CHAR==PARENTHESIS_OPEN ||                           // (; Ýaýyň açyjysy
        CUR_CHAR==PARENTHESIS_ELEM_SEPARATOR ||                 // ,; Skobkanyň içindäki elementleri aýyryjy
        CUR_CHAR==PARENTHESIS_CLOSE ||                          // ); Ýaýyň ýapyjysy
        CUR_CHAR==HARPL_OPENER ||                               // "
        CUR_CHAR==TOK_CLASS_ASSIGN_CHARS[0][0][0] ||               // <
        CUR_CHAR==TOK_CLASS_ASSIGN_CHARS[1][0][1] ||               // >
        CUR_CHAR==CHAR_CONST_DATA_OPENER ||                     // '
        CUR_CHAR==CHAR_MINUS ||                                 // -
        CUR_CHAR==CHAR_UNDERSCORE ||                            // _; drob sanlar başlap bilýär
        CUR_CHAR==GLOB_IDENT_OPENER ||                          // @; global ülňileriň birinji harpy
        CUR_CHAR==TOK_CLASS_ARIF_CHARS[0][0][0] ||                 /// ARIFMETIKI OPERATORLAR: +
        CUR_CHAR==TOK_CLASS_ARIF_CHARS[1][0][0] ||                 // -; aýyrmak
        CUR_CHAR==TOK_CLASS_ARIF_CHARS[2][0][0] ||                 // *; köpeltmek
        CUR_CHAR==TOK_CLASS_ARIF_CHARS[3][0][0] ||                 // :; bölmek
        CUR_CHAR==TOK_CLASS_CMP_CHARS[0][0][0]   ||                              /// DEŇEŞDIRME OPERATORLAR: =
        CUR_CHAR==TOK_CLASS_CMP_CHARS[1][0][0]   ||                              // <; kiçi
        CUR_CHAR==TOK_CLASS_CMP_CHARS[2][0][0]   ||                              // >; uly
        CUR_CHAR==LOGIC_AND_CHAR  ||                            /// LOGIKI OPERATORLAR: &
        CUR_CHAR==LOGIC_OR_CHAR   ||                            // ?; ýa
        CUR_CHAR==LOGIC_NOT_CHAR  ||                            // !; däl bolsa
        CUR_CHAR==TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[0][0] );  /// Ulanyjy TIPI BILEN BAGLY OPERATORLAR; birlik bölüji
}


/** Parserlemeli harpy ötürýär.
    Eger ötürdilmeli harp bar bolsa, şony, ýa-da
    Faýldan indiki harpy ötürýär.

    Eger faýlyň soňunya ýetilse, şowsuzlyk, ýa-da
    şowlylyk gaýtarýar.
*/
int process_char( FILE *source, char check_char )
{
    int success = set_cur_char( source ) && update_inf();
    /** TPL goldaýan harpy bolmasa, program togtadylýar */
    if ( success && !is_valid_wchar_t() )
    {
        CUR_PART = 2;
        print_err(CODE2_UNKNOWN_TOKENS_CHAR, &inf_tok);
    }
    return success;
}


/** Eger eýýäm barlanmaly harp bar bolsa, şol harp ulanylýar.
    Ýogsa faýldan indiki harp alynýar.

    Eger faýl gutarsa, onda
        0 gaýtarýar
    Ýogsa
        1 gaýtarýar.
*/
int set_cur_char( FILE *source )
{
    if ( TMP_CHAR )
    {
        CUR_CHAR = TMP_CHAR;
        TMP_CHAR = 0;
    }
    else
    {
        if ( (CUR_CHAR=fgetwc(source))==WEOF )
            return 0;
    }
    return 1;
}



int parser_mode_end_cmd( FILE *s)
{
    if ( CUR_CHAR!=CMD_END )
        return 0;

    work_with_cmd( &cmd );
    init_cmd(&cmd, 0);
    return 1;
}



/** Parser indiki harpy barlamaz */
void prevent_from_parsing_file()
{
    /** Indiki parsing edilmeli harpy,
        ýene-de şu harpdan başla.
    */
    TMP_CHAR = CUR_CHAR;
}
