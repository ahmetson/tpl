#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "cmds.h"
#include "token/token_types.h"
#include "token/helpers.h"
#include "cmd/assign.h"
#include "cmd/def_var.h"
#include "cmd/fn_call.h"
#include "cmd/call_glob_var.h"
#include "cmd/arif.h"
#include "cmd/cmp.h"
#include "cmd/logic.h"
#include "cmd/ctrl_statement.h"
#include "cmd/block.h"
#include "cmd/loop_sttmnt.h"
#include "cmd/array.h"
#include "cmd/user_def_type.h"
#include "cmd/fn_def.h"
#include "cmd/conv_basic_types.h"
#include "cmd/block_inside.h"
#include "paren/types.h"
#include "algor.h"
#include "dev_debug.h"
#include "error.h"
#include "main/inf.h"
#include "fns/fn_helpers.h"
#include "fns.h"
#include "main/user_def_type.h"

// Komandanyň birliginiň nomeri
const int UNKNOWN_ITEM  = 0;
const int TOKEN_ITEM    = 1;
const int CMD_ITEM      = 2;
const int PAREN_ITEM    = 3;

// Komandalaryn sanawy
int CMD_CLASS_UNKNOWN       = 0;
int CMD_CLASS_DEF_VAR       = 1;
int CMD_CLASS_ASSIGN        = 2;
int CMD_CLASS_FN_CALL       = 3;
int CMD_CLASS_CALL_GLOB_VAR = 4;
int CMD_CLASS_ARIF          = 5;
int CMD_CLASS_CMP           = 6;
int CMD_CLASS_LOGIC         = 7;
int CMD_CLASS_CTRL_STTMNT   = 8;
int CMD_CLASS_BLOCK         = 9;
int CMD_CLASS_LOOP_STTMNT   = 10;
int CMD_CLASS_ARR           = 11;
int CMD_CLASS_UTYPE         = 12;
int CMD_CLASS_FN_DEF        = 13;
int CMD_CLASS_CONV_BASIC_TYPES = 14;
int CMD_CLASS_BLOCK_INSIDE = 15;

int GLOB = 0;
int LOCAL = 1;

is_cmd_item cmd_types[] = {
	   {is_cmd_def_var},
	   {is_cmd_assign},
	   {is_cmd_fn_call},
	   {is_cmd_call_glob_var},
	   {is_cmd_arif},
	   {is_cmd_cmp},
	   {is_cmd_logic},
	   {is_cmd_ctrl_sttmnt},
	   {is_cmd_block},
	   {is_cmd_loop_sttmnt},
	   {is_cmd_arr},
	   {is_cmd_utype},
	   {is_cmd_fn_def},
	   {is_cmd_conv_basic_type},
	   {is_cmd_block_inside}
};

// Dine debug üçin ulanylyar. Komanda tiplerinin atlary
char *cmd_classes[] = {
	"var",
	"assign",
	"function",
	"call_glob_var",
	"arifmethic",
	"deňeşdirme",
	"logiki",
	"Dolandyryş operatory",
	"Blok",
	"Gaýtalama operatory",
	"Birsyhly sanaw",
	"UG bilen baglanyşykly",
	"Ulanyjynyň funksiýasy",
	"Sada tipini üýtgetme",
	"Bloklar içinde ulanyp bolýan komandalar"
};

// diňe debuglamak üçin
char *cmd_class_types[][MAX_CLASS_TYPES] = {
	{"def"},			// Var class
	{"left", "right"},
	{"call", 0},
	{""},
	{"low_prior", "high_prior"},
	{""},
	{"Iki maglumatly", "Bir maglumatly"},
	{"eger", "ýa", "ýogsa"},
	{"ýapýan"},
	{"tä", "bolýança"},
	{"yglanlama", "çatylma"},
	{"yglanlama", "çatylma"},
	{"yglanlama", "beýanlama"},
	{"sada tipi üýtgetme"},
	{"funksiýadan çykýan komanda"}
};


