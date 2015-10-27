/*
 * Tokenler bilen işleýän bölüm
**/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// TPL.H global ülňileriň yglan edilmelerini saklaýar
#include "tpl.h"
#include "tokens.h"
#include "token/token_structs.h"
#include "dev_debug.h"
#include "error.h"

// Sozüň tokendigini bilmek üçin, şu sanawdan geçäýmeli
is_token_item tok_types[] = {
	   {is_token_def_type},
	   {is_token_def_glob},
	   {is_token_ident},
	   {is_token_var_left_assign},
	   {is_token_float_const_data},
	   {is_token_int_const_data},
	   {is_token_char_const_data}
};


// 'Default' maglumatlar goýulýar.
// Token ýasalan soň, hökman şu funksiýany çagyryp 'default' maglumatlar goýulmaly
void init_token(token *tok)
{
	tok->ns = 0;							// Global ýa Lokal

	int i; token_type tok_type;

	if (tok->potentional_types_num>=1 && tok->potentional_types_num<=TOKEN_TYPES_NUM-1)
	{
	    for(i=0; i<(tok->potentional_types_num); i++)
		{
		    //tok->potentional_types[i].string_value = NULL;
		    tok->potentional_types[i] = tok_type;
			//
		}
    }

    tok->potentional_types_num = 0;								// Number of recognized types for token
	//char source_file[MAXLEN];									// source file of token
	tok->type_class = 0;
	tok->value[0] = '\0';
	tok->is_compl = 0;

    tok->inf_char =
    tok->inf_char_num =
    tok->inf_file_num =
    tok->inf_line_num = -1;
}


void empty_token(token *tok)
{

	init_token(tok);

}


/*
 * Sözüň token bolup biljegini barlaýar.
 * Eger token bolup bilýän bolsa, @tok ülňä ýasalan tokeni baglaýar
**/
int recognize_token(token *tok, char *val)
{
	int i;
	// Harpl tokeni hasaplananok.
	for (i=0; i<TOKEN_TYPES_NUM-1; ++i) tok_types[i].is_token(tok, val);

	// Token not recognized
	if (tok->potentional_types_num==0)
	{
		//printf("Token not recognized\t(Inputted text:%s)\n", val);
		return 0;
	}

	//printf("Token Recognized\t(Inputted text:%s)\n", val);
	return 1;
}


/**
 * Tokenin bolup biljek tipini tokene goşýar.
 * Tokeniň bolup biljek tipleri ýene köpelýär
 * TODO: Eger tokenin eyyam goshuljak gornushinin klasynda gornushi bar bolsa,
 * we taze gornush gutarylan bolsa, onda onki gornush tazelenyar.
 *
**/
int add_potentional_token_type(token *tok, token_type tok_type)
{
	tok->potentional_types[tok->potentional_types_num++] = tok_type;
	return 1;
}


/**
 * Tokenin bolup biljek tipini tokene goşýar.
 * Tokeniň bolup biljek tipleri ýene köpelýär
 * TODO: Eger tokenin eyyam goshuljak gornushinin klasynda gornushi bar bolsa,
 * we taze gornush gutarylan bolsa, onda onki gornush tazelenyar.
 *
**/
int delete_potentional_token_type(token *tok, int type_class, int type_num)
{
    int i, j, moveto, potentional_types_num = tok->potentional_types_num;
    for (i=0; i<tok->potentional_types_num; ++i)
    {
        if (tok->potentional_types[i].type_class==type_class &&
            tok->potentional_types[i].type_num  ==type_num)
        {
            potentional_types_num--;
            // Eger pozulan tipden soň, ýene token tipleri bar bolsa,
            // olary öňe süýşirmeli
            if (i<tok->potentional_types_num-1)
            {
                moveto = 1;
            }
        }
        if (moveto)
        {
            for(j=i; j<tok->potentional_types_num; ++j)
            {
                if (j+1<tok->potentional_types_num)
                    tok->potentional_types[j] = tok->potentional_types[j+1];
            }
            moveto = 0;
        }
    }

    tok->potentional_types_num = potentional_types_num;

	return 1;
}

/*
 * Tokeniň içinden, komanda-da gerek bolmajak maglumatlary pozýar.
**/
int finishize_token(token *tok)
{
    // Hiç zat pozmak gerek däl
	if (!tok->potentional_types_num) return 0;

	// dine bir bolup biljek tip goyulyar,
	// in sonky goylan fayl, onki goyulanlary yapyar
	int i;
	for (i=tok->potentional_types_num-1;i>=0; --i)
	{
		if (tok->potentional_types[i].is_compl==0)
			continue;
		else
			tok->potentional_types[0] = tok->potentional_types[i];
	}
	tok->potentional_types_num = 1;
	tok->is_compl = tok->potentional_types[0].is_compl;

	return 1;
}


