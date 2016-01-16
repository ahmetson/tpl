#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "cmds.h"
#include "token/token_types.h"
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
#include "algor.h"
#include "dev_debug.h"
#include "error.h"
#include "main/inf.h"
#include "fns/fn_helpers.h"
#include "fns.h"

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
	   {is_cmd_conv_basic_type}
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
	"Sada tipini üýtgetme"
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
	{"sada tipi üýtgetme"}
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
    {cmd_arr_conv_basic_return_type}    //CMD_CLASS_CONV_BASIC_TYPE = 14;
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
    {semantic_cmd_conv_basic_type }//CMD_CLASS_CONV_BASIC_TYPE = 14;
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
    {MAX_TWO_ITEMS}                                   //CMD_CLASS_CONV_BASIC_TYPES  = 14;
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
    {cmd_conv_basic_type_c_code}  //CMD_CLASS_CONV_BASIC_TYPE = 14;
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


int recognize_cmd(command *cmd)
{
	// Komandany saygaryp bolmady ya ol gutarylmadyk
	if ((cmd->cmd_class<1 && cmd->cmd_type<1) || !cmd->is_compl)
	{
		return 0;
    }
	// Komandany saygaryp boldy
	return 1;
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
	// Komanda-da gaty kan tokenler bar
	if ((cmd->cmd_class!=CMD_CLASS_ARR &&
        !(cmd->cmd_class==CMD_CLASS_CALL_GLOB_VAR &&
          type==TOKEN_ITEM && tok.type_class==TOK_CLASS_CONST_DATA &&
          tok.potentional_types[0].type_num==INT_CONST_DATA_TOK_NUM)
        ) && cmd->is_compl && cmd->items_num==CMD_MAX_ITEMS[cmd->cmd_class][cmd->cmd_type])
    {
		return 0;
    }
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
        return 1;
    }
    else
    {
        return 0;
    }
}