int (*CMD_RETURN_TYPE[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd, int *cmd_class, int *cmd_type) = {
    {empty_cmd_return_type,         empty_cmd_return_type},
    {empty_cmd_return_type,         empty_cmd_return_type},//CMD_CLASS_DEF_VAR = 1;
    {empty_cmd_return_type,         empty_cmd_return_type},//CMD_CLASS_ASSIGN = 2;
    {cmd_fn_call_return_type,       empty_cmd_return_type},//CMD_CLASS_FN_CALL = 3;
    {cmd_call_glob_var_return_type, empty_cmd_return_type},//CMD_CLASS_CALL_GLOB_VAR_RETURN_TYOE = 4;
    {cmd_arif_return_type,          cmd_arif_return_type}, //CMD_CLASS_ARIF = 5;
    {empty_cmd_return_type,         empty_cmd_return_type},//CMD_CLASS_CMP = 6;
    {empty_cmd_return_type,         empty_cmd_return_type},//CMD_CLASS_LOGIC = 7;
    {empty_cmd_return_type,         empty_cmd_return_type},//CMD_CLASS_CTRL_STTMNT = 8;
    {empty_cmd_return_type},                               //CMD_CLASS_LOGIC = 9;
    {empty_cmd_return_type,         empty_cmd_return_type},//CMD_CLASS_LOGIC_STTMNT = 10;
    {empty_cmd_return_type,         cmd_arr_con_return_type},//CMD_CLASS_ARR = 11;
    {empty_cmd_return_type,         cmd_utype_con_return_type},//CMD_CLASS_UTYPE = 12;
    {empty_cmd_return_type,         empty_cmd_return_type},//CMD_CLASS_FN_DEF = 13;
    {cmd_arr_conv_basic_return_type},    //CMD_CLASS_CONV_BASIC_TYPE = 14;
    {cmd_block_inside_return_type}    //CMD_CLASS_BLOCK_INSIDE = 15;
};


// Komandanyň klasy we tipi boýunça semantikasyny barlaýan funksiýalar
int (*CMD_CHECK_SEMANTICS[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd) = {
    {empty_cmd_checking_semantic, empty_cmd_checking_semantic},
    {semantic_cmd_def_var,        semantic_cmd_def_var},       //CMD_CLASS_DEF_VAR = 1;
    {semantic_cmd_assign,         empty_cmd_checking_semantic},//CMD_CLASS_ASSIGN = 2;
    {semantic_cmd_fn_call,        empty_cmd_checking_semantic},//CMD_CLASS_FN_CALL = 3;
    {semantic_cmd_call_glob_var,  empty_cmd_checking_semantic},//CMD_CLASS_CALL_GLOB_VAR = 4;
    {semantic_cmd_arif,           semantic_cmd_arif},          //CMD_CLASS_ARIF = 5;
    {semantic_cmd_cmp,            semantic_cmd_cmp},           //CMD_CLASS_CMP = 6;
    {empty_cmd_checking_semantic, empty_cmd_checking_semantic},//CMD_CLASS_LOGIC = 7;
    {empty_cmd_checking_semantic, semantic_cmd_ctrl_sttmnt, semantic_cmd_ctrl_sttmnt},   //CMD_CLASS_CTRL_STTMNT = 8;
    {semantic_cmd_block},                                      //CMD_CLASS_BLOCK = 9;
    {empty_cmd_checking_semantic, empty_cmd_checking_semantic},//CMD_CLASS_LOOP_STTMNT = 10;
    {semantic_cmd_arr_def,        semantic_cmd_arr_con}, //CMD_CLASS_ARR = 11;
    {semantic_cmd_utype_def,      semantic_cmd_utype_con},//CMD_CLASS_UTYPE = 12;
    {semantic_cmd_fn_def,         semantic_cmd_fn_dec}, //CMD_CLASS_FN_DEF = 13;
    {semantic_cmd_conv_basic_type },//CMD_CLASS_CONV_BASIC_TYPE = 14;
    {semantic_cmd_block_inside }//CMD_CLASS_CONV_BASIC_TYPE = 14;
};

// Komandanyň klasy we tipi boýunça semantikasyny barlaýan funksiýalar
int CMD_MAX_ITEMS[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES] = {
    {MAX_NO_ITEMS,    MAX_NO_ITEMS},
    {MAX_THREE_ITEMS, MAX_THREE_ITEMS},               //CMD_CLASS_DEF_VAR = 1;
    {MAX_THREE_ITEMS, MAX_THREE_ITEMS},               //CMD_CLASS_ASSIGN = 2;
    {MAX_TWO_ITEMS,   MAX_TWO_ITEMS},                 //CMD_CLASS_FN_CALL = 3;
    {MAX_TWO_ITEMS,   MAX_TWO_ITEMS},                 //CMD_CLASS_CALL_GLOB_VAR = 4;
    {MAX_THREE_ITEMS, MAX_THREE_ITEMS},               //CMD_CLASS_ARIF = 5;
    {MAX_THREE_ITEMS, MAX_THREE_ITEMS},               //CMD_CLASS_CMP  = 6;
    {MAX_THREE_ITEMS, MAX_TWO_ITEMS},                 //CMD_CLASS_LOGIC  = 7;
    {MAX_THREE_ITEMS, MAX_THREE_ITEMS, MAX_ONE_ITEMS},//CMD_CLASS_CTRL_STTMNT  = 8;
    {MAX_ONE_ITEMS},                                  //CMD_CLASS_BLOCK  = 9;
    {MAX_THREE_ITEMS},                                //CMD_CLASS_LOOP_STTMNT  = 10;
    {},                                               //CMD_CLASS_ARR  = 11;
    {MAX_THREE_ITEMS, MAX_THREE_ITEMS},               //CMD_CLASS_UTYPE  = 12;
    {MAX_FOUR_ITEMS,  MAX_THREE_ITEMS},               //CMD_CLASS_FN_DEF  = 13;
    {MAX_TWO_ITEMS},                                  //CMD_CLASS_CONV_BASIC_TYPES  = 14;
    {MAX_TWO_ITEMS}                                   //CMD_CLASS_:BLOCK_INSIDE  = 15;
};


// Komandalaryň klasy we tipi boýunça, komandanyň tekstini ýazýan funksiýa çagyrylýar.
void (*CMD_GET_C_CODE[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd, wchar_t **l, int *len) = {
    {empty_cmd_c_code,          empty_cmd_c_code},
    {cmd_def_var_as_subcmd_c_code, empty_cmd_c_code},   //CMD_CLASS_DEF_VAR = 1;
    {cmd_assign_c_code,         cmd_assign_c_code},     //CMD_CLASS_ASSIGN = 2;
    {cmd_fn_call_c_code,        empty_cmd_c_code},      //CMD_CLASS_FNS = 3;
    {cmd_call_glob_var_c_code,  empty_cmd_c_code},      //CMD_CLASS_CALL_GLOB_VAR = 4;
    {cmd_arif_c_code,           cmd_arif_c_code},       //CMD_CLASS_ARIF = 5;
    {cmd_cmp_c_code,            cmd_cmp_c_code},        //CMD_CLASS_CMP = 6;
    {cmd_logic_c_code,          cmd_logic_c_code},      //CMD_CLASS_LOGIC = 7;
    {cmd_ctrl_sttmnt_c_code,    cmd_ctrl_sttmnt_c_code, cmd_ctrl_sttmnt_c_code},//CMD_CLASS_CTRL_STTMNT = 8;
    {cmd_block_c_code},                                 //CMD_CLASS_BLOCK = 9;
    {cmd_loop_sttmnt_c_code,    cmd_loop_sttmnt_c_code},//CMD_CLASS_LOOP_STTMNT = 10;
    {cmd_arr_def_c_code,        cmd_arr_con_c_code},    //CMD_CLASS_ARR = 11;
    {empty_cmd_c_code,          cmd_utype_con_c_code},  //CMD_CLASS_UTYPE = 12;
    {cmd_fn_def_c_code,         cmd_fn_dec_c_code}, //CMD_CLASS_FN_DEF = 13;
    {cmd_conv_basic_type_c_code},  //CMD_CLASS_CONV_BASIC_TYPE = 14;
    {cmd_block_inside_fn_return_c_code}  //CMD_CLASS_BLOCK_INSIDE = 15;
};


void init_cmd(command *cmd, wchar_t free_items)
{
	if (free_items && cmd->items_num)
	{
	    free(GLOB_SUBCMD_ITEMS_LIST[cmd->items]);
	    GLOB_SUBCMD_ITEMS_LIST[cmd->items] = NULL;
	}
	cmd->items_num =
	cmd->is_compl =
	cmd->cmd_type =
	cmd->cmd_class =
	cmd->value_class =
	cmd->value_type =
	cmd->parenthesis = 0;
	cmd->ns = -1;

}

/** Tokeni saygarylyar */
int parse_cmd(command *cmd)
{

	int i;
	for (i=0; i<CMDS_TYPES_NUM; i++)
	{
		if (cmd_types[i].is_cmd(cmd))
            return 1;
	}
	return 0;
}


/** Tokeni komanda salyar
 *
 * 0 - yalnyshlyk bar
 * 1 - hemme zat showly boldy
**/
int add_to_cmd(command *cmd, int type, token tok, parenthesis paren, command subcmd)
{
	command_item cmd_item = {};
	cmd_item.type = type;
	if (type==TOKEN_ITEM)
        cmd_item.tok = tok;
    else if(type==CMD_ITEM)
        cmd_item.cmd = subcmd;
    else if (type==PAREN_ITEM)
        cmd_item.paren = paren;

    cmd->items_num++;
	if (change_cmd_items_num(cmd->items, cmd->items_num))
    {
        put_cmd_item(cmd->items, cmd->items_num-1, cmd_item);
        /// Komanda - blok bolsa üýtgetmeli maglumatlary ýerine ýetirýän funksiýa çagyrmaly
        work_with_blocks(cmd);
        return 1;
    }
    else
    {
        return 0;
    }
}

// Komandalar bilen ishleyan bolum
int work_with_cmd( command *cmd )
{
	// Hazir TPL-in fayly komandalar bilen ishleyan boluminde
	int prev_part = CUR_PART;
	CUR_PART = 4;

    if ( 0==cmd->items_num )
        return 0;

    // TODO
    // KOMANDA gutarylmadyk bolsa, yalnyshlyk haty komanda gutarylmady diyip gaytaryar
	// Komanda saygarylyp showly gutardy

	if ( 0==recognize_cmd( cmd ) )
	{
        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token( cmd ) );
	}
	//debug_cmd( cmd );
	// Eger komanda funksiyanyn ichinde bolsa,
    //      bolup biljek komandalara barlanyar.
	if ( is_inside_fn() &&
        is_unvalid_inside_fn_cmd( cmd ) )
    {
        print_err(CODE4_UNSUPPORT_CMD_INSIDE_FN, (token *)inf_get_last_token( cmd ) );
    }
    if ( is_fn_def( cmd ) )
    {
        check_semantics( cmd );
        prepare_tmp_fn_data_container( cmd );
    }
    else if ( is_fn_close_cmd( cmd ) )
    {
        move_tmp_fn_data_to_local();
        CUR_PART = prev_part;
        return 1;
    }
    if ( is_inside_fn() )
    {
        if ( !( cmd->cmd_class==CMD_CLASS_FN_DEF &&
                cmd->cmd_type ==CMD_FN_DEF_TYPE )&& !add_to_tmp_fn_def_var_list( cmd ) && !add_to_tmp_fn_def_arr_list( cmd ) )
        {
            add_to_tmp_fn_algor( cmd );
        }
    }
    else
    {
        if ( cmd->cmd_class==CMD_CLASS_UTYPE && cmd->cmd_type==CMD_UTYPE_DEF_TYPE ) {
            add_new_utype( cmd );
            move_tmp_utype_items_to_last();
        }
        else if (!add_to_def_var_list( cmd ) && !add_to_def_arr_list( cmd ) ) // Komanda ulnini yglan etme dal eken
        {
            global_called_vars_add( cmd );
            global_called_arrs_add( cmd );
            // Komandany algoritme goshulyar

            algor_add_cmd( *cmd );
        }
    }
    // Eger komanda funksiyany bashlayan bolsa.
    // onda indiki parsinglenjek maglumatlary wagtlayn yada salmagyn parametrleri goyulyar.
    // Ya komanda funksiyany yapyan bolsa
    //      indiki parsinglenjek maglumatlary wagtlayyn yatdan uly yaya salmazlygynyn parametrleri goyulyar.


    // Eger funksiyanyn ichinde bolsa
        // Eger komanda ulny ya sanaw yglan edilmegi bolsa, wagtlayyn funksiyanyn maglumatlarynyn ichine gechirilyar.
        // Yogsa
        //     wagtlayyn algoritmlerin ichine gechirilyar.
    // Yogsa

	CUR_PART = prev_part;
	return 1;
}


int empty_cmd_return_type(command *cmd, int *cmd_class, int *cmd_type)
{
    *cmd_class = TOK_CLASS_UNDEFINED;
    return 0;
}

int empty_cmd_checking_semantic(command *cmd)
{
    return 0;
}

void empty_cmd_c_code(command *cmd, wchar_t **l, int *len)
{
    return;
}

command get_empty_cmd()
{
    command c;
    init_cmd(&c, 0);
    return c;
}


int is_cmd_item_compl(command_item *ci)
{
    if (ci->type==TOKEN_ITEM)
        return ci->tok.is_compl;
    else if(ci->type==CMD_ITEM)
        return ci->cmd.is_compl;
    else if(ci->type==PAREN_ITEM)
        return is_paren_compl(&ci->paren);

    return 0;
}

int is_paren_compl(parenthesis *p)
{
    if (!p->elems_num)
        return 1;
    parenthesis_elem *p_es = get_paren_elems(p->elems);

    if (p_es[0].type==TOKEN_ITEM)
        return p_es[0].tok.is_compl;
    else if(p_es[0].type==CMD_ITEM)
        return p_es[0].cmd.is_compl;
    else if(p_es[0].type==PAREN_ITEM)
        return is_paren_compl(&p_es[0].paren);
    return 0;
}


int subcmd_items_add(unsigned int items_num)
{
    /// Komandanyň pod sanawynda hökman bir birlik bolaýmaly
    if (!items_num)
    {
        CUR_PART = 4;
        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(&cmd));
    }
    ++GLOB_SUBCMDS_NUM;

    command_item**tmp = realloc(GLOB_SUBCMD_ITEMS_LIST, sizeof(*GLOB_SUBCMD_ITEMS_LIST)*GLOB_SUBCMDS_NUM);
    if (tmp!=NULL)
        GLOB_SUBCMD_ITEMS_LIST = tmp;
    if (GLOB_SUBCMD_ITEMS_LIST==NULL)
    {
        CUR_PART = 4;
        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(&cmd));
    }

    long size = sizeof(**GLOB_SUBCMD_ITEMS_LIST)*items_num;
    GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1] = NULL;
    GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1] = realloc(GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1], size);

    return GLOB_SUBCMDS_NUM-1;
}


