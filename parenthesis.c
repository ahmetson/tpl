/** Koddaky skobkalar bilen işleýär
 *
**/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parenthesis.h"
#include "paren/types.h"
#include "parser.h"
#include "token/harpl.h"
#include "tpl.h"
#include "main/glob.h"
#include "main/inf.h"
#include "fns.h"
#include "translator_to_c.h"
#include "token/helpers.h"
#include "error.h"

wchar_t PARENTHESIS_OPEN = L'(';
wchar_t PARENTHESIS_CLOSE= L')';
wchar_t PARENTHESIS_ELEM_SEPARATOR = L',';

// Komanda = 1, token = 2.
int PARENTHESIS_CLASS = 3;

int (*PAREN_RETURN_TYPE[PARENTHESIS_TYPES])(parenthesis *paren, int *ret_class, int *ret_type) = {
    empty_paren_return_type,
    paren_fns_args_return_type,
    empty_paren_return_type,
    empty_paren_return_type
};


int parser_mode_paren( command *cmd, wchar_t *SOURCE, int *SOURCE_POINTER_NUM, int SOURCE_BYTES_NUM )
{
    if( CUR_CHAR!=PARENTHESIS_OPEN )
        return 0;
    parenthesis par = parse_paren( SOURCE, SOURCE_POINTER_NUM, SOURCE_BYTES_NUM );
    add_to_cmd(cmd, PAREN_ITEM, get_empty_tok(), par, get_empty_cmd() );

    return 1;
}


/** Parserde skobka duşan wagty, içindäki elementler bilen işleýän bölüm

    \mode - eger skobka gutarsa, parseri adaty tokeni saýgarmana itermek üçin.
    \c    - parseriň häzirki duran harpy
    \cPos - parseriň häzirki duran harpynyň setirdäki nomeri
    \line - parseriň häzirki duran setiri

    \return void
*/
parenthesis parse_paren( wchar_t *SOURCE, int *SOURCE_POINTER_NUM, int SOURCE_BYTES_NUM )
{
    parenthesis paren = parenthesis_new();
    parenthesis_elem elem = parenthesis_elem_new();

    int i, sizeof_wchar = sizeof( wchar_t );
    for( i=( *SOURCE_POINTER_NUM )-1; i<SOURCE_BYTES_NUM/sizeof_wchar; ++i )
    {
        process_char( CHECK_VALID_CHAR, SOURCE_POINTER_NUM, SOURCE );

        if(!is_valid_wchar_t())
            print_err(CODE2_UNKNOWN_TOKENS_CHAR, &inf_tok);

        /* Bolmady. Parser ishlanok */
        if ( iswspace( CUR_CHAR ) )
            continue;
        else if ( CUR_CHAR==PARENTHESIS_CLOSE )
        {
            if ( elem.type!=UNKNOWN_ITEM )
                paren_add_element( &paren, elem );
            recognize_paren( &paren );
            return paren;
        }
        else if ( CUR_CHAR==PARENTHESIS_ELEM_SEPARATOR )
        {
            // Eger element boş bolmasa, onda ýalňyşlyk çykarylmaly
            if ((elem.type==CMD_ITEM && !elem.cmd.is_compl) ||
                (elem.type==TOKEN_ITEM && !elem.tok.is_compl))
            {
                CUR_PART = 2;
                //print_err(CODE2_ELEMENT_IN_PAREN_NOT_IDENT);
                return paren;
            }

            paren_add_element(&paren, elem);
            elem = parenthesis_elem_new();

            // Öňki element komandanyň içine geçirilýär.
        }
        else if (CUR_CHAR==HARPL_OPENER)
        {
            token tok = parse_string( SOURCE, SOURCE_POINTER_NUM, SOURCE_BYTES_NUM );
            paren_elem_add_token(&elem, tok);
        }
        else if (CUR_CHAR==PARENTHESIS_OPEN)
        {
           parenthesis in_paren = parse_paren( SOURCE, SOURCE_POINTER_NUM, SOURCE_BYTES_NUM );
           param_elem_add_paren(&elem, in_paren);
        }
        else
        {
            if (elem.type==CMD_ITEM)
                CUR_CMD = &elem.cmd;
            else
                CUR_CMD = NULL;
            token tok = parse_token( SOURCE, SOURCE_POINTER_NUM, SOURCE_BYTES_NUM );
            paren_elem_add_token(&elem, tok);
        }
    }
    parenthesis p;
    return p;
}

/** Täze skobkany ýasaýar. Skobkanyň elementleri üçin,
    kuçada ýer belleýär. Näme üçin kuçada? Sebäbi skobkda-da
    näçe element boljagy belli däl.*/
 parenthesis parenthesis_new()
 {
    ++GLOB_PARENTHS_NUM;
    long size = sizeof(*GLOB_PARENTHS)*GLOB_PARENTHS_NUM;

    GLOB_PARENTHS = realloc(GLOB_PARENTHS, size);

    int last = GLOB_PARENTHS_NUM-1;
    GLOB_PARENTHS[last] = NULL;

    parenthesis paren = {
        0,
        0,
        0
    };
    paren.elems = last;

    return paren;
 }

