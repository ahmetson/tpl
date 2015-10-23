/**
 * Tokenlerin tiplerinin bilen bagly ahli maglumatlar.
 * Eyyam bolup biljek tiplerin hemmesi yglan edilen son doredilmeli
**/
#include <stdio.h>
#include <string.h>

#include "token_types/assign.h"
#include "token_types/glob.h"
#include "token_types/def_types.h"
#include "token_types/const_data.h"
#include "keywords.h"
#include "token_types.h"

// 3 sany tokenlerin tipleri bar
const int TOKEN_TYPES_NUM = CONST_TOKEN_TYPES_NUM;

// In uly token 'ident' - 6 harpdan ybarat
const int TOKEN_TYPE_MAX_LEN = CONST_TOKEN_TYPE_MAX_LEN;

// Nache sany token tip bar bolsa, shonchada klas bar
const int TOK_CLASS_DEF_TYPE 	 = 1;
const int TOK_CLASS_GLOB 		 = 2;
const int TOK_CLASS_IDENT        = 3;
const int TOK_CLASS_ASSIGN		 = 4;
const int TOK_CLASS_CONST_DATA   = 5;

// Used for debugging
char *type_classes[] = {
	"def_type",
	"glob",
	"ident",
	"assign",
	"const_data"
};


// tokenleri tanayan funksiyalar
int is_token_def_type(token *tok, char *tok_val)
{
	//printf("Called is_token_tip\nReturned 1");
	/*Go through array of possible types*/
	int i, answer, found = 0;
	token_type tok_type;

	for(i=0; i<DEF_TYPES_NUM; i++)
	{

		answer = strstr_by_offset(def_type_list[i].tk_name, tok_val, 0);
		if (answer>=0)
		{
			tok_type.type_num = i;							// Number of token type
			tok_type.type_class = TOK_CLASS_DEF_TYPE;
			tok_type.need_value = 0;
			tok_type.is_compl = (answer==0) ? 1 : 0;
			tok_type.type_must_check = 0;
			tok->is_compl = (answer==0) ? 1 : 0;

			add_potentional_token_type(tok, tok_type);

			// Tokene gornush girizilen son chagyrylmaly
			if (tok_type.is_compl==1)
				tok->type_class = TOK_CLASS_DEF_TYPE;

			// DEBUG
			//printf("BARLAG: %s; %s; %d; %d; %d;\n", def_type_list[i].tk_name, tok->token_class, tok->potentional_types[0].num, tok->potentional_types_num,
			//		tok->potentional_types[0].is_compl
			//);

			found = 1;
		}
	}

	return found;
}

int is_token_def_glob(token *tok, char *tok_val)
{
	int answer;
	token_type tok_type;

	answer = strstr_by_offset(glob_def_string, tok_val, 0);
	if (answer>=0)
	{
		tok_type.type_num = 0;							// Number of token type
		tok_type.type_class = TOK_CLASS_GLOB;
		tok_type.need_value = 0;
		tok_type.is_compl = (answer==0) ? 1 : 0;
		tok_type.type_must_check = 0;
		tok->is_compl = (answer==0) ? 1 : 0;

		add_potentional_token_type(tok, tok_type);

		// Tokene gornush girizilen son chagyrylmaly
		if (tok_type.is_compl==1)
			tok->type_class = TOK_CLASS_GLOB;

		return 1;
	}
	return 0;
}

int is_token_ident(token *tok, char *tok_val)
{
	token_type tok_type;
	//printf("Identifikator barlagy: %s\n", tok_val->val);

	// Identifikatorlar birinji harpy harp bolmaly
	if (!isalpha(tok_val[0]))
		return 0;
	//printf("Identifikator harpdan bashlayar\n");

	int i;
	// Identifikator achar sozi bolup bilenok
	for(i=0; i<MAX_KEYWORDS_NUM; i++)
	{
		if (strlen(keywords[i])!=strlen(tok_val))
			continue;
		//len = (strlen(keywords[i])>strlen(tok_val->val)) ? strlen(keywords[i]) : strlen(tok_val->val);

		//printf("For %s, Result of comparing: %0d\n", tok_val->val, strncmp(tok_val->val, keywords[i], strlen(keywords[i])));
		if (strncmp(tok_val, keywords[i], strlen(keywords[i]) )==0 )
			return 0;
	}
	//printf("Identifikator achar sozi dal\n");

	// Identifikator harplardan, sanlardan we '_' belgiden durmaly
	for(i=0; i<strlen(tok_val); i++)
	{
		if (isalnum(tok_val[i])==0 && tok_val[i]!='_')
			return 0;
	}
	//printf("Identifikator harpdan, sandan we '_' belgiden duryar\n");

	// This is - identifikator
	tok_type.type_num = 0;							// Number of token type

	tok_type.need_value = 1;
	tok_type.type_class = TOK_CLASS_IDENT;
	strncpy(tok_type.value, tok_val, strlen(tok_val)+1);
	tok_type.is_compl = 1;
	tok_type.type_must_check = 0;
	tok->is_compl = 1;

	//debug_token(tok);
	//printf("Tokenin bolup biljek tipleri: <%s>\n", tok->potentional_types[0].value);

	add_potentional_token_type(tok, tok_type);
	//printf("Tokenin tipi goshuldymy: %0d\n", );

	// Tokene gornush girizilen son chagyrylmaly
	if (tok_type.is_compl==1)
		tok->type_class = TOK_CLASS_IDENT;

	return 1;
}


