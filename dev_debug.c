/*
 * This file includes functions for debugging while compiler building
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "tokens.h"
#include "cmds.h"
#include "algor.h"
#include "paren/types.h"
#include "dev_debug.h"
#include "main/user_def_type.h"

int DEV_ITEMS_STEPS = 0;

/** ŞERT #6: Debaglamanyň üçünji görnüşini düzmeli.
    ** Birinji görnüşinde ýöne ýalňyşlyk görkezýärdi.
       Ikinji görnüşinde ýalňyşlygyň nirede (haýsy faýlda, setirde, harpda) bolandygyny görkezýärdi.
       Indi üçünji görnüşini ýasamaly.
    Üçünji görnüşinde, içki birlikler bir TAB çepe süýşürilip tämiz görkezilmeli.

    1) Debaglanýan tekstiň häzir haýsy birligiň içindeliginiň basgançagyny görkezýän global ülňi bolýar.
    Adatça ol 0-njy derejede bolýar.
    2) Funksiýa berlen basgançagyň sanyna görä '\t' harplary çap edýär.

    3) Komanda, ýaý debaglanýan wagty, içindäki elementlere gezek ýetende,
       birlikleriň içindeliginiň basgançagy bir basgançak ulaldylýar.
    4) Komanda, ýaý debaglanýan wagty, içindäki elementler çap edilen soň,
       birlikleriň içindeliginiň basgançagy bir basgançak kemeldilýär.
*/

void debug_cmd(command *cmd)
{
	char *cmd_class = get_cmd_class(cmd->cmd_class);
	char *cmd_type = get_cmd_class_type(cmd->cmd_class, cmd->cmd_type);

	char *complete = (cmd->is_compl) ? "Gutaran" : "Gutarylmadyk";

	char *ns = (cmd->ns==1)?"lokal":"global";

    printf("\n");
    print_tabs(DEV_ITEMS_STEPS);
	printf("KOMANDANYŇ MAGLUMATY------------------------\n");
	print_tabs(DEV_ITEMS_STEPS);
	printf("('%s') '%s'. '%s'->'%s' görnüşdäki komanda\n", ns, complete, cmd_class, cmd_type);
	print_tabs(DEV_ITEMS_STEPS);
	printf("%d sany birlikleri bar", cmd->items_num);
	if (cmd->items_num>0)
	{
		int i;
		++DEV_ITEMS_STEPS;
		printf(". Olar:\n");
		//printf("  potentional types: %d", tok->potentional_types_num);
		for(i=0; i<cmd->items_num; i++)
		{
		    command_item *ci = get_cmd_item(cmd->items , i);
			if (ci->type==TOKEN_ITEM)
				debug_token(&ci->tok);
			else if(ci->type==CMD_ITEM)
				debug_cmd(&ci->cmd);
            else if(ci->type==PAREN_ITEM)
                debug_paren(&ci->paren);
		}
		--DEV_ITEMS_STEPS;
	}
	else
    {
        printf("\n");
    }

}

void debug_token(token *tok)
{
	char *ns = (tok->ns==1)?"Lokal":"Global";

	char *type_class = get_type_class(tok->type_class);

	char *complete = (tok->is_compl) ? "Gutaran" : "Gutarylmadyk";

	// Print results
	printf("\n");
    print_tabs(DEV_ITEMS_STEPS);
	printf("Tokeniň maglumatlary------------------------\n");
    print_tabs(DEV_ITEMS_STEPS);
	printf("Faýl: '%s'; Setir: %d; Başlaýan harpy: %c, harpyň nomeri: %d\n", FILES[tok->inf_file_num].source, tok->inf_line_num,
        tok->inf_char==-1 ? '?' : tok->inf_char, tok->inf_char_num);
    print_tabs(DEV_ITEMS_STEPS);
	printf("(%s) '%s'. '%s' görnüş klasyndaky token\n", ns, complete, type_class);
	print_tabs(DEV_ITEMS_STEPS);
	printf("Bolup biljek görnüşleriniň sany:%d\n", tok->potentional_types_num);
	if (tok->potentional_types_num>0)
	{
		int i;
		++DEV_ITEMS_STEPS;
		for(i=0; i<tok->potentional_types_num; i++)
			debug_token_type(&tok->potentional_types[i]);
        --DEV_ITEMS_STEPS;
	}

}


