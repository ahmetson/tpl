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
#include "token/harpl.h"

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
    char *cmd_class = get_cmd_class(cmd->cmd_class),
         *cmd_type  = get_cmd_class_type(cmd->cmd_class, cmd->cmd_type),
         *complete  = (cmd->is_compl) ? "Gutaran" : "Gutarylmadyk",
         *ns        = (cmd->ns==1) ? "lokal" : "global";

    printf("\n");
    print_tabs(DEV_ITEMS_STEPS);
	printf( "%ls", L"KOMANDANYŇ MAGLUMATY------------------------\n" );
	print_tabs(DEV_ITEMS_STEPS);
	printf( "('%s') '%s'. '%s'->'%s' %ls\n", ns, complete, cmd_class, cmd_type, L"görnüşdäki komanda" );
	print_tabs(DEV_ITEMS_STEPS);
	printf("%d sany birlikleri bar", cmd->items_num);
	if (cmd->items_num>0)
	{
		int i;
		++DEV_ITEMS_STEPS;
		printf(". Olar:\n");
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
	char *ns = (tok->ns==1) ? "Lokal" : "Global",
         *type_class = get_type_class(tok->type_class),
         *complete = (tok->is_compl) ? "Gutaran" : "Gutarylmadyk";

	// Çap edilýär
	printf("\n");
    print_tabs(DEV_ITEMS_STEPS);
	printf("%ls", L"Tokeniň maglumatlary------------------------\n");
    print_tabs(DEV_ITEMS_STEPS);
	printf("%ls: '%ls'; Setir: %d; %ls: %lc, %ls: %d\n", L"Faýl", FILES[tok->inf_file_num].source, tok->inf_line_num,
        L"Başlaýan harpy", tok->inf_wchar_t==-1 ? L'?' : tok->inf_wchar_t, L"harpyň nomeri", tok->inf_wchar_t_num);
    print_tabs(DEV_ITEMS_STEPS);
	printf("(%s) '%s'. '%s' %ls\n", ns, complete, type_class, L"görnüş klasyndaky token");
	print_tabs(DEV_ITEMS_STEPS);
	printf("%ls:%d\n", L"Bolup biljek görnüşleriniň sany", tok->potentional_types_num);
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
	wchar_t type[256] = {0};
	char *complete = (tok_type->is_compl) ? "Gutaran" : "Gutarylmadyk",
         *type_class = get_type_class(tok_type->type_class);

	if (tok_type->type_class==TOK_CLASS_DEF_TYPE)
		wcsncpy(type, def_type_list[tok_type->type_num].tk_name, wcslen(def_type_list[tok_type->type_num].tk_name)+1);
	else if (tok_type->type_class==TOK_CLASS_ASSIGN)
	{
		wcsncpy(type, ASSIGN_TOK_NUM_WORDS[tok_type->type_num], wcslen(ASSIGN_TOK_NUM_WORDS[tok_type->type_num])+1);
	}
	else if (tok_type->type_class==TOK_CLASS_CONST_DATA)
    {
        wcsncpy(type, CONST_DATA_TOK_NUM_WORDS[tok_type->type_num], wcslen(CONST_DATA_TOK_NUM_WORDS[tok_type->type_num])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_ARIF)
    {
        wcsncpy(type, ARIF_TOK_NUM_WORDS[tok_type->type_num], wcslen(ARIF_TOK_NUM_WORDS[tok_type->type_num])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_CMP)
    {
        wcsncpy(type, CMP_TOK_NUM_WORDS[tok_type->type_num], wcslen(CMP_TOK_NUM_WORDS[tok_type->type_num])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_LOGIC)
    {
        wcsncpy(type, LOGIC_TOK_NUM_WORDS[tok_type->type_num], wcslen(LOGIC_TOK_NUM_WORDS[tok_type->type_num])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_CTRL_STTMNT)
    {
        wcsncpy(type, TOK_CLASS_CTRL_STTMNT_WORDS[tok_type->type_num], wcslen(TOK_CLASS_CTRL_STTMNT_WORDS[tok_type->type_num])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_BLOCK)
    {
        wcsncpy(type, TOK_CLASS_BLOCK_WORDS[tok_type->type_num], wcslen(TOK_CLASS_BLOCK_WORDS[tok_type->type_num])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_LOOP_STTMNT)
    {
        wcsncpy(type, TOK_CLASS_LOOP_STTMNT_WORDS[tok_type->type_num], wcslen(TOK_CLASS_LOOP_STTMNT_WORDS[tok_type->type_num])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_TRIANGLE_BLOCK)
    {
        wcsncpy(type, TRIANGLE_BLOCK_TOKENS_KEYWORDS[tok_type->type_num][1], wcslen(TRIANGLE_BLOCK_TOKENS_KEYWORDS[tok_type->type_num][1])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_UTYPE)
    {
        if (tok_type->type_num==UTYPE_DEF_SIGNIFIER)
            wcsncpy(type, UTYPE_DEF_SIGNIFIER_DB_WORD, wcslen(UTYPE_DEF_SIGNIFIER_DB_WORD)+1);
        else if(tok_type->type_num==UTYPE_ITEM_SEPARATOR)
            wcsncpy(type, TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[2], wcslen(TOK_CLASS_UTYPE_ITEM_SEPARATOR_CHARS[2])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_UTYPE_CON)
    {
        int utype_num = tok_type->type_num;
        wcsncpy(type, USER_DEF_TYPES[utype_num].ident, wcslen(USER_DEF_TYPES[utype_num].ident)+1);
    }
    else if (tok_type->type_class==TOK_CLASS_VOID)
    {
        int type_num = tok_type->type_num;
        wcsncpy(type, TOK_VOID_CHARS[type_num][1], wcslen(TOK_VOID_CHARS[type_num][1])+1);
    }
    else if (tok_type->type_class==TOK_CLASS_VOID)
    {
        int type_num = tok_type->type_num;
        wchar_t *c = type_num==TOK_CLASS_COMMENT_SINGLE_TYPE?TOK_CLASS_COMMENT_SINGLE_DEBUG:TOK_CLASS_COMMENT_BLOCK_DEBUG;
        wcsncpy(type, c, wcslen(c)+1);
    }
    else
		wcsncpy(type, L"", wcslen(L"")+1);

    printf("\n");
    print_tabs(DEV_ITEMS_STEPS);
	printf("('%s') '%s'->'%ls' token tipi\n", complete, type_class, type);

    print_tabs(DEV_ITEMS_STEPS);
	if (tok_type->need_value==0)
		printf("Maglumat saklanok\n");
	else
    {
		printf("Maglumat: '%ls'\n", get_tok_type_value(tok_type) );
    }
	printf("\n");
}

wchar_t *get_tok_type_value(token_type *tok_type)
{
    if (tok_type->type_class==TOK_CLASS_CONST_DATA && tok_type->type_num==STRING_CONST_DATA_TOK_NUM)
    {
        return (wchar_t *)get_string_item(tok_type->string_value);
    }
    return tok_type->value;
};

char *get_type_class(int type_class_num)
{
	if (type_class_num>0)
		return type_classes[type_class_num];
	return "Unknown";
}

char *get_cmd_class(int cmd_class_num)
{
	if (cmd_class_num>0)
		return cmd_classes[cmd_class_num-1];

	return "Unknown";
}

char *get_cmd_class_type(int cmd_class_num, int cmd_type_num)
{
	if (cmd_class_num>0 && cmd_type_num>0)
		return cmd_class_types[cmd_class_num-1][cmd_type_num-1];

	return "Unknown";
}


void debug_paren(parenthesis *paren)
{
	char *paren_type  = get_paren_type(paren->type);

    printf("\n");
    print_tabs(DEV_ITEMS_STEPS);
	printf("%ls", L"ÝAÝYŇ MAGLUMATLARY------------------------------\n");
	print_tabs(DEV_ITEMS_STEPS);
	printf("'%s' %ls\n", paren_type, L"görnüşindäki ýaý" );
	print_tabs(DEV_ITEMS_STEPS);
	printf("%ls: %d", paren->elems_num, L"Birlikleriniň sany" );
	if (paren->elems_num>0)
	{
	    parenthesis_elem *p_es = get_paren_elems(paren->elems);

		int i;
		printf(". Olar:\n");
		++DEV_ITEMS_STEPS;
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
    char *tab = "\t";
    if (!tabs_num)
        return;
    char *str = malloc(sizeof(*tab)*(tabs_num+1));

    strncpy(str, tab, strlen(tab)+1);

    int i;
    for (i=1; i<tabs_num; ++i)
    {
        strncat(str, tab, strlen(tab));
    }
    printf("%s", str);
    free(str);
}
