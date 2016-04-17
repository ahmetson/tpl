/** Konstanta maglumatlar, dört sany.
*/
#include <string.h>
#include <stdlib.h>
#include "const_data.h"
#include "../../main/tpl_esc_keys.h"
#include "../../main/glob.h"
#include "../token_types.h"
#include "../helpers.h"
#include "../harpl.h"
#include "../../fns.h"


// DEBUG UCHIN
// Eger täze element goşulsa, onda token/token_types/const_data.h faýlda, bu sanawyň prototipinde-de
// Elementleriň sanyny bire ulaltmaly
wchar_t *CONST_DATA_TOK_NUM_WORDS[4] = {
    L"integer",
    L"float",
    L"wchar_t",
    L"string"
};

/// Token tipleriniň sanlary
int INT_CONST_DATA_TOK_NUM = 0,
    FLOAT_CONST_DATA_TOK_NUM = 1,
    CHAR_CONST_DATA_TOK_NUM = 2,
    STRING_CONST_DATA_TOK_NUM = 3;

/// Bu token tipi sanlar bilen işleýän komandalarda ulanylyp bilinýär.
/// Sanlar otrisatel ýa poloşitel bolup bilýär
wchar_t CHAR_MINUS = L'-',      /// Droblaryň san we drob bölümlerini bölüjiler
        FLOAT_CONST_DATA_SEPARATOR = L'_',              /// TPL kodda Sanlary bölüji
        FLOAT_CONST_DATA_C_CODE_SEPARATOR = L'.',       /// C kodda sanlary bölüji
/** Harplar dyrnagyň içine alynmaly */
        CHAR_CONST_DATA_OPENER = L'\'';




int set_def_type_alias_const_data(int *left_class, int *left_type)
{
    if (*left_class==TOK_CLASS_DEF_TYPE)
    {
        *left_class = TOK_CLASS_CONST_DATA;

        if (*left_type==1)  // San
            *left_type=INT_CONST_DATA_TOK_NUM;
        else if (*left_type==2)
            *left_type=FLOAT_CONST_DATA_TOK_NUM;
        else if (*left_type==0)
            *left_type=CHAR_CONST_DATA_TOK_NUM;
        else if (*left_type==3)
            *left_type=STRING_CONST_DATA_TOK_NUM;
    }
    return 1;
}


wchar_t *get_const_data_string(token *tok)
{
    int tok_type = get_token_type( tok );
    if ( tok_type==INT_CONST_DATA_TOK_NUM || tok_type==FLOAT_CONST_DATA_TOK_NUM )
        return (wchar_t *)get_token_value( tok );
    else if (tok->potentional_types[0].type_num==CHAR_CONST_DATA_TOK_NUM)
    {
        /// C dilinde 'backslash' iki symboldan durmalys
        if (tok->potentional_types[0].value[1]==L'\\')
        {
            wchar_t *backslash = L"'\\\\'";
            return backslash;
        }
        return (wchar_t *)get_token_value( tok );
    }
    else if (tok->potentional_types[0].type_num==STRING_CONST_DATA_TOK_NUM)
    {
        return (wchar_t *)get_glob_str_by_index( tok->potentional_types[0].string_value );
    }
    return L"";
}

int get_const_data_type(token *tok, int *tok_class, int *tok_type)
{
    *tok_class = TOK_CLASS_CONST_DATA;
    *tok_type  = get_token_type( tok );
    return 1;
}