void debug_token_type(token_type *tok_type)
{
	char type[256];
	char *complete = (tok_type->is_compl) ? "Gutaran" : "Gutarylmadyk";
	char *type_class = get_type_class(tok_type->type_class);

	if (tok_type->type_class==TOK_CLASS_DEF_TYPE)
		strncpy(type, def_type_list[tok_type->type_num].tk_name, strlen(def_type_list[tok_type->type_num].tk_name)+1);
	else if (tok_type->type_class==TOK_CLASS_ASSIGN)
	{
		strncpy(type, ASSIGN_TOK_NUM_WORDS[tok_type->type_num], strlen(ASSIGN_TOK_NUM_WORDS[tok_type->type_num])+1);
	}
	else if (tok_type->type_class==TOK_CLASS_CONST_DATA)
    {
        //printf("%s %d", CONST_DATA_TOK_NUM_WORDS[tok_type->type_num], tok_type->type_num);
        strncpy(type, CONST_DATA_TOK_NUM_WORDS[tok_type->type_num], strlen(CONST_DATA_TOK_NUM_WORDS[tok_type->type_num])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_ARIF)
    {
        strncpy(type, ARIF_TOK_NUM_WORDS[tok_type->type_num], strlen(ARIF_TOK_NUM_WORDS[tok_type->type_num])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_CMP)
    {
        strncpy(type, CMP_TOK_NUM_WORDS[tok_type->type_num], strlen(CMP_TOK_NUM_WORDS[tok_type->type_num])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_LOGIC)
    {
        strncpy(type, LOGIC_TOK_NUM_WORDS[tok_type->type_num], strlen(LOGIC_TOK_NUM_WORDS[tok_type->type_num])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_CTRL_STTMNT)
    {
        strncpy(type, TOK_CLASS_CTRL_STTMNT_WORDS[tok_type->type_num], strlen(TOK_CLASS_CTRL_STTMNT_WORDS[tok_type->type_num])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_BLOCK)
    {
        strncpy(type, TOK_CLASS_BLOCK_WORDS[tok_type->type_num], strlen(TOK_CLASS_BLOCK_WORDS[tok_type->type_num])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_LOOP_STTMNT)
    {
        strncpy(type, TOK_CLASS_LOOP_STTMNT_WORDS[tok_type->type_num], strlen(TOK_CLASS_LOOP_STTMNT_WORDS[tok_type->type_num])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_TRIANGLE_BLOCK)
    {
        strncpy(type, TRIANGLE_BLOCK_TOKENS_KEYWORDS[tok_type->type_num][1], strlen(TRIANGLE_BLOCK_TOKENS_KEYWORDS[tok_type->type_num][1])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_UTYPE)
    {
        if (tok_type->type_num==UTYPE_DEF_SIGNIFIER)
            strncpy(type, UTYPE_DEF_SIGNIFIER_DB_WORD, strlen(UTYPE_DEF_SIGNIFIER_DB_WORD)+1);
        else if(tok_type->type_num==UTYPE_ITEM_SEPARATOR)
            strncpy(type, TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[2], strlen(TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[2])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_UTYPE_CON)
    {
        int utype_num = tok_type->type_num;
        strncpy(type, USER_DEF_TYPES[utype_num].ident, strlen(USER_DEF_TYPES[utype_num].ident)+1);
    }
    else if (tok_type->type_class==TOK_CLASS_VOID)
    {
        int type_num = tok_type->type_num;
        strncpy(type, TOK_VOID_CHARS[type_num][1], strlen(TOK_VOID_CHARS[type_num][1])+1);
    }
    else
		strncpy(type, "", strlen("")+1);

    printf("\n");
    print_tabs(DEV_ITEMS_STEPS);
	printf("('%s') '%s'->'%s' token tipi\n", complete, type_class, type);

    print_tabs(DEV_ITEMS_STEPS);
	if (tok_type->need_value==0)
		printf("Maglumat saklanok\n");
	else
    {
		printf("Maglumat: '%s'\n", get_tok_type_value(tok_type));
    }
	printf("\n");
}

char *get_tok_type_value(token_type *tok_type)
{
    if (tok_type->type_class==TOK_CLASS_CONST_DATA && tok_type->type_num==STRING_CONST_DATA_TOK_NUM)
    {
        return (char *)get_string_item(tok_type->string_value);
    }
    return tok_type->value;
};

char *get_type_class(int type_class_num)
{
	if (type_class_num>0)
		return type_classes[type_class_num];
    printf("%d\n", type_class_num);
	return "Tipin klasy nabelli";
}

char *get_cmd_class(int cmd_class_num)
{
	if (cmd_class_num>0)
		return cmd_classes[cmd_class_num-1];

	return "Komandanyň klasy näbelli";
}

char *get_cmd_class_type(int cmd_class_num, int cmd_type_num)
{
	if (cmd_class_num>0 && cmd_type_num>0)
		return cmd_class_types[cmd_class_num-1][cmd_type_num-1];

	return "Komandanyň tipi näbelli";
}

void debug_GLOB_VAR_DEFS()
{
	printf("\tGLOBAL YGLAN EDILEN ÜLŇILER------------------\n");

    printf("\n");
    int i;
    for(i=0; i<GLOBAL_VAR_DEFS_NUMS; ++i)
    {
        char *type_class = get_type_class(GLOBAL_VAR_DEFS[i].type_class);

        const char *type       = def_type_list[ GLOBAL_VAR_DEFS[i].type_num ].tk_name;
        printf("\t%d. %s faýlda, (%s) %s %s\n", i+1,
            FILES[GLOBAL_VAR_DEFS[i].inf_file_num].source,
            type_class,
            type,
            GLOBAL_VAR_DEFS[i].name);
    }
	printf("\n\n\n");
}


void debug_LOCAL_VAR_DEFS()
{
	printf("\tLOKAL YGLAN EDILEN ÜLŇILER------------------\n");

	printf("\n");
    int i;
    for(i=0; i<LOCAL_VAR_DEFS_NUMS; ++i)
    {
        char *type_class = get_type_class(LOCAL_VAR_DEFS[i].type_class);

        const char *type       = def_type_list[LOCAL_VAR_DEFS[i].type_num ].tk_name;
        printf("\t%d. %s faýlda, (%s) %s %s\n", i+1,
            CUR_FILE_NAME,
            type_class,
            type,
            LOCAL_VAR_DEFS[i].name);
    }

	printf("\n\n\n");
}



void debug_paren(parenthesis *paren)
{
	char *paren_type  = get_paren_type(paren->type);

    printf("\n");
    print_tabs(DEV_ITEMS_STEPS);
	printf("ÝAÝYŇ MAGLUMATLARY------------------------------\n");
	print_tabs(DEV_ITEMS_STEPS);
	printf("'%s' görnüşindäki ýaý\n", paren_type);
	print_tabs(DEV_ITEMS_STEPS);
	printf("Birlikleriniň sany: %d", paren->elems_num);
	if (paren->elems_num>0)
	{
	    parenthesis_elem *p_es = get_paren_elems(paren->elems);

		int i;
		printf(". Olar:\n");
		++DEV_ITEMS_STEPS;
		//printf("  potentional types: %d", tok->potentional_types_num);
		for(i=0; i<paren->elems_num; i++)
		{
			if (p_es[i].type==TOKEN_ITEM)
				debug_token(&p_es[i].tok);
			else if(p_es[i].type==CMD_ITEM)
				debug_cmd(&p_es[i].cmd);
            else if(p_es[i].type==PAREN_ITEM)
                debug_paren(&p_es[i].paren);
		}
		--DEV_ITEMS_STEPS;
	}
	else
    {
		printf("\n");
    }

}


char *get_paren_type(int paren_type)
{
    return PAREN_TYPES_WORDS[paren_type];
}


void print_tabs(int tabs_num)
{
    if (!tabs_num)
        return;
    char *str = malloc(sizeof(char)*(tabs_num+1));

    strncpy(str, "\t", strlen("\t")+1);

    int i;
    for (i=1; i<tabs_num; ++i)
    {
        strncat(str, "\t", strlen("\t"));
    }
    printf("%s", str);
    free(str);
}
