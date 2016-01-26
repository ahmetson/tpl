/** HARPL tokeniň öz aýratynlyklary bar.
    Sebäbi bu tokeniň näçe symboldan durjagy näbelli.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "harpl.h"
#include "helpers.h"
#include "../tpl.h"
#include "../main/glob.h"
#include "../main/tpl_esc_keys.h"
#include "../main/inf.h"
#include "../parser.h"
#include "../error.h"
#include "../fns.h"

wchar_t HARPL_OPENER = L'"';            /// TPL kodynda HARPL'yň başlaýan harpy
wchar_t *HARPL_OPENER_STRING = L"L\"";  /// C kodynda HARPL'yň başlaýan harpy


int parser_mode_string( FILE *source, command *cmd )
{
    if ( CUR_CHAR!=HARPL_OPENER )
        return 0;
    token tok = parse_string( source );
    work_with_token( &tok, cmd );
    return 1;
}

/** Parserden harplary tanaýan bölüm */
token parse_string( FILE *s )
{
    token string_tok;
    set_token_string_params(&string_tok);

    /** Harplaryň näçe sany simboldan durjagy näbelli bolany üçin,
        HEAP'de global sanawda ýatda saklanylýar.

        Global HEAP'däki sanawa täze harplyk üçin ýer ýasalýar*/
    increment_string_tokens();

    /** C dilinde HARPL goşa dyrnak bilen başlamaly.
    Şonuň üçin goşa dyrnagy ötürýär*/
    last_glob_str_init();

    while ( 1 )
    {
        if ( !process_char( s, UNCHECK_VALID_CHAR ) )
            break;

        last_glob_str_add_chr( CUR_CHAR );

        if ( !is_token_string_const_data( &string_tok ) )
        {
            /** Goşa dyrnak bilen başlaýan başga token ýoklugy sebäpli,
                Ýalňyşlygy görkezýär*/
            print_err( CODE2_STRING_IS_WRONG, &string_tok );
        }
        /** Soňky goşa dyrnak ötürdilip, HARPLYK ýasalypdyr*/
        else if ( is_complete_token( &string_tok) )
        {
            return string_tok;
        }
    }
    return string_tok;
}

/** Berlen tokene HARPL parametrleri ötürdýär*/
void set_token_string_params(token *string_tok)
{
    inf_add_to_token(string_tok, CUR_CHAR, CUR_CHAR_POS, CUR_LINE);

    string_tok->is_compl = 0;
    string_tok->type_class = TOK_CLASS_CONST_DATA;

    token_type tok_type;
    tok_type.is_compl       = 0;
    tok_type.need_value     = 1;
    tok_type.type_class     = TOK_CLASS_CONST_DATA;
    tok_type.type_num       = STRING_CONST_DATA_TOK_NUM;
    tok_type.parenthesis    = 1;
    tok_type.string_value   = GLOB_STRINGS_NUM-1;

    add_potentional_token_type(string_tok, tok_type);
}

/** Ýasaljak programma-da ulanylýan sözleriň sany ýene bire köpeldilýär. */
int increment_string_tokens()
{
    GLOB_STRINGS_NUM++;
    size_t size = 0;

    size = sizeof(*GLOB_STRINGS)*GLOB_STRINGS_NUM;
    GLOB_STRINGS = realloc(GLOB_STRINGS, size);

    if (GLOB_STRINGS==NULL)
    {
        return 0;
    }
    GLOB_STRINGS[GLOB_STRINGS_NUM-1] = NULL;


    return 1;
}


/** Iň soňky tekste, HARPL tokeniň C dildäki başyny, ýagny goşa dyrnagy açyjyny goýýar*/
void last_glob_str_init()
{
    int len = 0;
    wcsadd_on_heap( &GLOB_STRINGS[ GLOB_STRINGS_NUM-1 ], &len, HARPL_OPENER_STRING );
}


wchar_t *get_glob_str_by_index( int str_num )
{
    return GLOB_STRINGS[ str_num ];
}


