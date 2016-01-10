/**
 * Tokenlerin tiplerinin bilen bagly ahli maglumatlar.
 * Eyyam bolup biljek tiplerin hemmesi yglan edilen son doredilmeli
**/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "token_types/assign.h"
#include "token_types/glob.h"
#include "token_types/def_types.h"
#include "token_types/const_data.h"
#include "token_types/arif.h"
#include "token_types/cmp.h"
#include "token_types/logic.h"
#include "token_types/ctrl_sttmnt.h"
#include "token_types/block.h"
#include "token_types/loop_sttmnt.h"
#include "token_types/triangle_block.h"
#include "token_types/utype.h"
#include "token_types/utype_con.h"
#include "token_types/void.h"
#include "token_types/comment.h"
#include "keywords.h"
#include "token_types.h"
#include "../main/tpl_esc_keys.h"
#include "../main/user_def_type.h"
#include "../fns.h"
#include "../tokens.h"
#include "../main/glob.h"

// In uly token 'ident' - 6 harpdan ybarat

// Nache sany token tip bar bolsa, shonchada klas bar
const int TOK_CLASS_UNDEFINED       = -1;
const int TOK_CLASS_UNKNOWN         = 0;
const int TOK_CLASS_DEF_TYPE 	    = 1;
const int TOK_CLASS_GLOB 		    = 2;
const int TOK_CLASS_IDENT           = 3;
const int TOK_CLASS_ASSIGN		    = 4;
const int TOK_CLASS_CONST_DATA      = 5;
const int TOK_CLASS_ARIF            = 6;
const int TOK_CLASS_CMP             = 7;
const int TOK_CLASS_LOGIC           = 8;
const int TOK_CLASS_CTRL_STTMNT     = 9;
const int TOK_CLASS_BLOCK           = 10;
const int TOK_CLASS_LOOP_STTMNT     = 11;
const int TOK_CLASS_TRIANGLE_BLOCK  = 12;
const int TOK_CLASS_UTYPE           = 13;
const int TOK_CLASS_UTYPE_CON       = -2;
const int TOK_CLASS_VOID            = 15;
const int TOK_CLASS_COMMENT         = 16;

// Used for debugging
char *type_classes[] = {
    "unknown",
	"def_type",
	"glob",
	"ident",
	"assign",
	"const_data",
	"arifmetika",
	"deňeşdirme",
	"logiki",
	"dolandyryş operatory",
	"blok",
	"gaýtalama operatory",
	"üçburç blok",
	"Ulanyjyň görnüşi",
	"Ulanyjyň görnüşine çatylma",
	"Boş tip",
	"Teswir"
};