// Komandalar bilen ishleyan bolum
int work_with_cmd()
{
	// Hazir TPL-in fayly komandalar bilen ishleyan boluminde
	int prev_part = CUR_PART;
	CUR_PART = 4;

    if (!cmd.items_num)
        return 0;

    // TODO
    // KOMANDA gutarylmadyk bolsa, yalnyshlyk haty komanda gutarylmady diyip gaytaryar
	// Komanda saygarylyp showly gutardy
	if (!recognize_cmd(&cmd))
	{
        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(&cmd));
	}
	// Eger komanda funksiyanyn ichinde bolsa,
    //      bolup biljek komandalara barlanyar.
	if (is_inside_fn() && is_unvalid_inside_fn_cmd(&cmd))
    {
        print_err(CODE4_UNSUPPORT_CMD_INSIDE_FN, (token *)inf_get_last_token(&cmd));
    }
    if (is_fn_def(&cmd))
    {
        check_semantics(&cmd);
        prepare_tmp_fn_data_container(&cmd);
    }
    else if (is_fn_close_cmd(&cmd))
    {
        move_tmp_fn_data_to_local();
        CUR_PART = prev_part;
        return 1;
    }
    if (is_inside_fn())
    {
        if (!(cmd.cmd_class==CMD_CLASS_FN_DEF &&
             cmd.cmd_type ==CMD_FN_DEF_TYPE )&& !add_to_tmp_fn_def_var_list(&cmd) && !add_to_tmp_fn_def_arr_list(&cmd))
        {
            add_to_tmp_fn_algor(&cmd);
        }
    }
    else
    {
        if (!add_to_def_var_list(&cmd) && !add_to_def_arr_list(&cmd) && !(cmd.cmd_class==CMD_CLASS_UTYPE && cmd.cmd_type==CMD_UTYPE_DEF_TYPE) ) // Komanda ulnini yglan etme dal eken
        {
            global_called_vars_add(&cmd);
            global_called_arrs_add(&cmd);
            // Komandany algoritme goshulyar

            algor_add_cmd(cmd);
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


/** Komandanyň içine birlik goşýar
**/
int cmd_add_item(command *cmd, int item_type, parenthesis p, command c, token t)
{

    /** 1) (Maksimum näçe elementi bardygy näbelli komandalary hasap etmeseň)
           Eger komanda-da maksimum bolup biljek birlikler bolsa Ýa-da
           Eger komandanyň öňki birligi gutarylmadyk bolsa, onda
            a) Eger öňki birlik token bolsa
                I) Komanda ýasap, öňki we häzirki birlik içine salynýar.
                II) Eger ýasalan komanda ýasalmasa, onda
                    1) Komanda tanalmady diýen ýalňyşlyk görkezilýär
                III) Eger täze komanda ýasalmasa, onda
                    1) Öňki token nädogry diýen ýalňyşlyk görkezilýär.
            b) Eger öňki birlik komanda bolsa
                I) Komandanyň birlikleriniň içine goşmaly birlik salynýar.
                II) Eger birlik goşulansoň, komanda tanalmasa
                    2) "Öňki birlik nädogry" diýen ýalňyşlyk görkezilýär.
        Ýogsa
            c) Eger komanda birlik goşulsa we komanda tanalmasa, onda
                I) Eger komanda, goşulmadyk birlikden başga özbaşdak manyny aňladýan we gutarylan bolsa
                    1) Täze komanda ýasalýar.
                    2) Öňki komanda, täze komandanyň birligi diýilip salynýar.
                    3) Goşulmaly birlik, komandanyň ikinji birligi diýilýär.
                    4) Eger täze komanda tanalmasa, onda
                        a) "Komandany tanap bolmady" diýen ýalňyşlyk görkezilýär.
                Ýogsa, eger komanda, goşulmadyk birlikden başga özbaşdak many aňladýan we gutarYLMADYK bolsa
                    5) Täze komanda ýasalýar.
                    6) Täze komandanyň birinji birligi diýilip goşulmaly birlik goýulýar.
                    7) Eger täze komanda tanalmasa
                        a) "Komandany tanap bolmady" diýen ýalňyşlyk görkezilýär.
                    8) Täze komanda, öňki komanda goşulýar.
                    9) Eger täze komanda goşulan soň, öňki komanda tanalmasa
                        a) "Komandany tanap bolmady" diýen ýalňyşlyk görkezilýär.
    */

    int prev_part = CUR_PART;
	CUR_PART = 4;

	if (item_type==TOKEN_ITEM && t.is_compl==0)
    {
        CUR_PART = 2;
        print_err(CODE2_PREV_TOKEN_INCORRECT, &t);
    }

	/// 1)
	//if (cmd->cmd_class!=CMD_CLASS_ARR && !(cmd->cmd_class==CMD_CLASS_CALL_GLOB_VAR && item_type==TOKEN_ITEM && t.type_class==TOK_CLASS_CONST_DATA && t.potentional_types[0].type_num==INT_CONST_DATA_TOK_NUM) ||
    // (cmd->items_num!=0 && !is_cmd_item_compl(get_cmd_item(cmd->items, cmd->items_num-1))))

	if (((cmd->cmd_class!=CMD_CLASS_ARR && !(cmd->cmd_class==CMD_CLASS_CALL_GLOB_VAR && item_type==TOKEN_ITEM && t.type_class==TOK_CLASS_CONST_DATA && t.potentional_types[0].type_num==INT_CONST_DATA_TOK_NUM))
        && cmd->items_num>=CMD_MAX_ITEMS[cmd->cmd_class][cmd->cmd_type]) || (cmd->items_num!=0 && !is_cmd_item_compl(get_cmd_item(cmd->items, cmd->items_num-1))) )
    {
        int last = cmd->items_num-1;

        /// 1.a)
        command_item *lci = get_cmd_item(cmd->items, last);
        if (lci->type==TOKEN_ITEM || lci->type==PAREN_ITEM)
        {
            token tmp_tok;
            parenthesis tmp_paren;
            if (item_type==TOKEN_ITEM)
            {
                tmp_tok = lci->tok;
            }
            else
                tmp_paren = lci->paren;
            /** ŞERT #2: Çepden saga, sagdan çepe gidýän komandalary saklaýan komandany ýasamaly:
                         1.a.I) - /// 1.a.III) aralygy alýar.
                        Eger soňky birlik token bolsa
                            1)Wagtlaýyn komanda ýasalýar (Wagtlaýyn komandanyň birliklerini kuçada saklajak wagtlaýyn ýer ýasalýar).
                            2)Soňky birlik bilen täze birlik alynyp wagtlaýyn komanda goşulýar.
                            3)Eger wagtlaýyn komanda tanalsa
                                a)sub-komanda ýasalayp, wagtlaýyn komandanyň parametrlerini alýar.
                                b)sub-komanda komandanyň soňky birliginiň deregine goýulýar.
                                ç)wagtlaýyn komandanyň birlikleri üçin goýlan kuçadaky ýer pozulýär.
                                d)eger komandanyň soňky birligi çalyşylan soň tanalmasa
                                    I)komandanyň öňki birligi ýene tokene öwrülýär.
                                    II)eger komanda özbaşdak many aňladýan bolsa
                                        1)soňky ulanylan sub-komandanyň parametrlerine komandanyň ähli parametrleri göçürilýär.
                                        2)komanda arassalanýar.
                                        3)komandanyň iki birligi bar diýilýär.
                                        4)Birinji birlige sub-komanda goýulýar
                                        5)Ikinji birlige goşulmaly birlik goýulýar.
                                        6)Eger komanda tanalmasa
                                            a)wagtlaýyn komandanyň birlikleri üçin goýlan kuçadaky ýer pozulýär.
                                            b)"Ýalňyşlyk #51"
                                    ýogsa
                                        7)"Ýalňyşlyk #40".
                            Ýogsa
                                e)eger komanda özbaşdak many aňladýan bolsa
                                    I)täze sub-komanda ýasalýar.
                                    II)sub-komandanyň parametrleri diýip komandanyň ähli parametrleri göçürilýär.
                                    III)komanda arassalanýar.
                                    IÝ)komandanyň iki birligi bar diýilýär.
                                    Ý)Birinji birlige sub-komanda goýulýar
                                    ÝI)Ikinji birlige goşulmaly birlik goýulýar.
                                    ÝII)Eger komanda tanalmasa
                                        1)wagtlaýyn komandanyň birlikleri üçin goýlan kuçadaky ýer pozulýär.
                                        2)"Ýalňyşlyk #50"
                                 ýogsa
                                    ÝIII)wagtlaýyn komandanyň birlikleri üçin goýlan kuçadaky ýer pozulýär.
                                    IÜ)"Ýalňyşlyk #51"

                */

            /// #2.1)
            command tmpcmd;
            init_cmd(&tmpcmd, 0);
            tmpcmd.items_num = 2;
            tmpcmd.items = -2;
            change_cmd_items_num(-2, tmpcmd.items_num);

            /// #2.2)
            put_cmd_item(tmpcmd.items, 0, *lci);

            command_item ci;
            ci.type = item_type;
            if (item_type==TOKEN_ITEM)
                ci.tok  = t;
            else
                ci.paren  = p;
            put_cmd_item(tmpcmd.items, 1, ci);

            /// #2.3)
            if (parse_cmd(&tmpcmd))
            {
                /// #2.3.a)
                command new_subcmd;
                init_cmd(&new_subcmd, 0);
                new_subcmd = tmpcmd;

                new_subcmd.items = subcmd_items_add(new_subcmd.items_num);
                int i;
                for(i=0; i<new_subcmd.items_num; ++i)
                {
                    put_cmd_item(new_subcmd.items, i,  *get_cmd_item(tmpcmd.items, i));
                }

                add_to_def_var_list(&new_subcmd);
                add_to_def_arr_list(&new_subcmd);
                global_called_vars_add(&new_subcmd);
                global_called_arrs_add(&new_subcmd);

                /// #2.3.b)
                command_item last_c;
                last_c.type = CMD_ITEM;
                last_c.cmd  = new_subcmd;

                put_cmd_item(cmd->items, last, last_c);

                /// #2.3.ç)
                free(TMP_CMD_ITEMS_LIST);
                TMP_CMD_ITEMS_LIST = NULL;

                /// #2.3.d)
                if (!parse_cmd(cmd))
                {
                    /// #2.3.d.I)
                    lci->type = item_type;
                    if (item_type==TOKEN_ITEM)
                    {
                        lci->tok=tmp_tok;
                    }
                    else
                        lci->paren = tmp_paren;
                    lci->cmd = get_empty_cmd();

                    /// #2.3.d.II)
                    if (parse_cmd(cmd)  && cmd->is_compl)
                    {
                        /// #2.3.d.II.1)
                        init_cmd(&new_subcmd, 0);
                        new_subcmd = *cmd;
                        new_subcmd.items = subcmd_items_add(new_subcmd.items_num);
                        int i;
                        for(i=0; i<new_subcmd.items_num; ++i)
                        {
                            put_cmd_item(new_subcmd.items, i, *get_cmd_item(cmd->items, i));
                        }

                        add_to_def_var_list(&new_subcmd);
                        add_to_def_arr_list(&new_subcmd);
                        global_called_vars_add(&new_subcmd);
                        global_called_arrs_add(&new_subcmd);

                        /// #2.3.d.II.2)
                        int tmp_items_num = cmd->items;
                        init_cmd(cmd, 0);

                        /// #2.3.d.II.3)
                        cmd->items_num = 2;
                        cmd->items = tmp_items_num;
                        change_cmd_items_num(cmd->items, cmd->items_num);

                        /// #2.3.d.II.4)
                        command_item ci1;
                        ci1.type = CMD_ITEM;
                        ci1.cmd  = new_subcmd;
                        put_cmd_item(cmd->items , 0, ci1);

                        /// #2.3.d.II.5)
                        command_item ci2;
                        ci2.type = item_type;
                        if(item_type==TOKEN_ITEM)
                            ci2.tok = t;
                        else if(item_type==CMD_ITEM)
                            ci2.cmd = c;
                        else if(item_type==PAREN_ITEM)
                            ci2.paren = p;
                        put_cmd_item(cmd->items , 1, ci2);

                        /// #2.3.d.II.6)
                        if (!parse_cmd(cmd))
                        {
                            /// #2.3.d.II.6.a)
                            print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                        }
                        add_to_def_var_list(cmd);
                        add_to_def_arr_list(cmd);
                        global_called_vars_add(cmd);
                        global_called_arrs_add(cmd);
                    }
                    else
                    {
                        /// #2.3.d.II.7)
                        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                    }
                }
            }
            else
            {
                /// #2.3.e)
                if (cmd->items_num && cmd->is_compl)
                {
                    /// #2.3.e.I)
                    command subcmd;

                    /// #2.3.e.II)
                    subcmd = *cmd;
                    subcmd.items = subcmd_items_add(subcmd.items_num);
                    int i;
                    for(i=0; i<subcmd.items_num; ++i)
                    {
                        put_cmd_item(subcmd.items , i, *get_cmd_item(cmd->items, i));
                    }

                    /// #2.3.e.III)
                    int tmp_items_num = cmd->items;
                    init_cmd(cmd, 0);

                    /// #2.3.e.IÝ)
                    cmd->items_num = 2;
                    cmd->items = tmp_items_num;
                    change_cmd_items_num(cmd->items, cmd->items_num);

                    /// #2.3.e.Ý)
                    command_item ci1;
                    ci1.type = CMD_ITEM;
                    ci1.cmd  = subcmd;
                    put_cmd_item(cmd->items , 0, ci1);
                    /// #2.3.e.ÝI)
                    command_item ci2;
                    ci2.type = item_type;
                    if(item_type==TOKEN_ITEM)
                        ci2.tok = t;
                    else if(item_type==CMD_ITEM)
                        ci2.cmd = c;
                    else if(item_type==PAREN_ITEM)
                        ci2.paren = p;
                    put_cmd_item(cmd->items , 1, ci2);
                    /// #2.3.e.ÝII)
                    if (!parse_cmd(cmd))
                    {
                        /// #2.3.e.ÝII.1)
                        free(TMP_CMD_ITEMS_LIST);
                        TMP_CMD_ITEMS_LIST = NULL;
                        /// #2.3.e.ÝII.2)
                        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                    }
                }
                /// Komanda özbaşdak manyny aňladanok. Häzirki durşyna
                else
                {
                    /// #2.3.e.ÝIII)
                    free(TMP_CMD_ITEMS_LIST);
                    TMP_CMD_ITEMS_LIST = NULL;
                    /// #2.3.e.IÜ)
                    print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                }
            }
        }
        /// 1.b)
        else if(lci->type==CMD_ITEM)
        {

            /** ŞERT #3: Dört we ondanam kän komandany saklaýan komanda tanalar ýaly edilýär.
                         Onuň üçin, iň soňky komanda täze komandanyň bölegi bölegine öwrülýär.
                #3.1)Eger iň soňky birlik gutarylan bolsa, onda
                    #3.1.a)Täze komanda ýasalýar.
                    #3.1.b)Komanda iň soňky birlik geçirilýär.
                    #3.1.ç)Täze komandanyň ikinji birligi diýip goşulmaly obýek geçirilýär.
                    #3.1.d)Eger täze komanda tanalmasa
                        #3.1.d.I)"Komanda tanalmady" diýen ýalňyşlyk görkezilýär.
                    Ýogsa
                        #3.1.d.II)Täze komanda iň soňky birligiň deregine goýulýar.
                        #3.1.d.III)Eger komanda iň soňky birligi üýtgän soň tanalmasa
                            #3.1.d.III.1) "Komanda tanalmady" diýen ýalňyşlyk görkezilýär
                Ýogsa eger token gutarylmadyk bolsa
                ** öňki şertiň 1.b) bölümini alýar **
                    #3.1.e)iň soňky birlige goşulmaly birlik goşulýar.
                    #3.1.ä)Eger iň soňky birlik tanalmasa
                        #3.1.ä.I)"Komanda tanalmady" diýen ýalňyşlyk görkezilýär.
            */
            command *lc = &lci->cmd;
            if (lc->is_compl && lc->items_num)
            {
                /// #3.1.a)
                command newlc; init_cmd(&newlc, 0);
                newlc.items_num = 2;
                newlc.items = -2;
                change_cmd_items_num(-2, newlc.items_num);

                /// #3.1.b)
                command newlcsub;
                newlcsub = *lc;
                newlcsub.items = subcmd_items_add(newlcsub.items_num);
                int i;
                for (i=0; i<newlcsub.items_num; ++i)
                {
                    put_cmd_item(newlcsub.items, i, *get_cmd_item(lc->items, i));
                }
                command_item fci;
                fci.type = CMD_ITEM;
                fci.cmd  = newlcsub;
                put_cmd_item(newlc.items , 0, fci);

                /// #3.1.c)
                command_item sci;
                sci.type = item_type;
                if(item_type==TOKEN_ITEM)
                    sci.tok = t;
                else if(item_type==CMD_ITEM)
                    sci.cmd = c;
                else if(item_type==PAREN_ITEM)
                    sci.paren = p;
                put_cmd_item(newlc.items , 1, sci);

                /// #3.1.d)
                if (!parse_cmd(&newlc))
                {
                    /// #3.1.d.I)
                    free(TMP_CMD_ITEMS_LIST);
                    TMP_CMD_ITEMS_LIST = NULL;

                    print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(lc));
                }
                else
                {
                    /// #3.1.d.II)
                    if (newlc.items_num!=lc->items_num)
                    {
                        change_cmd_items_num(lc->items, newlc.items_num);
                    }
                    lc->cmd_class = newlc.cmd_class;
                    lc->cmd_type = newlc.cmd_type;
                    lc->is_compl = newlc.is_compl;
                    lc->items_num = newlc.items_num;
                    lc->ns = newlc.ns;
                    lc->parenthesis = newlc.parenthesis;
                    lc->value_class = newlc.value_class;
                    lc->value_type = newlc.value_type;
                    for(i=0; i<lc->items_num; ++i)
                    {
                        put_cmd_item(lc->items, i, *get_cmd_item(newlc.items, i));
                    }
                    add_to_def_var_list(lc);
                    add_to_def_arr_list(lc);
                    global_called_vars_add(lc);
                    global_called_arrs_add(lc);
                    free(TMP_CMD_ITEMS_LIST);
                    TMP_CMD_ITEMS_LIST = NULL;

                    /// #3.1.da.III)
                    if (!parse_cmd(cmd))
                    {
                        /// #3.1.d.III.1)
                        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                    }
                }
            }
            else
            {

                /// #3.1.e)
                lc->items_num++;
                change_cmd_items_num(lc->items, lc->items_num);

                command_item ci;
                ci.type = item_type;
                if(item_type==TOKEN_ITEM)
                    ci.tok = t;
                else if (item_type==CMD_ITEM)
                    ci.cmd = c;
                else if (item_type==PAREN_ITEM)
                {
                    ci.paren = p;
                }
                put_cmd_item(lc->items, lc->items_num-1, ci);

                /// #3.1.ä)
                if (!parse_cmd(lc))
                {
                    /// #3.1.ä.I)
                    print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(lc));
                }
                else if (!parse_cmd(cmd))
                {
                    /// 1.b.ä.I)
                    print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                }
                add_to_def_var_list(lc);
                add_to_def_arr_list(lc);
                global_called_vars_add(lc);
                global_called_arrs_add(lc);
            }
        }
        else if(lci->type==PAREN_ITEM)
        {
            /** ŞERT #4: Ýaýlar birlik hökümnde ulanylanda komanda ýasalmagy

                #4.1)Täze komanda ýasalýar.
                #4.2)Öňki komandadaky ähli birlikler täze komanda salynýar.

                #4.3)Öňki komandadaky birlikleriň sany 2-ä getirilýär.

                #4.4)Birinji birlik diýilip täze ýasalan komanda goýulýar.

                #4.5)Ikinji birlik diýip goşulmaly birlik goýulýar.

                #4.6)Eger komanda iki birlik bilen tanalmasa
                    #4.6.a)"Token tanalmady" diýen ýalňyşlyk görkezilýär.
            */
            /// #4.1)
            command fc;

            /// #4.2)
            fc = *cmd;
            fc.items = subcmd_items_add(fc.items_num);
            int i;
            for (i=0; i<fc.items_num; ++i)
            {
                put_cmd_item(fc.items, i, *get_cmd_item(cmd->items, i));
            }


            /// #4.3)
            if (cmd->items_num!=2)
            {
                cmd->items_num=2;
                change_cmd_items_num(cmd->items, cmd->items_num);
            }

            /// #4.4)
            command_item fci;
            fci.type = CMD_ITEM;
            fci.cmd  = fc;
            put_cmd_item(cmd->items , 0, fci);

            /// #4.5)
            command_item sci;
            sci.type = item_type;
            if(item_type==TOKEN_ITEM)
                sci.tok = t;
            else if(item_type==CMD_ITEM)
                sci.cmd = c;
            else if(item_type==PAREN_ITEM)
                sci.paren = p;
            put_cmd_item(cmd->items , 1, sci);

            /// #4.6)
            if (!parse_cmd(cmd))
            {
                /// #4.6.a)
                print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
            }
        }
    }
    else
    {
        if (!add_to_cmd(cmd, item_type, t, p, c))
        {
            if (item_type==TOKEN_ITEM)
            {
                print_err(CODE4_CANT_ADD_TOKEN, &t);
            }
            else if(item_type==CMD_ITEM)
            {
                print_err(CODE4_CANT_ADD_TOKEN, (token *)inf_get_last_token(&c));
            }
            else if(item_type==PAREN_ITEM)
            {
                print_err(CODE4_CANT_ADD_TOKEN, &inf_tok);
            }
        }

        /// C)

        if (!parse_cmd(cmd))
        {
            cmd->items_num--;
            /// C.I
            if (cmd->items_num && parse_cmd(cmd) && cmd->is_compl)
            {
                add_to_def_var_list(cmd);
                add_to_def_arr_list(cmd);
                global_called_vars_add(cmd);
                global_called_arrs_add(cmd);
                /// C.I.1)
                command new_cmd;
                new_cmd = *cmd;
                new_cmd.items = subcmd_items_add(cmd->items_num);
                int i=0;
                for (i=0; i<cmd->items_num; ++i)
                {
                    put_cmd_item(new_cmd.items, i, *get_cmd_item(cmd->items, i));
                }

                int tmp_items_num = cmd->items;
                init_cmd(cmd, 0);
                cmd->items_num = 2;
                cmd->items = tmp_items_num;
                if (new_cmd.items_num!=2)
                {
                    change_cmd_items_num(cmd->items, cmd->items_num);
                }

                /// C.I.2)
                // Komandanyň birinji birligi - Önki komanda goşulýar
                command_item cmd_item = {};
                cmd_item.type = CMD_ITEM;
                cmd_item.cmd = new_cmd;
                put_cmd_item(cmd->items , 0, cmd_item);

                /// C.I.3)
                // Komandanyň ikinji birligi   - Täze token goşulýar
                command_item cmd_tok_item;
                cmd_tok_item.type  = item_type;
                if (item_type==PAREN_ITEM)
                    cmd_tok_item.paren = p;
                else if(item_type==TOKEN_ITEM)
                    cmd_tok_item.tok = t;
                else if(item_type==CMD_ITEM)
                    cmd_tok_item.cmd = c;
                put_cmd_item(cmd->items , 1, cmd_tok_item);

                /// C.I.4)
                if (!parse_cmd(cmd))
                {
                    /// C.I.4.a)
                    print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                }
            }
            else
            {
                /// C.I.5)
                command new_subcmd;
                init_cmd(&new_subcmd, 0);

                /// C.I.6)
                command_item cmd_tok_item;
                cmd_tok_item.type = item_type;
                if (item_type==PAREN_ITEM)
                    cmd_tok_item.paren = p;
                else if(item_type==TOKEN_ITEM)
                    cmd_tok_item.tok = t;
                else if(item_type==CMD_ITEM)
                    cmd_tok_item.cmd = c;

                new_subcmd.items_num = 1;
                new_subcmd.items = subcmd_items_add(new_subcmd.items_num);
                put_cmd_item(new_subcmd.items , 0, cmd_tok_item);

                /// C.I.7)
                if (!parse_cmd(&new_subcmd))
                {
                    cmd->items_num++;
                    /// C.I.7.a)
                    print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(&new_subcmd));
                }

                // Eger ulni yglan etme bolsa, onda ichki komandany ulni yglan etmelerin ichine gechirmeli
                add_to_def_var_list(&new_subcmd);
                add_to_def_arr_list(&new_subcmd);
                global_called_vars_add(&new_subcmd);
                global_called_arrs_add(&new_subcmd);

                cmd->items_num++;
                //change_cmd_items_num(cmd->items, cmd->items_num);

                /// C.I.8)
                // Komandanyň birinji birligi - Önki komanda goşulýar
                command_item cmd_subitem = {};
                cmd_subitem.type = CMD_ITEM;
                cmd_subitem.cmd = new_subcmd;
                put_cmd_item(cmd->items , cmd->items_num-1, cmd_subitem);

                /// C.I.9)
                /** ŞERT #5: Eger komanda-da gutarylmadyk birlik bar bolsa (is_cmd_not_compl_item_exist(cmd, recursive=0)),
                    Onda birlik, öz duran ýerinde bolýan birligiň - komandanyň talap edýän birlik görnüşiniň
                    haýsy bolsa biri bolup bilýämi ýa ýokdugy barlanýar (is_cmd_item_can_be_needed(cmd)).

                    Eger ýokarky agzalan iki funksiýa-da ýalňyşlyk gaýtarsa, ýalňyşlyk görkezilýär.

                    ** Komanda algoritme goşulanda **
                    Eger komanda-da gutarylmadyk birlik bar bolsa (is_cmd_not_compl_item_exist(cmd, recursive=1))
                        "Gutarylmadyk komanda birlik bar" diýen ýalňyşlyk görkezilýär. */

                if (!parse_cmd(cmd))
                {
                    /// C.I.9.a)
                    print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                }
            }
        }
    }

	CUR_PART = prev_part;

    /// Komanda - blok bolsa üýtgetmeli maglumatlary ýerine ýetirýän funksiýa çagyrmaly
    work_with_blocks(cmd);

	return 1;
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
    int j, len;
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
    int j, len;
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
    int j, len;
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
        array_item *gi = glob_arrs_def_get_by_name(GLOB_VAR_DECS[j].name);
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