/** Ýasaljak programma-da ulanylýan söz tokenleriň iň soňky tanalanyna harpy goşýar*/
int last_glob_str_add_chr( wchar_t c )
{
    int last = GLOB_STRINGS_NUM-1;
    long size = ( sizeof( c )*2 ) + count_bytes( GLOB_STRINGS[last] );

    GLOB_STRINGS[ last ] = realloc( GLOB_STRINGS[last], size );
    if ( GLOB_STRINGS[ last ]==NULL )
        printf("Soz uchin yer yasap bolmady: %s\n", __FILE__ );

    GLOB_STRINGS[last][size/sizeof(c)-1] = L'\0';
    GLOB_STRINGS[last][size/sizeof(c)-2] = c;
    return 1;
}

/** Ýasaljak programma-da ulanylýan söz tokenleriň iň soňky tanalanyna harpy goşýar */
int last_glob_str_conc_str( wchar_t *str )
{
    #ifndef LAST
    #define LAST GLOB_STRINGS_NUM-1
    #endif // LAST
    int size = 0;

    if ( GLOB_STRINGS[ LAST ]!=NULL )
    {
        size = count_bytes( GLOB_STRINGS[ LAST ] );
        GLOB_STRINGS[ LAST ] = realloc(GLOB_STRINGS[ LAST ], size);

        if ( GLOB_STRINGS[ LAST ]==NULL)
            printf("Soz uchin yer yasap bolmady: %s\n", __FILE__ );
        else
            wcsncat( GLOB_STRINGS[ LAST ], str, wcslen( str ) );
    }
    else
    {
        size += count_bytes( str )+get_null_size();
        GLOB_STRINGS[ LAST ] = realloc( GLOB_STRINGS[ LAST ], size );
        if ( GLOB_STRINGS[ LAST ]==NULL )
            printf("Soz uchin yer yasap bolmady: %s\n", __FILE__);
        wcsncpys( GLOB_STRINGS[ LAST ], str );
    }

    #undef LAST
    return 1;
}


void last_glob_str_free()
{
    #define LAST  GLOB_STRINGS_NUM-1
    if ( GLOB_STRINGS[ LAST ]!=NULL )
    {
        free( GLOB_STRINGS[ LAST ] );
        GLOB_STRINGS[ LAST ] = NULL;
    }
    #undef LAST
}

/** Ýasaljak programma üçin goşulan iň soňky sözi çalyşýar */
int last_glob_str_change_val( wchar_t *new_val )
{
    #define LAST  GLOB_STRINGS_NUM-1
    last_glob_str_free();

    int new_size = count_bytes( new_val )+get_null_size();

    GLOB_STRINGS[ LAST ] = realloc( GLOB_STRINGS[ LAST ], new_size );
    wcsncpys( GLOB_STRINGS[ LAST ], new_val );

    #undef LAST
    return 1;
}


/// Tokeniň sözmidigini barlaýar
int is_token_string_const_data(token *tok)
{
    int i;
    wchar_t *tok_val = GLOB_STRINGS[ GLOB_STRINGS_NUM-1 ];

    if ( !( get_token_type_class( tok )==TOK_CLASS_CONST_DATA &&
            get_token_type( tok )      ==STRING_CONST_DATA_TOK_NUM ) )
        return 0;

    int len = wcslen( tok_val );

    /// Dogry tekstidigini barlamak nämä gerek, diňe Goşa dyrnak bar.
    if ( len==1 )
        return 1;

    #define ONSTACK 1
    if (is_string_const_data_compl(tok_val, ONSTACK))
    {
        tok->potentional_types[0].is_compl = 1;
        tok->is_compl = 1;
    }
    #undef ONSTACK
    return 1;
}



int is_string_const_data_compl(wchar_t *tok_val, wchar_t onstack)
{
    #define LAST ( wcslen( tok_val )-1 )

    if (wcslen(tok_val)<2)
        return 0;
    if ( tok_val[ LAST ]==HARPL_OPENER && tok_val[ LAST-1 ]!=TPL_ESC_KEY_OPENER )
    {
        tpl_ESC_key_to_c_ESC_key(tok_val);
        if ( !onstack ) /** HEAP'däki tekst*/
            last_glob_str_change_val( tok_val );
        return 1;
    }
    return 0;
    #undef LAST
}