command_item **main_cmd_add(unsigned int items_num)
{
    /// Komandanyň pod sanawynda hökman bir birlik bolaýmaly
    if (!items_num)
    {
        printf("SALAM 41: %s\n", __FILE__);
        CUR_PART = 4;

    }
    command_item *tmp = realloc(MAIN_CMD_ITEMS_LIST, sizeof(*MAIN_CMD_ITEMS_LIST)*items_num);
    if (tmp!=NULL)
        MAIN_CMD_ITEMS_LIST = tmp;
    if (tmp==NULL)
    {
        printf("SALAM YALNYDHLYK:  %s\n", __FILE__);
    }

    return &MAIN_CMD_ITEMS_LIST;
}



int is_glob_decl_support_cmd(command *cmd)
{
    if ((cmd->cmd_class==CMD_CLASS_DEF_VAR && cmd->ns==LOCAL) ||
        (cmd->cmd_class==CMD_CLASS_ARR && cmd->cmd_type==CMD_CLASS_ARR_DEF && cmd->ns==LOCAL) ||
        (cmd->cmd_class==CMD_CLASS_FN_DEF && cmd->cmd_type==CMD_FN_DEC_TYPE))
        return 1;
    return 0;
}


void glob_vars_decl_add(command *cmd)
{
    if (cmd->cmd_class!=CMD_CLASS_DEF_VAR)
        return;
    if (!cmd->is_compl)
    {
        CUR_PART = 0;
        print_err(CODE0_UNSUPPORT_INCLUDE_FILE_CMD, (token *)inf_get_last_token(cmd));
    }
    ++GLOB_VAR_DECS_NUMS;
    GLOB_VAR_DECS = realloc(GLOB_VAR_DECS, sizeof(*GLOB_VAR_DECS)*GLOB_VAR_DECS_NUMS);

    command_item *fci = get_cmd_item(cmd->items, 0);
    command_item *sci = get_cmd_item(cmd->items, 1);

    token *t1 = &fci->tok;
    token *t2 = &sci->tok;

    glob_ident gi;
    gi.type_class = t1->potentional_types[0].type_class;
    gi.type_num   = t1->potentional_types[0].type_num;
    wcsncpy(gi.name, t2->potentional_types[0].value, wcslen(t2->potentional_types[0].value)+1);
    gi.inf_wchar_t = t1->inf_wchar_t;
    gi.inf_wchar_t_num = t1->inf_wchar_t_num;
    gi.inf_line_num = t1->inf_line_num;
    gi.inf_file_num = t1->inf_file_num;

    GLOB_VAR_DECS[GLOB_VAR_DECS_NUMS-1] = gi;
}