/** Kuçadaky iň soňky skobka degişli ýere, täze element goşulýar.*/
parenthesis_elem parenthesis_elem_new()
{
    parenthesis_elem elem;
    elem.type = UNKNOWN_ITEM;

    return elem;
}

/** Skobka täze element goşulýar

    \paren - elementiň goşulýan skobkasy
    \elem  - goşulmaly element
**/
void paren_add_element(parenthesis *paren, parenthesis_elem elem)
{
    /// Check for true the element
    if ( elem.type==CMD_ITEM && !recognize_cmd( &elem.cmd ) )
    {
        CUR_PART = 2;
        print_err( CODE2_CANT_IDENT_CMD_IN_PAREN, ( token* )inf_get_last_token( &elem.cmd ) );
    }

    ++paren->elems_num;

    parenthesis_elem *e = realloc(GLOB_PARENTHS[paren->elems], sizeof(**GLOB_PARENTHS)*paren->elems_num);
    if (e==NULL)
    {
        // TODO
        // Yalnyshlyk: yer ramkada yetmedi
        printf("Yalnysh yer goshup bolmady, FILE: %s, LINE: %d\n", __FILE__, __LINE__);
    }
    else
    {
        GLOB_PARENTHS[paren->elems] = e;
        GLOB_PARENTHS[paren->elems][paren->elems_num-1] = elem;
    }
}


/** Elemente täze tokeni goşýar

    \elem - içine goşulmaly
    \tok  - goşulmaly token

    \return - goşulma şowlymy ýa däl
**/
int paren_elem_add_token(parenthesis_elem *elem, token tok)
{
    /// Elementde öňem birlik bardy
    if (tok.type_class==TOK_CLASS_COMMENT)
        return 1;
    if (elem->type!=UNKNOWN_ITEM)
    {
        if (elem->type==CMD_ITEM)
        {
            add_to_cmd( &elem->cmd, TOKEN_ITEM, tok, get_empty_paren(), get_empty_cmd());
        }
        else if (elem->type==PAREN_ITEM)
        {
            command new_cmd;
            init_cmd(&new_cmd, 0);
            new_cmd.items_num = 1;
            new_cmd.items = subcmd_items_add(new_cmd.items_num);

            command_item fci;
            fci.type = PAREN_ITEM;
            fci.paren = elem->paren;
            put_cmd_item(new_cmd.items , 0, fci);

            elem->cmd = new_cmd;
            elem->type = CMD_ITEM;

            add_to_cmd( &elem->cmd, TOKEN_ITEM, tok, get_empty_paren(), get_empty_cmd() );
        }
        else if (elem->type==TOKEN_ITEM)
        {
            command new_cmd;
            init_cmd(&new_cmd, 0);
            new_cmd.items_num = 1;
            new_cmd.items = subcmd_items_add(new_cmd.items_num);
            command_item fci;
            fci.type = TOKEN_ITEM;
            fci.tok = elem->tok;
            put_cmd_item(new_cmd.items , 0, fci);

            elem->cmd = new_cmd;
            elem->type = CMD_ITEM;

            add_to_cmd( &elem->cmd, TOKEN_ITEM, tok, get_empty_paren(), get_empty_cmd() );
        }
        return 1;
    }
    else
    {
        elem->tok = tok;
        elem->type = TOKEN_ITEM;
        return 1;
    }
    return 0;
}

/** Elemente içki ýaýy goşýar

    \elem - içine goşulmaly
    \tok  - goşulmaly ýaýlar

    \return - goşulma şowlymy ýa däl
**/
int param_elem_add_paren(parenthesis_elem *elem, parenthesis paren)
{
    /// Elementde öňem birlik bardy
    if (elem->type!=UNKNOWN_ITEM)
    {
        if (elem->type==CMD_ITEM)
        {
            add_to_cmd( &elem->cmd, PAREN_ITEM, get_empty_tok(), paren, get_empty_cmd());
        }
        else if (elem->type==PAREN_ITEM)
        {
            command new_cmd;
            init_cmd(&new_cmd, 0);
            new_cmd.items_num = 1;
            new_cmd.items = subcmd_items_add(new_cmd.items_num);

            command_item fci;
            fci.type = PAREN_ITEM;
            fci.paren = elem->paren;
            put_cmd_item(new_cmd.items, 0, fci);

            elem->cmd = new_cmd;
            elem->type = CMD_ITEM;

            add_to_cmd( &elem->cmd, PAREN_ITEM, get_empty_tok(), paren, get_empty_cmd());
        }
        else if (elem->type==TOKEN_ITEM)
        {
            command new_cmd;
            init_cmd(&new_cmd, 0);
            new_cmd.items_num = 1;
            new_cmd.items = subcmd_items_add(new_cmd.items_num);

            command_item fci;
            fci.type = TOKEN_ITEM;
            fci.tok = elem->tok;
            put_cmd_item(new_cmd.items , 0, fci);

            elem->cmd = new_cmd;
            elem->type = CMD_ITEM;

            add_to_cmd( &elem->cmd, PAREN_ITEM, get_empty_tok(), paren, get_empty_cmd());
        }
        return 1;
    }
    else
    {
        elem->paren = paren;
        elem->type = PAREN_ITEM;
        return 1;
    }
    return 0;
}