void tok_int_c_code(token *tok, wchar_t **l, int *llen)
{
    wchar_t *val = tok->potentional_types[0].value;
    wcsadd_on_heap( l, llen, val );
}
void tok_float_c_code(token *tok, wchar_t **l, int *llen)
{
    wchar_t *val = tok->potentional_types[0].value;
    /// Translate _ to .
    size_t index = wcscspn( val, L"_" );
    val[ index ] = L'.';
    wcsadd_on_heap( l, llen, val );
}
void tok_wchar_t_c_code(token *tok, wchar_t **l, int *llen)
{
    wchar_t prev_l = L'L',
            *val = tok->potentional_types[0].value,
            *wchar_val = NULL;
    int len = sizeof(prev_l)+get_null_size();
    wchar_val = malloc( len );
    wchar_val[0] = prev_l;
    wchar_val[1] = L'\0';

    wcsadd_on_heap( &wchar_val, &len, val );

    wcsadd_on_heap( l, llen, wchar_val );

    free( wchar_val );
}
void tok_string_c_code(token *tok, wchar_t **l, int *llen)
{
    wchar_t *str = (wchar_t *)get_glob_str_by_index( tok->potentional_types[0].string_value );
    wcsadd_on_heap( l, llen, str );
}



int is_token_int_const_data(token *tok, wchar_t *tok_val)
{
    int i;
    // Birinji harp san, - bolup bilýär.
    if ( !iswdigit(tok_val[0]) && tok_val[0]!=CHAR_MINUS )
    {
        return 0;
    }

    /// Eger tokeniň birinji harpy '-' bolsa we tokenden öň komanda-da san bar bolsa
    ///     Token arifmetiki aýyrmak belgini aňladýar.
    /// Ýogsa
    ///     Token negatiw sany aňladýar.
    if ( wcslen(tok_val)>=1 && tok_val[0]==TOK_CLASS_ARIF_CHARS[ TOK_CLASS_ARIF_MINUS_TYPE ][0][0] &&
        CUR_CMD!=NULL && CUR_CMD->items_num)
    {
        int ret_class = -1, ret_type = -1;
        command_item *lci = get_cmd_item(CUR_CMD->items, CUR_CMD->items_num-1);
        if (lci->type==CMD_ITEM)
        {
            command *c = &lci->cmd;
            CMD_RETURN_TYPE[c->cmd_class][c->cmd_type](c, &ret_class, &ret_type);
        }
        else if(lci->type==TOKEN_ITEM)
            return_tok_type(&lci->tok, &ret_class, &ret_type);
        else if(lci->type==PAREN_ITEM)
            PAREN_RETURN_TYPE[lci->paren.type](&lci->paren, &ret_class, &ret_type);

        if (ret_class==TOK_CLASS_CONST_DATA && (ret_type==INT_CONST_DATA_TOK_NUM || ret_type==FLOAT_CONST_DATA_TOK_NUM))
        {
            return 0;
        }
   }

    for (i=1; i<wcslen(tok_val); ++i)
    {
        if (!iswdigit(tok_val[i]))
            return 0;
    }

    token_type tok_type;
    tok_type.type_num = INT_CONST_DATA_TOK_NUM;	// Number of token type
    tok_type.type_class = TOK_CLASS_CONST_DATA;
    tok_type.need_value = 1;
    tok_type.parenthesis = 1;
    wcsncpy(tok_type.value, tok_val, wcslen(tok_val)+1);
    tok_type.is_compl = 1;
    tok_type.type_must_check = 0;

    tok->is_compl     = 1;
    tok->type_class   = TOK_CLASS_CONST_DATA;

	add_potentional_token_type(tok, tok_type);

    return 1;
}
int is_token_float_const_data(token *tok, wchar_t *tok_val)
{
    int i, dot=0;

    if (!iswdigit(tok_val[0]) && tok_val[0]!=L'-' && tok_val[0]!=FLOAT_CONST_DATA_SEPARATOR)
    {
        return 0;
    }
    if (wcslen(tok_val)>=1 && tok_val[0]==TOK_CLASS_ARIF_CHARS[ TOK_CLASS_ARIF_MINUS_TYPE ][0][0] &&
        CUR_CMD!=NULL && CUR_CMD->items_num)
    {
        int ret_class = -1, ret_type = -1;
        command_item *lci = get_cmd_item(CUR_CMD->items, CUR_CMD->items_num-1);
        if (lci->type==CMD_ITEM)
        {
            command *c = &lci->cmd;
            CMD_RETURN_TYPE[c->cmd_class][c->cmd_type](c, &ret_class, &ret_type);
        }
        else if(lci->type==TOKEN_ITEM)
            return_tok_type(&lci->tok, &ret_class, &ret_type);
        else if(lci->type==PAREN_ITEM)
            PAREN_RETURN_TYPE[lci->paren.type](&lci->paren, &ret_class, &ret_type);

        if (ret_class==TOK_CLASS_CONST_DATA && (ret_type==INT_CONST_DATA_TOK_NUM || ret_type==FLOAT_CONST_DATA_TOK_NUM))
        {
            return 0;
        }
   }


    for (i=1; i<wcslen(tok_val); ++i)
    {
        if (!iswdigit(tok_val[i]))
        {
            if (tok_val[i]==FLOAT_CONST_DATA_SEPARATOR && !dot)
            {
                // Ýasaljak kodda, droblar kodly faýlynyňkydan üýtgeşik bölüniji arkaly tapawutlanmaly.
                //tok_val[i] = FLOAT_CONST_DATA_C_CODE_SEPARATOR;
                // Indiki sanlar drobyň galyndylarynyňky
                dot = 1;
            }
            else
            {
                return 0;
            }
        }
    }

    token_type tok_type;
    tok_type.type_num   = FLOAT_CONST_DATA_TOK_NUM;	// Number of token type
    tok_type.type_class = TOK_CLASS_CONST_DATA;
    tok_type.need_value = 1;
    tok_type.parenthesis = 1;
    wcsncpy(tok_type.value, tok_val, wcslen(tok_val)+1);
    tok_type.is_compl = 1;
    tok_type.type_must_check = 0;

    tok->is_compl     = 1;
    tok->type_class   = TOK_CLASS_CONST_DATA;

	add_potentional_token_type(tok, tok_type);

    return 1;
}
int is_token_wchar_t_const_data(token *tok, wchar_t *tok_val)
{
    int len = wcslen(tok_val), complete=0;

    if (len>(TPL_ESC_KEY_MAX_LEN+2) || tok_val[0]!=L'\'' )
        return 0;

    wchar_t ESC_key = 0;
    if (tok_val[1]==TPL_ESC_KEY_OPENER && len>2)
    {
        ESC_key=1;
        if (!is_tpl_ESC_key(tok_val[2], 1))
        {
            // Tokeniň içinde ESC belgi bolmasa, token maksimum 3 belgiden ybarat bolmaly.
            //delete_potentional_token_type(tok, TOK_CLASS_CONST_DATA, CHAR_CONST_DATA_TOK_NUM);
            return 0;
        }
    }

    if ((ESC_key && wcslen(tok_val)>4) || (!ESC_key && len>3))
    {
        return 0;
    }

    // Token maglumatyn ichinde ESC belgi uygtedileni sebapli, uzynlyga tazeden hasaplanmaly
    if ((ESC_key && len==4) || (!ESC_key && len==3))
    {
        if (tok_val[wcslen(tok_val)-1]==L'\'')
        {
            tpl_ESC_key_to_c_ESC_key(tok_val);
            complete = 1;
        }
        else
            return 0;
    }

    token_type tok_type;
    tok_type.type_num   = CHAR_CONST_DATA_TOK_NUM;	// Number of token type
    tok_type.type_class = TOK_CLASS_CONST_DATA;
    tok_type.need_value = 1;
    tok_type.parenthesis = 1;
    wcsncpy(tok_type.value, tok_val, wcslen(tok_val)+1);
    tok_type.is_compl = complete;
    tok_type.type_must_check = 0;

    tok->is_compl     = complete;
    tok->type_class   = TOK_CLASS_CONST_DATA;

	add_potentional_token_type(tok, tok_type);

    return 1;
}