/**
 * Tokeniň içinde ulanyp boljak maglumat bardygyny barlaýar
**/
int is_token_empty(token *tok)
{
	return !tok->potentional_types_num;
}


/**
 * Tokeni komanda geçirýär.
 * Şowly goşulansoň, komandany tanajak bolýar
**/
int move_to_cmd(token *tok)
{
	int prev_part = CUR_PART;
	CUR_PART = 4;

	// Komandanyň öňki birligi gutarylan bolmaly
	if (cmd.items_num!=0 &&
       (cmd.items[cmd.items_num-1].type==TOKEN_ITEM && cmd.items[cmd.items_num-1].tok.is_compl==0 ||
        cmd.items[cmd.items_num-1].type==CMD_ITEM   && cmd.items[cmd.items_num-1].cmd.is_compl==0))
    {
        CUR_PART = 3;
        print_err(CODE3_PREV_TOK_INCORRECT, tok);
    }

    // Tokenden komanda goşulanda gerek bolmajak maglumatlar pozulýar
	finishize_token(tok);

	if (tok->is_compl==0)
    {
        CUR_PART = 2;
        print_err(CODE2_PREV_TOKEN_INCORRECT, tok);
    }

    //printf("Token komanda goshulmana tayynlandy\n");

	if (!add_to_cmd(&cmd, tok))  print_err(CODE4_CANT_ADD_TOKEN, tok);

	//printf("komanda barlanmana gechmeli\n");
	if (!parse_cmd(&cmd))
	{
		cmd.items_num--;
		if (!cmd.items_num || !parse_cmd(&cmd))
        {
            cmd.items_num++;
            //debug_cmd(&cmd);
			print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(&cmd));
        }
		else
		{
		    // Eger ulni yglan etme bolsa, onda ichki komandany ulni yglan etmelerin ichine gechirmeli
		    add_to_def_var_list(&cmd);

			// Ichki komandalaryn sanyny kopeltmeli
			GLOB_SUBCMDS_NUM++;
			GLOB_SUBCMD_ITEMS_LIST = realloc(GLOB_SUBCMD_ITEMS_LIST, sizeof(command_item*)*GLOB_SUBCMDS_NUM);
			if (GLOB_SUBCMD_ITEMS_LIST==NULL)
			{
				//printf("Ichki komandalar uchin yer taplymady\n");
				token *t=(token *)inf_get_last_token(&cmd);
				print_err(CODE4_CANT_IDENT_CMD, t);
			}
			else
			{
				//printf("Ichki komandalar uchin yer bar\n");

				// Ichki komanda uchin yer.
				long size = cmd.items_num * sizeof(command_item);
				GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1] = malloc(size);

				// Ichki komanda uchin yere, birlikler gechirilyar.
				int i=0;
				for (i=0; i<cmd.items_num; ++i)
				{
					GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1][i] = cmd.items[i];
				}

				command new_cmd;
				new_cmd = cmd;
				new_cmd.items = GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1];

				init_cmd(&cmd, 0);
				cmd.items_num = 2;

				command_item *tmp = realloc(cmd.items, cmd.items_num*sizeof(command_item));
				if (tmp==NULL)
				{
					//printf("Komanda ichki komandany goshmak uchin yer tapylmady\n");
					print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(&cmd));
				}
				else
				{
					//printf("Komanda ichki komandany goshmak uchin yer bar\n");
					cmd.items = tmp;

					// Komandanyň birinji birligi - Önki komanda goşulýar
					command_item cmd_item = {};
					cmd_item.type = 2;
					cmd_item.cmd = new_cmd;
					cmd.items[0] = cmd_item;

					// Komandanyň ikinji birligi   - Täze token goşulýar
					command_item cmd_tok_item;
					cmd_tok_item.type = 1;
					cmd_tok_item.tok = *tok;
					cmd.items[1] = cmd_tok_item;

					// Täze token öňki tokenlerden emele gelen komanda bilen täze komandany emele getirmedi
					if (!parse_cmd(&cmd))
                        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(&cmd));

				}
			}
		}
	}

    // Token komanda goşulany üçin, indi bu ülňiler gerek däl
	empty_token(tok);

	CUR_PART = prev_part;
	return 1;
}