void glob_arrs_decl_add(command *cmd)
{
    if (!(cmd->cmd_class==CMD_CLASS_ARR && cmd->cmd_type==CMD_CLASS_ARR_DEF))
        return;
    if (!cmd->is_compl)
    {
        CUR_PART = 0;
        print_err(CODE0_UNSUPPORT_INCLUDE_FILE_CMD, (token *)inf_get_last_token(cmd));
    }
    ++GLOB_ARR_DECS_NUMS;
    GLOB_ARR_DECS = realloc(GLOB_ARR_DECS, sizeof(*GLOB_ARR_DECS)*GLOB_ARR_DECS_NUMS);

    command_item *type_ci = get_cmd_item(cmd->items, cmd->items_num-2);
    command_item *ident_ci = get_cmd_item(cmd->items, cmd->items_num-1);
    command_item *yay_ci = get_cmd_item(cmd->items, cmd->items_num-3);

    token *t1 = &type_ci->tok;
    token *t2 = &ident_ci->tok;

    array_item ai;
    ai.type_class = t1->potentional_types[0].type_class;
    ai.type_num   = t1->potentional_types[0].type_num;
    wcsncpy(ai.ident, t2->potentional_types[0].value, wcslen(t2->potentional_types[0].value)+1);
    ai.inf_wchar_t = t1->inf_wchar_t;
    ai.inf_wchar_t_num = t1->inf_wchar_t_num;
    ai.inf_line_num = t1->inf_line_num;
    ai.inf_file_num = t1->inf_file_num;
    ai.incs = yay_ci->paren.elems_num;

    GLOB_ARR_DECS[GLOB_ARR_DECS_NUMS-1] = ai;

    GLOB_ARR_DECS_ITEMS = realloc(GLOB_ARR_DECS_ITEMS, sizeof(*GLOB_ARR_DECS_ITEMS)*GLOB_ARR_DECS_NUMS);
    GLOB_ARR_DECS_ITEMS[GLOB_ARR_DECS_NUMS-1] = NULL;

    add_to_last_dec_arr_items(cmd);
}


void glob_fns_decl_add(command *cmd)
{
    if (!(cmd->cmd_class==CMD_CLASS_FN_DEF && cmd->cmd_type==CMD_FN_DEC_TYPE))
        return;
    if (!cmd->is_compl)
    {
        CUR_PART = 0;
        print_err(CODE0_UNSUPPORT_INCLUDE_FILE_CMD, (token *)inf_get_last_token(cmd));
    }
    add_fn_dec(cmd);

}



int is_glob_var_dec_exist(wchar_t *ident)
{
    int i, len = wcslen(ident);
    for(i=0; i<GLOB_VAR_DECS_NUMS; ++i)
    {
        if (wcslen(GLOB_VAR_DECS[i].name)==len && wcsncmp(GLOB_VAR_DECS[i].name, ident, len)==0)
            return 1;
    }
    return 0;
}


int is_glob_arr_dec_exist(wchar_t *ident)
{
    int i, len = wcslen(ident);
    for(i=0; i<GLOB_ARR_DECS_NUMS; ++i)
    {
        if (wcslen(GLOB_ARR_DECS[i].ident)==len && wcsncmp(GLOB_ARR_DECS[i].ident, ident, len)==0)
            return 1;
    }
    return 0;
}


void get_glob_var_dec_value_type(wchar_t *ident, int *c, int *t)
{
    int i, len = wcslen(ident);
    for(i=0; i<GLOB_VAR_DECS_NUMS; ++i)
    {
        if (wcslen(GLOB_VAR_DECS[i].name)==len && wcsncmp(GLOB_VAR_DECS[i].name, ident, len)==0)
        {
            *c = GLOB_VAR_DECS[i].type_class;
            *t = GLOB_VAR_DECS[i].type_num;
            return;
        }
    }
}


void  work_with_glob_var_decs()
{
    int j;
    for (j=0; j<GLOB_VAR_DECS_NUMS; ++j)
    {
        if (!is_glob_var_def_exist(GLOB_VAR_DECS[j].name))
        {
            CUR_PART = 7;
            inf_tok.inf_wchar_t     = GLOB_VAR_DECS[j].inf_wchar_t;
            inf_tok.inf_wchar_t_num = GLOB_VAR_DECS[j].inf_wchar_t_num;
            inf_tok.inf_line_num = GLOB_VAR_DECS[j].inf_line_num;
            inf_tok.inf_file_num = GLOB_VAR_DECS[j].inf_file_num;
            print_err(CODE7_GLOB_VAR_MUST_DEF, &inf_tok);
        }
        /// Yglan edilen we maglumaty yglan edilen global ülňileriň tipleri gabat gelmeli
        glob_ident *gi = glob_vars_def_get_by_name(GLOB_VAR_DECS[j].name);
        if (!(gi->type_class==GLOB_VAR_DECS[j].type_class && gi->type_num==GLOB_VAR_DECS[j].type_num))
        {
            CUR_PART = 7;
            inf_tok.inf_wchar_t     = GLOB_VAR_DECS[j].inf_wchar_t;
            inf_tok.inf_wchar_t_num = GLOB_VAR_DECS[j].inf_wchar_t_num;
            inf_tok.inf_line_num = GLOB_VAR_DECS[j].inf_line_num;
            inf_tok.inf_file_num = GLOB_VAR_DECS[j].inf_file_num;
            print_err(CODE7_GLOB_VAR_NOT_MATCH_DATA_TYPE, &inf_tok);
        }
    }
}

void  work_with_glob_arr_decs()
{
    int j;
    for (j=0; j<GLOB_ARR_DECS_NUMS; ++j)
    {
        if (!is_glob_arr_def_exist(GLOB_ARR_DECS[j].ident))
        {
            CUR_PART = 7;
            inf_tok.inf_wchar_t     = GLOB_ARR_DECS[j].inf_wchar_t;
            inf_tok.inf_wchar_t_num = GLOB_ARR_DECS[j].inf_wchar_t_num;
            inf_tok.inf_line_num = GLOB_ARR_DECS[j].inf_line_num;
            inf_tok.inf_file_num = GLOB_ARR_DECS[j].inf_file_num;
            print_err(CODE7_GLOB_ARR_MUST_DEF, &inf_tok);
        }
        /// Yglan edilen we maglumaty yglan edilen global ülňileriň tipleri gabat gelmeli
        array_item *gi = glob_arrs_def_get_by_name(GLOB_VAR_DECS[j].name);
        if (!(gi->type_class==GLOB_ARR_DECS[j].type_class && gi->type_num==GLOB_ARR_DECS[j].type_num))
        {
            CUR_PART = 7;
            inf_tok.inf_wchar_t     = GLOB_VAR_DECS[j].inf_wchar_t;
            inf_tok.inf_wchar_t_num = GLOB_VAR_DECS[j].inf_wchar_t_num;
            inf_tok.inf_line_num = GLOB_VAR_DECS[j].inf_line_num;
            inf_tok.inf_file_num = GLOB_VAR_DECS[j].inf_file_num;
            print_err(CODE7_GLOB_ARR_NOT_MATCH_DATA_TYPE, &inf_tok);
        }
    }
}