// san, drob, harp we harpl
int is_token_def_type(token *tok, wchar_t *tok_val)
{
	/*Go through array of possible types*/
	int i, answer, found = 0;
	token_type tok_type;

	for(i=0; i<DEF_TYPES_NUM; i++)
	{
		answer = wcsstr_by_offset(def_type_list[i].tk_name, tok_val, 0);
		if (answer>=0)
		{
			tok_type.type_num = i;							// Number of token type
			tok_type.type_class = TOK_CLASS_DEF_TYPE;
			tok_type.need_value = 0;
			tok_type.is_compl = (answer==0) ? 1 : 0;
			tok_type.type_must_check = 0;
			tok_type.parenthesis = 1;
			tok->is_compl = (answer==0) ? 1 : 0;

			add_potentional_token_type(tok, tok_type);

			// Tokene gornush girizilen son chagyrylmaly
			if (tok_type.is_compl==1)
				tok->type_class = TOK_CLASS_DEF_TYPE;
			found = 1;
		}
	}

	return found;
}
int is_token_def_glob(token *tok, wchar_t *tok_val)
{
	int answer;
	token_type tok_type;

	answer = wcsstr_by_offset(glob_def_string, tok_val, 0);
	if (answer>=0)
	{
		tok_type.type_num = 0;							// Number of token type
		tok_type.type_class = TOK_CLASS_GLOB;
		tok_type.need_value = 0;
		tok_type.is_compl = (answer==0) ? 1 : 0;
		tok_type.type_must_check = 0;
        tok_type.parenthesis = 0;
		tok->is_compl = (answer==0) ? 1 : 0;

		add_potentional_token_type(tok, tok_type);

		// Tokene gornush girizilen son chagyrylmaly
		if (tok_type.is_compl==1)
			tok->type_class = TOK_CLASS_GLOB;

		return 1;
	}
	return 0;
}
int is_token_ident(token *tok, wchar_t *tok_val)
{
	token_type tok_type;
	//printf(L"Identifikator barlagy: %s\n", tok_val->val);

	// Identifikatorlar birinji harpy harp bolmaly
	if (!isalpha(tok_val[0]))
		return 0;
	//printf(L"Identifikator harpdan bashlayar\n");

	int i;
	// Identifikator awchar_t sozi bolup bilenok
	for(i=0; i<MAX_KEYWORDS_NUM; i++)
	{
		if (wcslen(keywords[i])!=wcslen(tok_val))
			continue;
		//len = (wcslen(keywords[i])>wcslen(tok_val->val)) ? wcslen(keywords[i]) : wcslen(tok_val->val);

		//printf(L"For %s, Result of comparing: %0d\n", tok_val->val, wcsncmp(tok_val->val, keywords[i], wcslen(keywords[i])));
		if (wcsncmp(tok_val, keywords[i], wcslen(keywords[i]) )==0 )
			return 0;
	}
	//printf(L"Identifikator awchar_t sozi dal\n");

	// Identifikator harplardan, sanlardan we '_' belgiden durmaly
	for(i=0; i<wcslen(tok_val); i++)
	{
		if (isalnum(tok_val[i])==0 && tok_val[i]!=L'_')
			return 0;
	}

	// This is - identifikator
	tok_type.type_num = 0;							// Number of token type

	tok_type.need_value = 1;
	tok_type.type_class = TOK_CLASS_IDENT;
	tok_type.parenthesis = 1;
	wcsncpy(tok_type.value, tok_val, wcslen(tok_val)+1);
	tok_type.is_compl = 1;
	tok_type.type_must_check = 0;

	add_potentional_token_type(tok, tok_type);

	// Tokene gornush girizilen son chagyrylmaly
	if (tok_type.is_compl==1)
		tok->type_class = TOK_CLASS_IDENT;

	return 1;
}
int is_token_assign(token *tok, wchar_t *tok_val)
{
    if (is_token_var_left_assign(tok, tok_val))
        return 1;
    return is_token_var_rigtht_assign(tok, tok_val);
}