/** Ýaýyň näme üçindigini anyklaýar.
    Şoňa görä-de, ýaýyň maglumatlaryny üýtgedýär.

    \paren - tanamaly ýaý

    \return - goşulma şowlymy ýa däl
**/
int recognize_paren(parenthesis *paren)
{
    int i;
    for (i=0; i<CONST_PAREN_TYPES_NUM; ++i)
    {
        if (PAREN_TYPES[i](paren))
            return 1;
    }
    return 0;
}

parenthesis get_empty_paren()
{
    parenthesis p;
    return p;
}


void paren_get_c_code(parenthesis *p, wchar_t **l, int *llen)
{
    wchar_t *o = L"(",
            *c = L")";

    /// ( Ýaý açylýar
    wcsadd_on_heap( l, llen, o );

    /// ülňiler salynýar
    if (p->type==PAREN_TYPE_FNS_ARGS)
    {
        if ( p->elems_num )
        {
            parenthesis_elem *p_es = get_paren_elems(p->elems);

            int i;
            for (i=0; i<p->elems_num; ++i)
            {
                write_paren_item_c_code( &p_es[i], l, llen );
            }
        }
    }
    else if ( !p->type && p->elems_num==1 )
    {
        parenthesis_elem *p_es = get_paren_elems(p->elems);
        write_paren_item_c_code( &p_es[0], l, llen );
    }

    /// ) Ýaý ýapylýar
    wcsadd_on_heap( l, llen, c );
}


/** Eger ýaý bir birlik saklaýan bolsa, onda birligiň tipini gaýtarýar:
    @p        - ýaýyň özi,
    @itemType - ýaýyň içindäki birligiň tipi
    @rClass   - birligiň tipiniň klasy
    @rType    - birligiň tipi

    \return - ýaýyň içindäki birligiň tipini ötürdip boldumy ýa ýoklugynyň statusyny gaýtarýar */
int get_paren_item_type(parenthesis *p, int *item_type, int *rClass, int *rType)
{
    if ( !p->type && p->elems_num==1)
    {
        parenthesis_elem *p_es = get_paren_elems(p->elems);

        if (p_es[0].type==PAREN_ITEM)
        {
            return get_paren_item_type(&p_es[0].paren, item_type, rClass, rType);
        }
        else if (p_es[0].type==CMD_ITEM)
        {
            *item_type = CMD_ITEM;
            *rClass = p_es[0].cmd.cmd_class;
            *rType  = p_es[0].cmd.cmd_type;
        }
        else if (p_es[0].type==TOKEN_ITEM)
        {
            *item_type = TOKEN_ITEM;
            *rClass = p_es[0].tok.potentional_types[0].type_class;
            *rType  = p_es[0].tok.potentional_types[0].type_num;
        }
        return 1;
    }
    return 0;
}



int is_paren_not_compl_item_exist(parenthesis *p, wchar_t rec)
{
    if (p->elems_num)
    {
        parenthesis_elem *p_es = get_paren_elems(p->elems);

        int i;
        for (i=0; i<p->elems_num; ++i)
        {
            if (p_es[i].type==CMD_ITEM)
            {
                if (!p_es[i].cmd.is_compl)
                {
                    return 1;
                }
            }
            else if (p_es[i].type!=TOKEN_ITEM && rec)
            {
                if (p_es[i].type==CMD_ITEM)
                {
                    return is_cmd_not_compl_item_exist(&p_es[i].cmd, 1);
                }
                else if (p_es[i].type==PAREN_ITEM)
                {
                    return is_paren_not_compl_item_exist(&p_es[i].paren, rec);
                }
            }
        }
    }
    return 0;
}


void get_parenthesis_elem_type( parenthesis_elem *pe, int *elem_type, int *c, int *t )
{
    if ( pe->type==PAREN_ITEM )
    {
        get_paren_type( &pe->paren, elem_type, c, t);
        *elem_type = PAREN_ITEM;
    }
    else if ( pe->type==TOKEN_ITEM )
    {
        *elem_type = TOKEN_ITEM;
        *c = get_token_type_class( &pe->tok );
        *t = get_token_type( &pe->tok );
    }
    else if ( pe->type==CMD_ITEM )
    {
        *elem_type = CMD_ITEM;
        *c = pe->cmd.cmd_class;
        *t = pe->cmd.cmd_type;
    }
}

parenthesis_elem *get_paren_elems(int paren_num)
{
    return GLOB_PARENTHS[paren_num];
}
