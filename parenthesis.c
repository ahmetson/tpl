/** Koddaky skobkalar bilen işleýär
 *
**/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "parenthesis.h"
#include "paren/types.h"
#include "parser.h"
#include "token/harpl.h"
#include "main/glob.h"
#include "main/inf.h"
#include "error.h"

char PARENTHESIS_OPEN = '(';
char PARENTHESIS_CLOSE= ')';
char PARENTHESIS_ELEM_SEPARATOR = ',';

// Komanda = 1, token = 2.
int PARENTHESIS_CLASS = 3;

int (*PAREN_RETURN_TYPE[PARENTHESIS_TYPES])(parenthesis *paren, int *ret_class, int *ret_type) = {
    empty_paren_return_type,
    paren_fns_args_return_type
};


/** Parserde skobka duşan wagty, içindäki elementler bilen işleýän bölüm

    \mode - eger skobka gutarsa, parseri adaty tokeni saýgarmana itermek üçin.
    \c    - parseriň häzirki duran harpy
    \cPos - parseriň häzirki duran harpynyň setirdäki nomeri
    \line - parseriň häzirki duran setiri

    \return void
**/
parenthesis parse_paren(FILE *s)
{
    parenthesis paren = parenthesis_new();
    parenthesis_elem elem = parenthesis_elem_new();

    while ((CUR_CHAR=fgetc(s))!=EOF)
    {
        update_inf();

        if(!is_valid_char())
            print_err(CODE2_UNKNOWN_TOKENS_CHAR, &inf_tok);

        /* Bolmady. Parser ishlanok */
        if (isspace(CUR_CHAR))
            continue;
        else if (CUR_CHAR==PARENTHESIS_CLOSE)
        {
            if (elem.type!=UNKNOWN_ITEM)
                paren_add_element(&paren, elem);
            recognize_paren(&paren);
            return paren;
        }
        else if (CUR_CHAR==PARENTHESIS_ELEM_SEPARATOR)
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
            token tok = parse_string(s);
            paren_elem_add_token(&elem, tok);
        }
        else if (CUR_CHAR==PARENTHESIS_OPEN)
        {
           parenthesis in_paren = parenthesis_new();
           param_elem_add_paren(&elem, in_paren);
        }
        else
        {
            token tok = parse_token(s);
            paren_elem_add_token(&elem, tok);
        }
    }
    parenthesis p;
    return p;
}

/** Täze skobkany ýasaýar. Skobkanyň elementleri üçin,
    kuçada ýer belleýär. Näme üçin kuçada? Sebäbi skobkda-da
    näçe element boljagy belli däl.
**/
 parenthesis parenthesis_new()
 {
    parenthesis paren = {
        NULL,
        0,
        0
    };

    ++GLOB_PARENTHS_NUM;
    long size = sizeof(*GLOB_PARENTHS)*GLOB_PARENTHS_NUM;

    GLOB_PARENTHS = realloc(GLOB_PARENTHS, size);

    int last = GLOB_PARENTHS_NUM-1;
    GLOB_PARENTHS[last] = NULL;
    paren.elems = GLOB_PARENTHS[last];

    return paren;
 }

/** Kuçadaky iň soňky skobka degişli ýere, täze element goşulýar.
**/
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
    paren->elems_num++;
    parenthesis_elem *e = realloc(paren->elems, sizeof(parenthesis_elem)*paren->elems_num);
    if (e==NULL)
    {
        // TODO
        // Yalnyshlyk: yer ramkada yetmedi
        printf("Yalnysh yer goshup bolmady, FILE: %s, LINE: %d\n", __FILE__, __LINE__);
    }
    else
    {
        paren->elems = e;
        paren->elems[paren->elems_num-1] = elem;
    }
}




/** Elemente täze tokeni goşýar

    \elem - içine goşulmaly
    \tok  - goşulmaly token

    \return - goşulma şowlymy ýa däl
**/
int paren_elem_add_token(parenthesis_elem *elem, token tok)
{
    if (elem->type!=UNKNOWN_ITEM)
    {
        command new_cmd;
        init_cmd(&new_cmd, 0);
        if (elem->type==CMD_ITEM)
        {
            cmd_add_item(&cmd, CMD_ITEM, get_empty_paren(), elem->cmd, get_empty_tok());
        }
        else if (elem->type==PAREN_ITEM)
        {
            cmd_add_item(&new_cmd, PAREN_ITEM, elem->paren, get_empty_cmd(), get_empty_tok());
        }
        else if (elem->type==TOKEN_ITEM)
        {
            cmd_add_item(&new_cmd, TOKEN_ITEM, get_empty_paren(), get_empty_cmd(), elem->tok);
        }
        cmd_add_item(&new_cmd, TOKEN_ITEM, get_empty_paren(), get_empty_cmd(), tok);

        elem->cmd = new_cmd;
        elem->type = CMD_ITEM;
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
    return 0;
}

/** Ýaýyň näme üçindigini anyklaýar.
    Şoňa görä-de, ýaýyň maglumatlaryny üýtgedýär.

    \paren - tanamaly ýaý

    \return - goşulma şowlymy ýa däl
**/
int recognize_paren(parenthesis *paren)
{
    int i, types = CONST_PAREN_TYPES_NUM;
    for (i=0; i<types; ++i)
    {
        if (PAREN_TYPES[i](paren))
            return 1;
    }
    return 0;
}


/** Nätanyş ulanylan ýaýlara goşulýar

    \paren - nätanyş ulanylan ýaý
    \cmdClass - gabat gelen komandanyň klasy
    \cmdType - gabat gelen komandanyň tipi
    \arg - komanda-da, şu tokeniň deregine garaşylan maglumat tipi
**/
void unknown_paren_add(parenthesis *paren, int cmd_class, int cmd_type, int waited_class, int waited_type)
{
    unknown_paren unk;
    unk.paren = paren;
    unk.cmd_class = cmd_class;
    unk.cmd_type = cmd_class;
    unk.waited_class = waited_class;
    unk.waited_type = waited_type;

    ++UNKNOWN_PARENS_NUM;

    long size;
    size = sizeof(unk)*UNKNOWN_PARENS_NUM;
    UNKNOWN_PARENS = realloc(UNKNOWN_PARENS, size);

    UNKNOWN_PARENS[UNKNOWN_PARENS_NUM-1] = unk;
}


parenthesis get_empty_paren()
{
    parenthesis p;
    return p;
}