// <-
int is_token_var_rigtht_assign(token *tok, wchar_t *tok_val)
{
    int answer;
    wchar_t *zero = L"0";
	token_type tok_type;

	answer = wcsstr_by_offset(RIGHT_ASSIGN_TOKEN_VALUE, tok_val, 0);
	if (answer>=0)
	{
		tok_type.type_num = RIGHT_ASSIGN_TOK_NUM;	// Number of token type
		tok_type.type_class = TOK_CLASS_ASSIGN;
		tok_type.need_value = 1;
		wcsncpy(tok_type.value, zero, wcslen(zero)+1);
		tok_type.is_compl = (answer==0) ? 1 : 0;
		tok_type.type_must_check = 0;
		tok_type.parenthesis = 0;
		tok->is_compl = (answer==0) ? 1 : 0;

		add_potentional_token_type(tok, tok_type);

		// Tokene gornush girizilen son chagyrylmaly
		if (tok_type.is_compl==1)
			tok->type_class = TOK_CLASS_ASSIGN;

		return 1;
	}
	return 0;
}
int is_token_var_left_assign(token *tok, wchar_t *tok_val)
{
	int answer;
	token_type tok_type;

	answer = wcsstr_by_offset(LEFT_ASSIGN_TOKEN_VALUE, tok_val, 0);
	if (answer>=0)
	{
		tok_type.type_num = LEFT_ASSIGN_TOK_NUM;	// Number of token type
		tok_type.type_class = TOK_CLASS_ASSIGN;
		tok_type.need_value = 0;
		tok_type.is_compl = (answer==0) ? 1 : 0;
		tok_type.type_must_check = 0;
		tok_type.parenthesis = 0;
		tok->is_compl = (answer==0) ? 1 : 0;

		add_potentional_token_type(tok, tok_type);

		// Tokene gornush girizilen son chagyrylmaly
		if (tok_type.is_compl==1)
			tok->type_class = TOK_CLASS_ASSIGN;

		return 1;
	}
	return 0;
}
int is_token_int_const_data(token *tok, wchar_t *tok_val)
{
    int i;
    // Birinji harp san, - bolup bilýär.
    if (!isdigit(tok_val[0]) && tok_val[0]!=CHAR_MINUS)
    {
        return 0;
    }

    /// Eger tokeniň birinji harpy '-' bolsa we tokenden öň komanda-da san bar bolsa
    ///     Token arifmetiki aýyrmak belgini aňladýar.
    /// Ýogsa
    ///     Token negatiw sany aňladýar.
    if (wcslen(tok_val)>=1 && tok_val[0]==ARIF_MINUS_CHAR &&
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
        if (!isdigit(tok_val[i]))
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

    if (!isdigit(tok_val[0]) && tok_val[0]!=L'-' && tok_val[0]!=FLOAT_CONST_DATA_SEPARATOR)
    {
        return 0;
    }
    if (wcslen(tok_val)>=1 && tok_val[0]==ARIF_MINUS_CHAR &&
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
        if (!isdigit(tok_val[i]))
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
int is_token_arif(token *tok, wchar_t *tok_val)
{
    if (wcslen(tok_val)!=1)
        return 0;

    int i;
    for (i=0; i<TOK_CLASS_ARIF_TYPES_NUM; ++i)
    {
        if (wcsncmp(tok_val, TOK_CLASS_ARIF_CHARS[i][0], wcslen(TOK_CLASS_ARIF_CHARS[i][0]))==0)
        {
            token_type tok_type;
            tok_type.type_num   = i;	// Number of token type
            tok_type.type_class = TOK_CLASS_ARIF;
            tok_type.need_value = 0;
            tok_type.parenthesis = 1;
            tok_type.is_compl = 1;
            tok_type.type_must_check = 0;

            tok->is_compl     = 1;
            tok->type_class   = TOK_CLASS_ARIF;

            add_potentional_token_type(tok, tok_type);

            return 1;
        }
    }

    return 0;
}
int is_token_cmp(token *tok, wchar_t *tok_val)
{
    if (!wcslen(tok_val) || wcslen(tok_val)>2)
        return 0;

    int i;
    wchar_t found = 0;
    for (i=0; i<TOK_CLASS_CMP_TYPES_NUM; ++i)
    {

        if (wcslen(tok_val)==wcslen(TOK_CLASS_CMP_CHARS[i][0]) &&
            wcsncmp(tok_val, TOK_CLASS_CMP_CHARS[i][0], wcslen(TOK_CLASS_CMP_CHARS[i][0]))==0)
        {
            token_type tok_type;
            tok_type.type_num   = i;	// Number of token type
            tok_type.type_class = TOK_CLASS_CMP;
            tok_type.need_value = 0;
            tok_type.parenthesis = 1;
            tok_type.is_compl = 1;
            tok_type.type_must_check = 0;

            tok->is_compl     = 1;
            tok->type_class   = TOK_CLASS_CMP;

            add_potentional_token_type(tok, tok_type);

            found = 1;
        }
    }

    return found;
}
int is_token_logic(token *tok, wchar_t *tok_val)
{
    if (!wcslen(tok_val) || wcslen(tok_val)>2)
        return 0;

    int i;
    wchar_t found = 0;
    for (i=0; i<TOK_CLASS_LOGIC_TYPES_NUM; ++i)
    {

        if (wcslen(tok_val)==wcslen(TOK_CLASS_LOGIC_CHARS[i][0]) &&
            wcsncmp(tok_val, TOK_CLASS_LOGIC_CHARS[i][0], wcslen(TOK_CLASS_LOGIC_CHARS[i][0]))==0)
        {
            token_type tok_type;
            tok_type.type_num   = i;	// Number of token type
            tok_type.type_class = TOK_CLASS_LOGIC;
            tok_type.need_value = 0;
            tok_type.parenthesis = 1;
            tok_type.is_compl = 1;
            tok_type.type_must_check = 0;

            tok->is_compl     = 1;
            tok->type_class   = TOK_CLASS_LOGIC;

            add_potentional_token_type(tok, tok_type);

            found = 1;
        }
    }

    return found;
}
// eger, ýa, ýogsa, bolsa
int is_token_ctrl_sttmnt(token *tok, wchar_t *tok_val)
{
	/*Go through array of possible types*/
	int i, answer, found = 0;
	token_type tok_type;

	for(i=0; i<TOK_CLASS_CTRL_STTMNT_NUM; i++)
	{

		answer = wcsstr_by_offset(TOK_CLASS_CTRL_STTMNT_CHARS[i][0], tok_val, 0);
		if (answer>=0)
		{
			tok_type.type_num = i;							// Number of token type
			tok_type.type_class = TOK_CLASS_CTRL_STTMNT;
			tok_type.need_value = 0;
			tok_type.is_compl = (answer==0) ? 1 : 0;
			tok_type.type_must_check = 0;
			tok_type.parenthesis = 0;
			tok->is_compl = (answer==0) ? 1 : 0;

			add_potentional_token_type(tok, tok_type);

			// Tokene gornush girizilen son chagyrylmaly
			if (tok_type.is_compl==1)
				tok->type_class = TOK_CLASS_CTRL_STTMNT;
			found = 1;
		}
	}

	return found;
}
// ===
int is_token_block(token *tok, wchar_t *tok_val)
{
	/*Go through array of possible types*/
	int i, answer, found = 0;
	token_type tok_type;

	for(i=0; i<TOK_CLASS_BLOCK_NUM; i++)
	{

		answer = wcsstr_by_offset(TOK_CLASS_BLOCK_CHARS[i][0], tok_val, 0);
		if (answer>=0)
		{
			tok_type.type_num = i;							// Number of token type
			tok_type.type_class = TOK_CLASS_BLOCK;
			tok_type.need_value = 0;
			tok_type.is_compl = (answer==0) ? 1 : 0;
			tok_type.type_must_check = 0;
			tok_type.parenthesis = 0;
			tok->is_compl = (answer==0) ? 1 : 0;

			add_potentional_token_type(tok, tok_type);

			// Tokene gornush girizilen son chagyrylmaly
			if (tok_type.is_compl==1)
				tok->type_class = TOK_CLASS_BLOCK;
			found = 1;
		}
	}

	return found;
}
// eger, ýa, ýogsa, bolsa
int is_token_loop_sttmnt(token *tok, wchar_t *tok_val)
{
	/*Go through array of possible types*/
	int i, answer, found = 0;
	token_type tok_type;

	for(i=0; i<TOK_CLASS_LOOP_STTMNT_NUM; i++)
	{

		answer = wcsstr_by_offset(TOK_CLASS_LOOP_STTMNT_CHARS[i][0], tok_val, 0);
		if (answer>=0)
		{
			tok_type.type_num = i;							// Number of token type
			tok_type.type_class = TOK_CLASS_LOOP_STTMNT;
			tok_type.need_value = 0;
			tok_type.is_compl = (answer==0) ? 1 : 0;
			tok_type.type_must_check = 0;
			tok_type.parenthesis = 0;
			tok->is_compl = (answer==0) ? 1 : 0;

			add_potentional_token_type(tok, tok_type);

			// Tokene gornush girizilen son chagyrylmaly
			if (tok_type.is_compl==1)
				tok->type_class = TOK_CLASS_LOOP_STTMNT;
			found = 1;
		}
	}

	return found;
}// eger, ýa, ýogsa, bolsa
int is_token_triangle_block(token *tok, wchar_t *tok_val)
{
	/*Go through array of possible types*/
	int i, answer, found = 0;
	token_type tok_type;

	for(i=0; i<TRIANGLE_BLOCK_TOKENS_NUM; i++)
	{

		answer = wcsstr_by_offset(TRIANGLE_BLOCK_TOKENS_KEYWORDS[i][0], tok_val, 0);
		if (answer>=0)
		{
			tok_type.type_num = i;							// Number of token type
			tok_type.type_class = TOK_CLASS_TRIANGLE_BLOCK;
			tok_type.need_value = 0;
			tok_type.is_compl = (answer==0) ? 1 : 0;
			tok_type.type_must_check = 0;
			tok_type.parenthesis = 0;
			tok->is_compl = (answer==0) ? 1 : 0;

			add_potentional_token_type(tok, tok_type);

			// Tokene gornush girizilen son chagyrylmaly
			if (tok_type.is_compl==1)
				tok->type_class = TOK_CLASS_TRIANGLE_BLOCK;
			found = 1;
		}
	}

	return found;
}
int is_token_utype(token *tok, wchar_t *tok_val)
{
	/*Go through array of possible types*/
	int i, answer, found = 0;
	token_type tok_type;

	answer = wcsstr_by_offset(UTYPE_DEF_SIGNIFIER_WORD, tok_val, 0);
	if (answer>=0)
    {
        tok_type.type_num = UTYPE_DEF_SIGNIFIER;							// Number of token type
        tok_type.type_class = TOK_CLASS_UTYPE;
        tok_type.need_value = 0;
        tok_type.is_compl = (answer==0) ? 1 : 0;
        tok_type.type_must_check = 0;
        tok_type.parenthesis = 0;
        tok->is_compl = (answer==0) ? 1 : 0;

        add_potentional_token_type(tok, tok_type);

        // Tokene gornush girizilen son chagyrylmaly
        if (tok_type.is_compl==1)
            tok->type_class = TOK_CLASS_UTYPE;
        found = 1;
	}

    /// Birinji Ulanyjynyň tipi bilen bagly token tanalmady.
    /// Ikinji token görnüşi bilen deňeşdirip görülýär
	if (!found)
    {
        answer = wcsstr_by_offset(TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[0], tok_val, 0);
        if (answer>=0)
        {
            tok_type.type_num = UTYPE_ITEM_SEPARATOR;							// Number of token type
            tok_type.type_class = TOK_CLASS_UTYPE;
            tok_type.need_value = 0;
            tok_type.is_compl = (answer==0) ? 1 : 0;
            tok_type.type_must_check = 0;
            tok_type.parenthesis = 0;
            tok->is_compl = (answer==0) ? 1 : 0;

            add_potentional_token_type(tok, tok_type);

            // Tokene gornush girizilen son chagyrylmaly
            if (tok_type.is_compl==1)
                tok->type_class = TOK_CLASS_UTYPE;
            found = 1;
        }
    }

	return found;
}
int is_token_utype_con(token *tok, wchar_t *tok_val)
{
    if (is_utype_ident(tok_val))
    {
        token_type tok_type;
        get_utype_addr(tok_val, &tok_type.type_num);							// Number of token type
        tok_type.type_class = TOK_CLASS_UTYPE_CON;
        tok_type.need_value = 1;
        wcsncpy(tok_type.value, tok_val, wcslen(tok_val)+1);
        tok_type.is_compl = 1;
        tok_type.type_must_check = 0;
        tok_type.parenthesis = 0;
        tok->is_compl = 1;
        tok->type_class = TOK_CLASS_UTYPE_CON;

        add_potentional_token_type(tok, tok_type);
        return 1;
    }
    return 0;
}
int is_token_void(token *tok, wchar_t *tok_val)
{
	/*Go through array of possible types*/
	int i, answer, found = 0;
	token_type tok_type;

	answer = wcsstr_by_offset(TOK_VOID_CHARS[0][0], tok_val, 0);
    if (answer>=0)
    {
        tok_type.type_num = 0;							// Number of token type
        tok_type.type_class = TOK_CLASS_VOID;
        tok_type.need_value = 0;
        tok_type.is_compl = (answer==0) ? 1 : 0;
        tok_type.type_must_check = 0;
        tok_type.parenthesis = 1;
        tok->is_compl = (answer==0) ? 1 : 0;

        add_potentional_token_type(tok, tok_type);

        // Tokene gornush girizilen son chagyrylmaly
        if (tok_type.is_compl==1)
            tok->type_class = TOK_CLASS_VOID;
        found = 1;
    }

	return found;
}
int is_token_comment(token *tok, wchar_t *tok_val)
{
	/*Go through array of possible types*/
	int i, complete = 0, len = wcslen(tok_val), type;
	token_type tok_type;

    if (len>0)
    {
        if (tok_val[0]!=L'/')
            return 0;
        if (len>1)
        {
            if (tok_val[1]==L'/')
            {
                type     = TOK_CLASS_COMMENT_SINGLE_TYPE;
                if (len==2)
                {
                    complete = 0;
                }
                else if (tok_val[len-1]==L'\n')
                {
                    complete = 1;
                }
                else if (tok->potentional_types[0].is_compl)
                {
                    return 0;
                }
            }
            else if (tok_val[1]=='*')
            {
                type     = TOK_CLASS_COMMENT_BLOCK_TYPE;
                if (len==2)
                {
                    complete = 0;
                }
                else if (len>2)
                {
                    if (tok_val[len-1]==L'*')
                    {
                        complete = 0;
                    }
                    else if (len>3 && tok_val[len-1]==L'/' && tok_val[len-2]==L'*')
                    {
                        complete = 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                return 0;
            }

            /// Maglumaty tokene salmaly
            tok_type.type_num = type;							// Number of token type
            tok_type.type_class = TOK_CLASS_COMMENT;
            tok_type.need_value = 0;
            tok_type.is_compl = complete;
            tok_type.type_must_check = 0;
            tok_type.parenthesis = 1;
            tok->is_compl = complete;
            tok->type_class = TOK_CLASS_COMMENT;

            add_potentional_token_type(tok, tok_type);
            return 1;
        }
    }

	return 0;
}