void work_with_glob_fn_decs()
{
    int j;
    for (j=0; j<DEC_FUNCS_NUM; ++j)
    {
        if (!is_fn_name_used(DEC_FUNCS[j].name))
        {
            CUR_PART = 7;
            inf_tok.inf_wchar_t     = DEC_FUNCS[j].inf_wchar_t;
            inf_tok.inf_wchar_t_num = DEC_FUNCS[j].inf_wchar_t_pos;
            inf_tok.inf_line_num = DEC_FUNCS[j].inf_line_pos;
            inf_tok.inf_file_num = DEC_FUNCS[j].file_num;
            print_err(CODE7_FN_NOT_DEF, &inf_tok);
        }
        /// Yglan edilen we maglumaty yglan edilen funksiýalaryň kabul edýän argumentleri deň bolmaly
        //array_item *gi = glob_arrs_def_get_by_name(GLOB_VAR_DECS[j].name);
        /// Yglan edilen şol bir identifikatorly funksiýanyň nomeri
        int fn = get_fn_by_ident(DEC_FUNCS[j].name);
        if (FUNCS[fn].args_num != DEC_FUNCS[j].args_num)
        {
            CUR_PART = 7;
            inf_tok.inf_wchar_t     = DEC_FUNCS[j].inf_wchar_t;
            inf_tok.inf_wchar_t_num = DEC_FUNCS[j].inf_wchar_t_pos;
            inf_tok.inf_line_num = DEC_FUNCS[j].inf_line_pos;
            inf_tok.inf_file_num = DEC_FUNCS[j].file_num;
            /// Beýan edilýän funksiýa, yglan edilýän funksiýa ýaly deň möçberde argument kabul edenok
            print_err(CODE7_FN_ARG_NUMES_NOT_MATCH, &inf_tok);
        }
        else if (FUNCS[fn].args_num)
        {
            int i;
            for (i=0; i<FUNCS[fn].args_num; ++i)
            {
                if (FUNC_ARGS[fn][i].type_class!=DEC_FUNC_ARGS[j][i].type_class &&
                    FUNC_ARGS[fn][i].type_num  !=DEC_FUNC_ARGS[j][i].type_num)
                {
                    CUR_PART = 7;
                    inf_tok.inf_wchar_t     = DEC_FUNCS[j].inf_wchar_t;
                    inf_tok.inf_wchar_t_num = DEC_FUNCS[j].inf_wchar_t_pos;
                    inf_tok.inf_line_num = DEC_FUNCS[j].inf_line_pos;
                    inf_tok.inf_file_num = DEC_FUNCS[j].file_num;
                    /// Beýan edilýän we yglan edilýän funksiýadaky argumentleriň tipleri gabat gelmedi
                    print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, &inf_tok);
                }
            }
        }
        /// Funksiýanyň gaýtarýan tipi deňeşdirilýär:
        if (FUNCS[fn].return_class!=DEC_FUNCS[j].return_class &&
            FUNCS[fn].return_type!=DEC_FUNCS[j].return_type)
        {
            CUR_PART = 7;
            inf_tok.inf_wchar_t     = DEC_FUNCS[j].inf_wchar_t;
            inf_tok.inf_wchar_t_num = DEC_FUNCS[j].inf_wchar_t_pos;
            inf_tok.inf_line_num = DEC_FUNCS[j].inf_line_pos;
            inf_tok.inf_file_num = DEC_FUNCS[j].file_num;
            /// Beýan edilýän we yglan edilýän funksiýanyň gaýtarmaly maglumatynyň tipleri gabat gelmedi
            print_err(CODE7_FN_RET_TYPE_NOT_MATCH, &inf_tok);
        }
    }
}


void cmd_wrapper_c_code(wchar_t **l, int *llen)
{
    wchar_t *cmd_end = L"; \n";
    wcsadd_on_heap( l, llen, cmd_end );
}

void cmd_block_wrapper_c_code(wchar_t **l, int *llen)
{
    wchar_t *block_end = L" \n";
    wcsadd_on_heap( l, llen, block_end );
}


command_item *get_cmd_item(int cmd_num, int item_num)
{
    if (cmd_num==-1)
        return &MAIN_CMD_ITEMS_LIST[item_num];
    else if (cmd_num==-2)
        return &TMP_CMD_ITEMS_LIST[item_num];
    return &GLOB_SUBCMD_ITEMS_LIST[cmd_num][item_num];
}


int change_cmd_items_num(int cmd_num, int new_num)
{
    if (cmd_num==-1)
    {
        long size = new_num * sizeof(*MAIN_CMD_ITEMS_LIST);
        command_item *items = realloc(MAIN_CMD_ITEMS_LIST, size);

        if (items==NULL)
            return 0;
        MAIN_CMD_ITEMS_LIST = items;
    }
    else if (cmd_num==-2)
    {
        long size = new_num * sizeof(*TMP_CMD_ITEMS_LIST);
        command_item *items = realloc(TMP_CMD_ITEMS_LIST, size);
        if (items==NULL)
            return 0;
        TMP_CMD_ITEMS_LIST = items;
    }
    else
    {
        long size = new_num*sizeof(**GLOB_SUBCMD_ITEMS_LIST);
        command_item *items = realloc(GLOB_SUBCMD_ITEMS_LIST[cmd_num], size);
        if (items==NULL)
            return 0;
        GLOB_SUBCMD_ITEMS_LIST[cmd_num] = items;
    }
    return 1;
}


void put_cmd_item(int cmd_num, int item_num, command_item ci)
{
    if (cmd_num==-1)
        MAIN_CMD_ITEMS_LIST[item_num] = ci;
    else if (cmd_num==-2)
        TMP_CMD_ITEMS_LIST[item_num] = ci;
    else
        GLOB_SUBCMD_ITEMS_LIST[cmd_num][item_num] = ci;
}


int is_cmd_not_compl_item_exist(command *cmd, wchar_t rec)
{
    int i;
    for (i=0; i<cmd->items_num; ++i)
    {
        command_item *ci = get_cmd_item(cmd->items, i);
        if (ci->type==CMD_ITEM)
        {
            if (!ci->cmd.is_compl)
            {
                return 1;
            }
        }
        else if (ci->type!=TOKEN_ITEM && rec)
        {
            if (ci->type==CMD_ITEM)
            {
                return is_cmd_not_compl_item_exist(&ci->cmd, 1);
            }
            else if (ci->type==PAREN_ITEM)
            {

            }
        }
    }

    return 1;
}

