/*
 * Tokenlerin tiplerinin bilen bagly ahli maglumatlar.
 * Eyyam bolup biljek tiplerin hemmesi yglan edilen son doredilmeli
**/

#include <stdio.h>
#include <string.h>

#include "def_types.h"
#include "glob.h"
#include "keywords.h"
#include "token_types.h"

// 3 sany tokenlerin tipleri bar
const int TOKEN_TYPES_NUM= 3;

// In uly token 'ident' - 6 harpdan ybarat
const int TOKEN_TYPE_MAX_LEN = 6;

// Nache sany token tip bar bolsa, shonchada klas bar
const int DEF_TYPE_TYPE_CLASS = 1;
const int GLOB_TYPE_CLASS = 2;
const int IDENT_TYPE_CLASS = 3;

// Used for debugging
char *type_classes[] = {
	"def_type",
	"glob",
	"ident"
};


// tokenleri tanayan funksiyalar
int is_token_def_type(token *tok, token_word *tok_val)
{
	//printf("Called is_token_tip\nReturned 1");
	/*Go through array of possible types*/
	int i, answer, found = 0;
	token_type tok_type;
	
	for(i=0; i<DEF_TYPES_NUM; i++)
	{ 

		answer = strstr_by_offset(def_type_list[i].tk_name, tok_val->val, 0);
		if (answer>=0)
		{
			tok_type.type_num = i;							// Number of token type
			tok_type.type_class = DEF_TYPE_TYPE_CLASS;
			tok_type.need_value = 0;
			tok_type.is_compl = (answer==0) ? 1 : 0;
			tok->is_compl = (answer==0) ? 1 : 0;
			
			add_potentional_token_type(tok, tok_type);
			
			// Tokene gornush girizilen son chagyrylmaly
			if (tok_type.is_compl==1)
				tok->type_class = DEF_TYPE_TYPE_CLASS;

			// DEBUG
			//printf("BARLAG: %s; %s; %d; %d; %d;\n", def_type_list[i].tk_name, tok->token_class, tok->potentional_types[0].num, tok->potentional_types_num,
			//		tok->potentional_types[0].is_compl
			//);
			
			found = 1;
		}
	}
	
	return found;
}

int is_token_def_glob(token *tok, token_word *tok_val)
{
	int answer;
	token_type tok_type;

	answer = strstr_by_offset(glob_def_string, tok_val->val, 0);
	if (answer>=0)
	{
		tok_type.type_num = 0;							// Number of token type
		tok_type.type_class = GLOB_TYPE_CLASS;
		tok_type.need_value = 0;
		tok_type.is_compl = (answer==0) ? 1 : 0;
		tok->is_compl = (answer==0) ? 1 : 0;	

		add_potentional_token_type(tok, tok_type);
		
		// Tokene gornush girizilen son chagyrylmaly
		if (tok_type.is_compl==1)
			tok->type_class = GLOB_TYPE_CLASS;
	
		return 1;
	}
	return 0;
}

int is_token_ident(token *tok, token_word *tok_val)
{
	token_type tok_type;
	//printf("Identifikator barlagy: %s\n", tok_val->val);
	
	// Identifikatorlar birinji harpy harp bolmaly
	if (!isalpha(tok_val->val[0]))
		return 0;
	//printf("Identifikator harpdan bashlayar\n");
	
	int i;
	// Identifikator achar sozi bolup bilenok
	for(i=0; i<MAX_KEYWORDS_NUM; i++)
	{
		if (strlen(keywords[i])!=strlen(tok_val->val))
			continue;
		//len = (strlen(keywords[i])>strlen(tok_val->val)) ? strlen(keywords[i]) : strlen(tok_val->val);
		
		//printf("For %s, Result of comparing: %0d\n", tok_val->val, strncmp(tok_val->val, keywords[i], strlen(keywords[i])));
		if (strncmp(tok_val->val, keywords[i], strlen(keywords[i]) )==0 )
			return 0;
	}
	//printf("Identifikator achar sozi dal\n");
	
	// Identifikator harplardan, sanlardan we '_' belgiden durmaly
	for(i=0; i<strlen(tok_val->val); i++)
	{
		if (isalnum(tok_val->val[i])==0 && tok_val->val[i]!='_')
			return 0;
	}
	//printf("Identifikator harpdan, sandan we '_' belgiden duryar\n");
	
	// This is - identifikator
	tok_type.type_num = 0;							// Number of token type
	
	tok_type.need_value = 1;
	tok_type.type_class = IDENT_TYPE_CLASS;
	strncpy(tok_type.value, tok_val->val, strlen(tok_val->val)+1);
	tok_type.is_compl = 1;
	tok->is_compl = 1;
	
	//debug_token(tok);
	//printf("Tokenin bolup biljek tipleri: <%s>\n", tok->potentional_types[0].value);
	
	add_potentional_token_type(tok, tok_type);
	//printf("Tokenin tipi goshuldymy: %0d\n", );

	// Tokene gornush girizilen son chagyrylmaly
	if (tok_type.is_compl==1)
		tok->type_class = IDENT_TYPE_CLASS;

	return 1;
}