int is_token_var_left_assign(token *tok, char *tok_val)
{
	int answer;
	token_type tok_type;

	answer = strstr_by_offset(LEFT_ASSIGN_TOKEN_VALUE, tok_val, 0);
	if (answer>=0)
	{
		tok_type.type_num = LEFT_ASSIGN_TOK_NUM;	// Number of token type
		tok_type.type_class = TOK_CLASS_ASSIGN;
		tok_type.need_value = 0;
		tok_type.is_compl = (answer==0) ? 1 : 0;
		tok_type.type_must_check = 0;
		tok->is_compl = (answer==0) ? 1 : 0;

		add_potentional_token_type(tok, tok_type);

		// Tokene gornush girizilen son chagyrylmaly
		if (tok_type.is_compl==1)
			tok->type_class = TOK_CLASS_ASSIGN;

		return 1;
	}
	return 0;
}


int is_token_int_const_data(token *tok, char *tok_val)
{
    int i;
    for (i=0; i<strlen(tok_val); ++i)
    {
        if (!isdigit(tok_val[i]))
            return 0;
    }

    token_type tok_type;
    tok_type.type_num = INT_CONST_DATA_TOK_NUM;	// Number of token type
    tok_type.type_class = TOK_CLASS_CONST_DATA;
    tok_type.need_value = 1;
    strncpy(tok_type.value, tok_val, strlen(tok_val)+1);
    tok_type.is_compl = 1;
    tok_type.type_must_check = 0;

    tok->is_compl     = 1;
    tok->type_class   = TOK_CLASS_CONST_DATA;

	add_potentional_token_type(tok, tok_type);

    return 1;
}


int is_token_float_const_data(token *tok, char *tok_val)
{
    int i, dot=0;
    for (i=0; i<strlen(tok_val); ++i)
    {
        if (!isdigit(tok_val[i]))
        {
            if (tok_val[i]==FLOAT_CONST_DATA_SEPARATOR && !dot)
            {
                // Ýasaljak kodda, droblar kodly faýlynyňkydan üýtgeşik bölüniji arkaly tapawutlanmaly.
                tok_val[i] = FLOAT_CONST_DATA_C_CODE_SEPARATOR;

                // Indiki sanlar drobyň galyndylarynyňky
                dot = 1;
            }
            else
                return 0;
        }
    }

    token_type tok_type;
    tok_type.type_num   = FLOAT_CONST_DATA_TOK_NUM;	// Number of token type
    tok_type.type_class = TOK_CLASS_CONST_DATA;
    tok_type.need_value = 1;
    strncpy(tok_type.value, tok_val, strlen(tok_val)+1);
    tok_type.is_compl = 1;
    tok_type.type_must_check = 0;

    tok->is_compl     = 1;
    tok->type_class   = TOK_CLASS_CONST_DATA;

	add_potentional_token_type(tok, tok_type);

    return 1;
}


int is_token_char_const_data    (token *tok, char *tok_val)
{

    int len = strlen(tok_val), complete=0;
    // TODO  strlen(tok_val)>3 komandany
    // TPL_ESC_KEY_MAX_LEN + 2 (açar we ýapar) komandasy bilen calyşmaly
    if (len>3 || tok_val[0]!='\'' )
        return 0;

    // Eger ýörite belginiň başyny aňladýan harp bolup, harpdan soňam başga harp bar bolsa,
    // is_tpl_ESC_key() atly funksiýa arkaly ýörite belgileriň arasynda barlanýar.
    // Eger şeýle ýörite belgi bar bolsa, tpl_to_c_ESC_key(char *s) atly funksiýa arkaly,
    // ýörite belgi ýasaljak koddaky harp ekwiwalentligi bilen çalyşylmaly
    /*
    çhar ESC_key = 0;
    int  ESC_key_len = 0;
    if (tok_val[1]==TPL_ESC_KEY && ;en!=3)
    {
        ESC_key=1;
        if (is_tpl_ESCK_key(tok_val[2]))
        {
            ESC_key_len = tpl_ESC_key_to_c_ESC_key(tok_val);
        }
        else
        {
            // Tokeniň içinde ESC belgi bolmasa, token maksimum 4 belgiden ybarat bolmaly.
            return 0;
        }
    }*/

    // Minimum şu maglumatyň ybarat bolup bilýän harplary: 3 sany
    // Soňky harp durnak bolmasa, onda bu harp däl.
    // len==3 aslynda
    // // Token maglumatyn ichinde ESC belgi uygtedileni sebapli, uzynlyga tazeden hasaplanmaly
    // (ESC_key && (ESC_key_len+2)==strlen(tok_val)) || len==3 diyen komanda owrulmeli.
    // Sebabi, ESC belgi bar bolsa onun uzynlygy birnache harpdan ybarat bolup bilyar
    if (len==3)
    {
        if (tok_val[len-1]=='\'')
            complete = 1;
        else
            return 0;
    }

    token_type tok_type;
    tok_type.type_num   = CHAR_CONST_DATA_TOK_NUM;	// Number of token type
    tok_type.type_class = TOK_CLASS_CONST_DATA;
    tok_type.need_value = 1;
    strncpy(tok_type.value, tok_val, strlen(tok_val)+1);
    tok_type.is_compl = complete;
    tok_type.type_must_check = 0;

    tok->is_compl     = complete;
    tok->type_class   = TOK_CLASS_CONST_DATA;

	add_potentional_token_type(tok, tok_type);

    return 1;
}