int is_cmd_item_can_be_needed(command *cmd)
{
    int i, j;
    for (i=0; i<cmd->items_num; ++i)
    {
        command_item *ci = get_cmd_item(cmd->items, i);
        if (ci->type==CMD_ITEM && !ci->cmd.is_compl)
        {
            for (j=0; j<CMDS_TYPES_NUM; ++j)
            {
                // Gutarylmadyk birlik bolup biljek birlik diýilýär we
                // komanda şol bolup biljek birlik bilen barlanýar
                if (cmd_types[j].is_cmd(&ci->cmd) && parse_cmd(cmd))
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}


void make_cmd_copy(command *cmd, command *cmd_out)
{
    init_cmd(cmd_out, 0);
    *cmd_out = *cmd;

    if (!cmd->items_num)
    {
        return;
    }

    cmd_out->items = subcmd_items_add(cmd->items_num);
    int i;
    for (i=0; i<cmd->items_num; ++i)
    {
        command_item *ci = get_cmd_item(cmd->items, i);
        put_cmd_item(cmd_out->items, i, *ci);
    }
}

/** Recognize complete command from series of tokens*/
int     recognize_cmd( command *cmd )
{
    /**
    1. get_op_positions() int *
    2. minimize_operands(cmd, ops) void
    3. get_op_positions() int *
    4. make_subcmd_from_op(cmd, ops)
    5. Eger komanda bir ýa nol operator bolanda tanalmasa
           Komanda tanalmady diýen ýalňyşlyk görkezilýär.*/

    int *op_positions = get_op_positions( cmd );   // First element includes number of operators.
    minimize_single_operand_cmds( cmd );

    free( op_positions );
    op_positions = get_op_positions( cmd );
    minimize_operands( cmd, op_positions );

    //int num_of_ops = op_positions[0];
    make_subcmd_from_op( cmd, &op_positions );

    //debug_cmd( cmd );

    free(op_positions);

    if ( !parse_cmd(cmd) )
    {
        printf("------------------------------\n\n\n");
        parse_cmd(cmd);
        debug_cmd( cmd );
        print_err(CODE4_CANT_IDENT_CMD, (token*)inf_get_last_token(cmd));
    }

    return 1;
}


/** Make nested commands from list of tokens
    Where command's operand must contain only one item.
    Currently only one command's operands can contain only one item:
    connecting to user defined type variable
*/
void minimize_single_operand_cmds( command *cmd )
{
    int *op_positions = get_op_positions( cmd ),
        i, start, end;
    for ( i=0; i<op_positions[ 0 ]; ++i  )
    {
        command_item *ci = get_cmd_item( cmd->items, op_positions[ i+1 ] );

        if ( ci->type==TOKEN_ITEM && ci->tok.type_class==TOK_CLASS_UTYPE &&
             get_token_type( &ci->tok )==UTYPE_ITEM_SEPARATOR )
        {
            get_left_side_operand( cmd, op_positions, op_positions[ i+1 ], &start, &end );
//            start   = op_positions[ i+1 ]-1;
//            end     = op_positions[ i+1 ]+1;

            command new_cmd;
            init_cmd( &new_cmd, 0 );
            new_cmd.items = get_mem_for_cmd_items();
            new_cmd.items_num = end - start + 1;
            change_cmd_items_num( new_cmd.items, new_cmd.items_num );

            // Move items to new data
            int j, cur_pos = start;
            for ( j=0; j<new_cmd.items_num; ++j, ++cur_pos )
            {
                command_item *ci = get_cmd_item( cmd->items, cur_pos );
                put_cmd_item( new_cmd.items, j, *ci );
            }

            if ( !parse_cmd( &new_cmd ) || !new_cmd.is_compl )
            {
                printf( "CANNOT IDENTIFY OPERAND FOR UTYPE VAR CALL: %d\n", __LINE__ );
            }

            // Set made sub cmd from data items to main Command. Move next operators to left
            command_item data_cmd;
            data_cmd.type = CMD_ITEM;
            data_cmd.cmd  = new_cmd;
            put_cmd_item( cmd->items, start, data_cmd );

            move_cmd_items ( start+1, end+1, cmd );

            free( op_positions );
            op_positions = get_op_positions( cmd );
            i = 0;
        }
    }

    free( op_positions );
}

/** Make nested commands from list of tokens.
*/
void    minimize_operands( command *cmd, int *op_positions )
{
    int i, start = 0, end = cmd->items_num-1;

    if ( op_positions[ 0 ]==0 )
        return;

    /// Minimize left sides of operators
    for ( i=1; i<=op_positions[ 0 ]; ++i )
    {
        // Find first token position of data items
        if ( i==1 )
            start = 0;
        else
        {
            if ( op_positions[ i-1 ]+1 == op_positions[ i ] )       // Between previous and current operator no data.
            {
                continue;
            }
            else
            {
                start = op_positions[ i-1 ]+1;
            }
        }

        if ( op_positions[ i ]==0 )
            continue;
        //command_item *ci = get_cmd_item( cmd->items, op_positions[ i ] );

        // Find last token position of data items
        end = op_positions[ i ]-1;
        // Minimize data to single item
        if ( end - start==0 )
            continue;
        else
        {
            command new_cmd;
            init_cmd( &new_cmd, 0 );
            new_cmd.items = get_mem_for_cmd_items();
            new_cmd.items_num = end - start + 1;
            change_cmd_items_num( new_cmd.items, new_cmd.items_num );

            // Move items to new data
            int j, cur_pos = start;
            for ( j=0; j<new_cmd.items_num; ++j, ++cur_pos )
            {
                command_item *ci = get_cmd_item( cmd->items, cur_pos );
                put_cmd_item( new_cmd.items, j, *ci );
            }

            if ( !parse_cmd( &new_cmd ) || !new_cmd.is_compl )
            {
                printf( "CANNOT IDENTIFY OPERAND: %d\n", __LINE__ );
            }

            // Set made sub cmd from data items to main Command. Move next operators to left
            command_item data_cmd;
            data_cmd.type = CMD_ITEM;
            data_cmd.cmd  = new_cmd;
            put_cmd_item( cmd->items, start, data_cmd );

            move_cmd_items ( start+1, end+1, cmd );
        }

    }

    int *minimized_op_positions = get_op_positions( cmd );

    /// Minimize last token data on the right side of operator
    if ( minimized_op_positions[ minimized_op_positions[ 0 ] ]+2 <= cmd->items_num-1 )
    {
        start   = minimized_op_positions[ minimized_op_positions[ 0 ] ]+1;
        end     = cmd->items_num-1;
        command new_cmd;
        init_cmd( &new_cmd, 0 );
        new_cmd.items = get_mem_for_cmd_items();
        new_cmd.items_num = end - start+1;
        change_cmd_items_num( new_cmd.items, end - start+1 );

        // Move items to new data
        int j, cur_pos = start;
        for ( j=0; j<end - start+1; ++j )
        {
            command_item *ci = get_cmd_item( cmd->items, cur_pos++ );
            put_cmd_item( new_cmd.items, j, *ci );
        }

        if ( !parse_cmd( &new_cmd ) )
        {
            free( minimized_op_positions );
            /// TODO
            printf( "CANNOT IDENTIFY OPERAND: %d\n", __LINE__ );
        }

        // Set made sub cmd from data items to main Command. Move next operators to left
        command_item data_cmd;
        data_cmd.type = CMD_ITEM;
        data_cmd.cmd  = new_cmd;
        put_cmd_item( cmd->items, start, data_cmd );

        cmd->items_num -= end - start;
        //move_cmd_items ( start+1, end-1, cmd );
    }
    free( minimized_op_positions );
}


void    move_cmd_items ( int orig_pos, int start_pos, command *cmd )
{
    command tmpcmd;
    init_cmd( &tmpcmd, 0 );
    tmpcmd.items = -2;
    tmpcmd.items_num = cmd->items_num;
    change_cmd_items_num( -2, tmpcmd.items_num );

    int i;
    for ( i=0; i<tmpcmd.items_num; ++i )
    {
        command_item *ci = get_cmd_item( cmd->items, i );
        put_cmd_item( -2, i, *ci );
    }

    // Cut the items num for tokens
    cmd->items_num -= ( start_pos ) - ( orig_pos );
    change_cmd_items_num( cmd->items, cmd->items_num );

    for ( i=start_pos; i<tmpcmd.items_num; ++i, ++orig_pos, ++start_pos )
    {
        command_item *ci = get_cmd_item( tmpcmd.items, start_pos );
        put_cmd_item( cmd->items, orig_pos, *ci );
    }

    free(TMP_CMD_ITEMS_LIST);
    TMP_CMD_ITEMS_LIST = NULL;
}


/** Makes sub-cmd for operator*/
int     make_subcmd_from_op(command *cmd, int **op_position)
{
    //Tä bir operator galýança
    int i, most_prior_pos = -1, most_prior_lvl = -1, left_operand_pos, right_operand_pos,
            until_one = (*op_position)[ 0 ]-1;
    for ( i=0; i<until_one; ++i )
    {
        left_operand_pos = -1;
        right_operand_pos = -1;
        //Iň wajyp operator tapylýar (find_most_prior_op)
        find_most_prior_op( cmd, (*op_position), &most_prior_pos, &most_prior_lvl );

        //Eger operator çep tarapda birlik talap edýän bolsa
        if ( is_op_require_left_data( most_prior_lvl ) )
        {
            if ( most_prior_pos==0 )
            {
                print_err(0, &inf_tok);
            }
            else if ( most_prior_pos!=(*op_position)[1] && most_prior_pos==get_prev_op_pos((*op_position), most_prior_pos )+1 )
            {
                // eger öňki operatoryň numeri häzirki operator - 1 bolsa
                print_err(0, &inf_tok);
            }
            else
            {
                //çep operand numeri alynyp çep operand üçin goýulýar.
                left_operand_pos = most_prior_pos-1;
            }
        }
        if ( is_op_require_right_data( most_prior_lvl ) )
        {
            if ( most_prior_pos==cmd->items_num-1 )
            {
                print_err(0, &inf_tok);
            }
            else if ( most_prior_pos<(*op_position)[ 0 ]-2 && most_prior_pos==get_next_op_pos((*op_position), most_prior_pos )-1 )
            {
                // eger operator soňky birlik bolsa ýa operator soňky operatoryň numeri häzirki operator + 1 bolsa
                print_err(0, &inf_tok);
            }
            else
            {
                //sag_operand numeri alynyp sag operand üçin diýilýär.
                right_operand_pos = most_prior_pos+1;
            }
        }

        // Täze komanda ýasalýar.
        command newcmd;
        init_cmd( &newcmd, 0 );
        newcmd.items = get_mem_for_cmd_items();

        // Eger çep we sag operand talap edýän bolsa
        if ( left_operand_pos!=-1 && right_operand_pos!=-1 )
        {
            newcmd.items_num = 3;
        }
        // Ýa çep we sag operand talap edilmeýän bolsa
        else if ( left_operand_pos==-1 && right_operand_pos==-1 )
        {
            newcmd.items_num = 1;
        }
        else
        {
            newcmd.items_num = 2;
        }
        change_cmd_items_num( newcmd.items, newcmd.items_num );

        // Eger çep birlik bar bolsa
        int item_pos = 0;
        if ( left_operand_pos!=-1 )
        {
            command_item *ci = get_cmd_item( cmd->items, left_operand_pos );
           //birinji birlik çep operand salynýar
           put_cmd_item( newcmd.items, item_pos++, *ci );
        }
        //operand salynýar.
        command_item *op_ci = get_cmd_item( cmd->items, most_prior_pos );
        put_cmd_item( newcmd.items, item_pos++, *op_ci );
        //eger sag birlik bar bolsa
        if ( right_operand_pos!=-1 )
        {
            command_item *rci = get_cmd_item( cmd->items, right_operand_pos );
           // üçünji birlik sag operand salynýar.
           put_cmd_item( newcmd.items, item_pos++, *rci );
        }

        //Eger komanda tanalmasa
        if ( !parse_cmd( &newcmd) )
        {
            //Komandany tanap bolmady diýen ýalňyşlyk görkezilýär.
            print_err( 0, &inf_tok );
        }
        else
        {
            //çepki birlik bolsa şonuň deregine ýogsa operator deregine goýulýar.
            command_item op_as_subcmd;
            op_as_subcmd.type = CMD_ITEM;
            op_as_subcmd.cmd = newcmd;

            int put_place = left_operand_pos==-1 ? most_prior_pos : left_operand_pos;
            put_cmd_item( cmd->items, put_place, op_as_subcmd );

            int last_moved = right_operand_pos==-1 ? most_prior_pos : right_operand_pos;

            //häzirki komanda gysgalýar.
            if ( put_place+1!=last_moved+1 )
            {
                // If the last operand is moved deleted to, short cut the command
                if ( cmd->items_num-1<last_moved+1 )
                    cmd->items_num -= ( last_moved ) - ( put_place );
                else
                    move_cmd_items( put_place+1, last_moved+1, cmd );
                free ((*op_position));
                (*op_position) = get_op_positions( cmd );
            }
        }
    }

    return 1;
}

int     get_prev_op_pos( int *pos, int cur_pos )
{
    int i, prev = -1;
    for ( i=1; i<=pos[ 0 ]; ++i )
    {
        if ( pos[ i ]==cur_pos )
            return prev;
        pos[ i ] = prev;
    }
    return prev;
}

int     get_next_op_pos( int *pos, int cur_pos )
{
    int i, ret;//, next = -1;
    for ( i=1; i<pos[ 0 ]; ++i )
    {
        if ( pos[ i ]==cur_pos )
        {
            ret = pos[ i+1 ];
            break;
        }
    }

    return ret;
}

void     find_most_prior_op(command *cmd, int *op_positions, int *most_prior_pos, int *most_prior_lvl )
{
    int i;
    *most_prior_lvl = *most_prior_pos = -1;
    for (i=1; i<=op_positions[ 0 ]; ++i)
    {
        if ( get_op_prior( cmd->items, op_positions[i] ) > *most_prior_lvl )
        {
            *most_prior_pos = op_positions [ i ];
            *most_prior_lvl  = get_op_prior(cmd->items, op_positions[i]);
        }
    }
}

int    *get_op_positions(command *cmd)
{
    int *op_positions = malloc(sizeof(int));
    op_positions[0] = 0;

    int i;
    for ( i=0; i<cmd->items_num; ++i )
    {
        command_item *ci = get_cmd_item( cmd->items, i );
        if ( is_cmd_item_op( ci ) && !( ci->type==TOKEN_ITEM && get_token_type_class( &ci->tok )==TOK_CLASS_ASSIGN
                                        && get_token_type( &ci->tok )==RIGHT_ASSIGN_TOK_NUM &&
                                        i==cmd->items_num-1 ) ) /// -> as last operand in command means block opener.
        {
            op_positions[0] += 1;
            op_positions = realloc( op_positions, ( op_positions[0]+1 )*sizeof(int) );     // Additional one for number of  operators
            op_positions[ op_positions[0] ] = i;
        }
    }
    return op_positions;
}

int     get_op_prior(int cmd_items, int op_position)
{
    command_item *ci = get_cmd_item( cmd_items, op_position );
    int item_type=ci->type,
        op_class,
        op_type;

    if ( item_type==PAREN_ITEM && ci->paren.type==PAREN_TYPE_DEF_TYPE )
        return 8;       // Most prior high values.
    op_class = get_token_type_class(&ci->tok);
    op_type  = get_token_type(&ci->tok);
    if ( op_class==TOK_CLASS_UTYPE && op_type==UTYPE_ITEM_SEPARATOR )
        return 9;
    if ( op_class==TOK_CLASS_ARIF && (op_type==TOK_CLASS_ARIF_MULTI_TYPE || op_type==TOK_CLASS_ARIF_DIV_TYPE) )
        return 7;
    if ( op_class==TOK_CLASS_ARIF && (op_type==TOK_CLASS_ARIF_PLUS_TYPE || op_type==TOK_CLASS_ARIF_MINUS_TYPE) )
        return 6;
    if ( op_class==TOK_CLASS_CMP )
        return 5;
    if ( op_class==TOK_CLASS_LOGIC && op_type==TOK_CLASS_LOGIC_NOT_TYPE )
        return 4;
    if ( op_class==TOK_CLASS_LOGIC && op_type!=TOK_CLASS_LOGIC_NOT_TYPE )
        return 3;
    if ( op_class==TOK_CLASS_ASSIGN )
        return 2;           // Least prior operator
    if ( op_class==TOK_CLASS_BLOCK_INSIDE && op_type==TOK_CLASS_BLOCK_INSIDE_FN_RETURN )
        return 1;
    return 0;
}

/** 1.item delimiter:       /                   token_item
    2.Conv. type: (simple_type)                 paren_item
    3.Arithmetik high-lvl:  *, :                token_item
    4.Arithmetik low-lvl:   +, -                token_item
    5.Compare:               >, <, =, <=, =>    token_item
    6.Boolean single op:    !                   token_item
    7.Boolean binary op:    &, ?                token_item
    8.Assign:               <-, ->              token_item
    8.Return:               yza                 token_item

**/
int is_cmd_item_op( command_item *ci )
{
    int i = 0, tok_class = -1, tok_type;
    for (i=0; i<7; ++i)
    {
        if ( ci->type==CMD_ITEM )
            return 0;

        if ( ci->type==PAREN_ITEM )
        {
            if ( ci->paren.type==PAREN_TYPE_DEF_TYPE )
                return 2;       // Most prior high values.
            else
                return 0;
        }

        tok_class = get_token_type_class(&ci->tok);
        tok_type = get_token_type(&ci->tok);
        if ( tok_class==TOK_CLASS_UTYPE && tok_type==UTYPE_ITEM_SEPARATOR )
            return 1;
        if ( tok_class==TOK_CLASS_ARIF && (tok_type==TOK_CLASS_ARIF_MULTI_TYPE || tok_type==TOK_CLASS_ARIF_DIV_TYPE) )
            return 3;
        if ( tok_class==TOK_CLASS_ARIF && (tok_type==TOK_CLASS_ARIF_PLUS_TYPE || tok_type==TOK_CLASS_ARIF_MINUS_TYPE) )
            return 4;
        if ( tok_class==TOK_CLASS_CMP )
            return 5;
        if ( tok_class==TOK_CLASS_LOGIC && tok_type==TOK_CLASS_LOGIC_NOT_TYPE )
            return 6;
        if ( tok_class==TOK_CLASS_LOGIC && tok_type!=TOK_CLASS_LOGIC_NOT_TYPE )
            return 7;
        if ( tok_class==TOK_CLASS_ASSIGN )
            return 8;           // Least prior operator
        if ( tok_class==TOK_CLASS_BLOCK_INSIDE && tok_type==TOK_CLASS_BLOCK_INSIDE_FN_RETURN )
            return 9;
    }
    return 0;
}


int is_op_require_left_data(int op_code)
{
    // All operators require left data, except Basic type converts
    if ( op_code==8)
        return 0;
    return 1;
}

int is_op_require_right_data( int op_code )
{
    // All operators require right data, except SINGLE LOGIC OPERATOR
    if ( op_code==4 )
        return 0;
    return 1;
}

int get_mem_for_cmd_items()
{
    ++GLOB_SUBCMDS_NUM;

    GLOB_SUBCMD_ITEMS_LIST = realloc( GLOB_SUBCMD_ITEMS_LIST, sizeof( **GLOB_SUBCMD_ITEMS_LIST )*GLOB_SUBCMDS_NUM );
    if ( GLOB_SUBCMD_ITEMS_LIST==NULL )
    {
        CUR_PART = 4;
        print_err( CODE4_CANT_IDENT_CMD, &inf_tok );
    }

    GLOB_SUBCMD_ITEMS_LIST[ GLOB_SUBCMDS_NUM-1 ] = NULL;

    return GLOB_SUBCMDS_NUM-1;
}

/** Returns operands type of command item */
int return_cmd_item_type( command_item *ci, int *type_class, int *type_num)
{
    /// Make string from first index number
    if ( (ci->type==TOKEN_ITEM && return_tok_type( &ci->tok, type_class, type_num ) ) ||
         (ci->type==CMD_ITEM   && CMD_RETURN_TYPE[ ci->cmd.cmd_class][ci->cmd.cmd_type](&ci->cmd, type_class, type_num )) ||
         (ci->type==PAREN_ITEM && PAREN_RETURN_TYPE[ ci->paren.type ]( &ci->paren, type_class, type_num ) ) )
    {
        return 1;
    }
    return 0;
}


void get_left_side_operand( command *cmd, int *op_positions, int op, int *start, int *end )
{
    if ( op==0 )
        return;

    int pre_op = op-1;
move_to_prev_operand:
{
    command_item *pre_op_item = get_cmd_item( cmd->items, pre_op );
    //debug_token ( &pre_op_item->tok );
    if ( pre_op_item->type==TOKEN_ITEM && get_token_type_class( &pre_op_item->tok )==TOK_CLASS_CONST_DATA
         && get_token_type( &pre_op_item->tok )==STRING_CONST_DATA_TOK_NUM )
    {
        /// Only integers as indexes of arrays can be inserted as multiple operand
        wchar_t *index = ( wchar_t* )get_glob_str_by_index( pre_op_item->tok.potentional_types[0].string_value );
        if ( ( wcslen( index )==1 && index[ 0 ]==L'0' ) ||
              _wtoi( index )!=0 )
        {
            --pre_op;
            goto move_to_prev_operand;
        }
    }
}
    *start   = pre_op;
    *end     = op+1;

    if ( *start != op-1 )
    {
        *end = op;

        // Täze komanda ýasalýar.
        command newcmd;
        init_cmd( &newcmd, 0 );
        newcmd.items = get_mem_for_cmd_items();

        newcmd.items_num =  op - *start;

        change_cmd_items_num( newcmd.items, newcmd.items_num );

        // Eger çep birlik bar bolsa
        int item_pos = *start, i;
        for ( i=0; i<newcmd.items_num; ++i )
        {
            command_item *ci = get_cmd_item( cmd->items, item_pos++ );
            //birinji birlik çep operand salynýar
            put_cmd_item( newcmd.items, i, *ci );
        }

        //Eger komanda tanalmasa
        if ( !parse_cmd( &newcmd) )
        {
            //Komandany tanap bolmady diýen ýalňyşlyk görkezilýär.
            print_err( 0, &inf_tok );
        }
        else
        {
            //çepki birlik bolsa şonuň deregine ýogsa operator deregine goýulýar.
            command_item op_as_subcmd;
            op_as_subcmd.type = CMD_ITEM;
            op_as_subcmd.cmd = newcmd;

            int put_place = *start;
            put_cmd_item( cmd->items, put_place, op_as_subcmd );

            //häzirki komanda gysgalýar.
            //cmd->items_num -= op-1 - *start ;

            move_cmd_items( *start+1, *start+2, cmd );
        }
    }
}

