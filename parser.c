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

int parse()
{
	int prev_part = CUR_PART;
	CUR_PART = 2;

    int i, sizeof_wchar = sizeof( wchar_t );
	for ( i=0; i<CUR_SOURCE_BYTES_NUM/sizeof_wchar-1; ++i )
	{
	    /** Get next character.
            Set informations for parser */
	    process_char( CHECK_VALID_CHAR, &CUR_SOURCE_POINTER, CUR_SOURCE );

        if ( !iswspace( CUR_CHAR ) &&
             !parser_mode_pragma( CUR_SOURCE, &CUR_SOURCE_POINTER, CUR_SOURCE_BYTES_NUM ) &&
             !parser_mode_paren( &cmd, CUR_SOURCE, &CUR_SOURCE_POINTER, CUR_SOURCE_BYTES_NUM ) &&
             !parser_mode_string( &cmd, CUR_SOURCE, &CUR_SOURCE_POINTER, CUR_SOURCE_BYTES_NUM ) &&
             !parser_mode_end_cmd( CUR_SOURCE, &CUR_SOURCE_POINTER, CUR_SOURCE_BYTES_NUM ) )
        {
            /** PARSERIŇ TOKEN MODASYNDA */
            CUR_CMD = &cmd;
            token tok = parse_token( CUR_SOURCE, &CUR_SOURCE_POINTER, CUR_SOURCE_BYTES_NUM );

            work_with_token( &tok, &cmd );

            if (tok.type_class==TOK_CLASS_TRIANGLE_BLOCK && tok.potentional_types[0].type_num==TOKEN_TRIANGLE_BLOCK_OPEN_TYPE)
            {
                parse_triangle_block_inside( CUR_SOURCE, &CUR_SOURCE_POINTER, CUR_SOURCE_BYTES_NUM );
            }
        }
        i = CUR_SOURCE_POINTER;
	}

	check_semantics_after_parsing(&cmd);

	/// TRANSLATOR TO C: algoritmi faýla ýazýar
	translator_to_c();

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
    şowlylyk gaýtarýar. */
void process_char( char check_char, int *source_pointer, wchar_t *source )
{
    *source_pointer +=1 ;
    CUR_CHAR = source[ (*source_pointer)-1 ];

    update_inf();
    /** If Character is not support by TPL, break it */
    /*if ( check_char && !is_valid_wchar_t() )
    {
        CUR_PART = 2;
        print_err( CODE2_UNKNOWN_TOKENS_CHAR, &inf_tok );
    }*/
}



int parser_mode_end_cmd( )
{
    if ( CUR_CHAR!=CMD_END )
        return 0;

    work_with_cmd( &cmd );
    init_cmd( &cmd, 0 );
    return 1;
}



/** Parser indiki harpy barlamaz */
void prevent_from_parsing_file( int *pointer )
{
    /** Indiki parsing edilmeli harpy,
        ýene-de şu harpdan başla.
    */
    *pointer -= 1;
}
