/*
All Command information
*/
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
#include "algor.h"
#include "dev_debug.h"
#include "error.h"
#include "main/inf.h"

// Komandanyň birliginiň nomeri
const int UNKNOWN_ITEM  = 0;
const int TOKEN_ITEM    = 1;
const int CMD_ITEM      = 2;
const int PAREN_ITEM    = 3;



// Komandalaryn sanawy
int CMD_CLASS_UNKNOWN = 0;
int CMD_CLASS_DEF_VAR = 1;
int CMD_CLASS_ASSIGN = 2;
int CMD_CLASS_FN = 3;
int CMD_CLASS_CALL_GLOB_VAR = 4;
int CMD_CLASS_ARIF = 5;

int GLOB = 0;
int LOCAL = 1;

is_cmd_item cmd_types[] = {
	   {is_cmd_def_var},
	   {is_cmd_assign},
	   {is_cmd_fn_call},
	   {is_cmd_call_glob_var},
	   {is_cmd_arif}
};

// Dine debug uchin ulanylyar. Komanda tiplerinin atlary
char *cmd_classes[] = {
	"var",
	"assign",
	"function",
	"call_glob_var",
	"arifmethic"
};

char *cmd_class_types[][MAX_CLASS_TYPES] = {
	{"def", 0},			// Var class
	{"left", "right"},
	{"call", 0},
	{""},
	{"low_prior", "high_prior"}
};


int (*CMD_RETURN_TYPE[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd, int *cmd_class, int *cmd_type) = {
    {empty_cmd_return_type,         empty_cmd_return_type},
    {cmd_def_var_return_type,       empty_cmd_return_type},//CMD_CLASS_DEF_VAR = 1;
    {empty_cmd_return_type,         empty_cmd_return_type},//CMD_CLASS_ASSIGN = 2;
    {cmd_fn_call_return_type,       empty_cmd_return_type},//CMD_CLASS_FNS = 3;
    {cmd_call_glob_var_return_type, empty_cmd_return_type},//CMD_CLASS_CALL_GLOB_VAR_RETURN_TYOE = 4;
    {cmd_arif_return_type,          cmd_arif_return_type} //CMD_CLASS_ARIF = 5;
};


// Komandanyň klasy we tipi boýunça semantikasyny barlaýan funksiýalar
int (*CMD_CHECK_SEMANTICS[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd) = {
    {empty_cmd_checking_semantic, empty_cmd_checking_semantic},
    {empty_cmd_checking_semantic, empty_cmd_checking_semantic},//CMD_CLASS_DEF_VAR = 1;
    {semantic_cmd_assign,         empty_cmd_checking_semantic},//CMD_CLASS_ASSIGN = 2;
    {semantic_cmd_fn_call,        empty_cmd_checking_semantic},//CMD_CLASS_FNS = 3;
    {semantic_cmd_call_glob_var,  empty_cmd_checking_semantic},//CMD_CLASS_CALL_GLOB_VAR = 4;
    {semantic_cmd_arif,           semantic_cmd_arif}           //CMD_CLASS_ARIF = 5;
};
// Command can contain maximum 3 tokens

// Komandanyň klasy we tipi boýunça semantikasyny barlaýan funksiýalar
int CMD_MAX_ITEMS[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES] = {
    {MAX_NO_ITEMS,    MAX_NO_ITEMS},
    {MAX_THREE_ITEMS, MAX_THREE_ITEMS}, //CMD_CLASS_DEF_VAR = 1;
    {MAX_THREE_ITEMS, MAX_THREE_ITEMS}, //CMD_CLASS_ASSIGN = 2;
    {MAX_TWO_ITEMS,   MAX_TWO_ITEMS},   //CMD_CLASS_FNS = 3;
    {MAX_TWO_ITEMS,   MAX_TWO_ITEMS},   //CMD_CLASS_CALL_GLOB_VAR = 4;
    {MAX_THREE_ITEMS, MAX_THREE_ITEMS}  //CMD_CLASS_ARIF = 5;
};


// Komandalaryň klasy we tipi boýunça, komandanyň tekstini ýazýan funksiýa çagyrylýar.
void (*CMD_GET_C_CODE[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd, char **l, int *len) = {
    {empty_cmd_c_code,          empty_cmd_c_code},
    {cmd_def_var_as_subcmd_c_code, empty_cmd_c_code}, //CMD_CLASS_DEF_VAR = 1;
    {cmd_assign_c_code,         cmd_assign_c_code},//CMD_CLASS_ASSIGN = 2;
    {cmd_fn_call_c_code,        empty_cmd_c_code},  //CMD_CLASS_FNS = 3;
    {cmd_call_glob_var_c_code,  empty_cmd_c_code},  //CMD_CLASS_CALL_GLOB_VAR = 4;
    {cmd_arif_c_code,           cmd_arif_c_code}  //CMD_CLASS_ARIF = 5;
};


void init_cmd(command *cmd, char free_items)
{
	if (free_items && cmd->items_num)
	{
	    free(cmd->items);
	    cmd->items = 0;			// Kop free() funksiyany chagyrmakdan gorayar
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
		//debug_cmd(cmd);
		return 0;
    }
	// Komandany saygaryp boldy
	return 1;
}

/**
 * Tokeni saygarylyar
**/
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
	if (cmd->items_num==CMD_MAX_ITEMS[cmd->cmd_class][cmd->cmd_type])
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

	long size = (cmd->items_num+1)*sizeof(*cmd->items);
	command_item *tmp = realloc(cmd->items, size);
	if (tmp!=NULL)
	{
		cmd->items = tmp;
		cmd->items[cmd->items_num++] = cmd_item;
		//printf("Kuchada yer berildi (tokenlerin sany:%d gowrumi:%d)\n", cmd->items_num, size);
		return 1;
	}
	else
	{
		//printf("Kuchada yer tapylmady\n");
		return 0;
	}

}

// Komandalar bilen ishleyan bolum
int work_with_cmd()
{

	// Hazir TPL-in fayly komandalar bilen ishleyan boluminde
	int prev_part = CUR_PART;
	CUR_PART = 4;

    // TODO
    // KOMANDA gutarylmadyk bolsa, yalnyshlyk haty komanda gutarylmady diyip gaytaryar
	// Komanda saygarylyp showly gutardy
	if (!recognize_cmd(&cmd))
	{
	    //debug_cmd(&cmd);
        printf("SALAM 2");
		print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(&cmd));
	}


	if (!add_to_def_var_list(&cmd)) // Komanda ulnini yglan etme dal eken
    {
        global_called_vars_add(&cmd);
		// Komandany algoritme goshulyar
		algor_add_cmd(cmd);
	}

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

void empty_cmd_c_code(command *cmd, char **l, int *len)
{
    return;
}


/** Komandanyň içine birlik goşýar
**/
int cmd_add_item(command *cmd, int item_type, parenthesis p, command c, token t)
{

    /** 1) Eger komanda-da maksimum bolup biljek birlikler bolsa Ýa-da
    /** Eger komandanyň öňki birligi gutarylmadyk bolsa, onda
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
	if (cmd->items_num>=CMD_MAX_ITEMS[cmd->cmd_class][cmd->cmd_type] ||
        (cmd->items_num!=0 && !is_cmd_item_compl(&cmd->items[cmd->items_num-1])))
    {
        int last = cmd->items_num-1;
        /// 1.a)
        if (cmd->items[last].type==TOKEN_ITEM)
        {
            token tmp_tok = cmd->items[last].tok;
            // Ichki komandalaryn sanyny kopeltmeli
			//printf("Ichki komandalar uchin yer bar\n");
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
                //init_cmd(&new_subcmd, 0);
                //
                tmpcmd.items_num = 2;

                //
                tmpcmd.items = malloc(sizeof(**GLOB_SUBCMD_ITEMS_LIST)*tmpcmd.items_num);

                /// #2.2)
                //new_subcmd.items[0] = cmd->items[last];
                tmpcmd.items[0] = cmd->items[last];

			    command_item ci;
			    ci.type = item_type;
			    ci.tok  = t;
			    //new_subcmd.items[1] = ci;
			    tmpcmd.items[1] = ci;

			    //new_subcmd.items = GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1];

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
                        new_subcmd.items[i] = tmpcmd.items[i];
                    }

                    add_to_def_var_list(&new_subcmd);
                    global_called_vars_add(&new_subcmd);

                    /// #2.3.b)
                    command_item last_c;
                    last_c.type = CMD_ITEM;
                    last_c.cmd  = new_subcmd;
                    cmd->items[last] = last_c;

                    /// #2.3.ç)
                    free(tmpcmd.items);

                    /// #2.3.d)
                    if (!parse_cmd(cmd))
                    {

                        /// #2.3.d.I)
                        cmd->items[last].type = TOKEN_ITEM;
                        cmd->items[last].cmd = get_empty_cmd();
                        cmd->items[last].tok = tmp_tok;
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
                                new_subcmd.items[i] = cmd->items[i];
                            }

                            add_to_def_var_list(&new_subcmd);
                            global_called_vars_add(&new_subcmd);

                            /// #2.3.d.II.2)
                            init_cmd(cmd, 0);
                            /// #2.3.d.II.3)
                            cmd->items_num = 2;
                            /// #2.3.d.II.4)
                            command_item ci1;
                            ci1.type = CMD_ITEM;
                            ci1.cmd  = new_subcmd;
                            cmd->items[0] = ci1;
                            /// #2.3.d.II.5)
                            command_item ci2;
                            ci2.type = item_type;
                            if(item_type==TOKEN_ITEM)
                                ci2.tok = t;
                            else if(item_type==CMD_ITEM)
                                ci2.cmd = c;
                            else if(item_type==PAREN_ITEM)
                                ci2.paren = p;
                            cmd->items[1] = ci2;
                            /// #2.3.d.II.6)
                            if (!parse_cmd(cmd))
                            {
                                //debug_cmd(cmd);
                                /// #2.3.d.II.6.a)
                                printf("SALAM 51");
                                print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                            }
                            add_to_def_var_list(cmd);
                            global_called_vars_add(cmd);
                        }
                        else
                        {

                            /// #2.3.d.II.7)
                            printf("SALAM 40");
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
                            subcmd.items[i] = cmd->items[i];
                        }
                        /// #2.3.e.III)
                        init_cmd(cmd, 0);
                        /// #2.3.e.IÝ)
                        cmd->items_num = 2;
                        /// #2.3.e.Ý)
                        command_item ci1;
                        ci1.type = CMD_ITEM;
                        ci1.cmd  = subcmd;
                        cmd->items[0] = ci1;
                        /// #2.3.e.ÝI)
                        command_item ci2;
                        ci2.type = item_type;
                        if(item_type==TOKEN_ITEM)
                            ci2.tok = t;
                        else if(item_type==CMD_ITEM)
                            ci2.cmd = c;
                        else if(item_type==PAREN_ITEM)
                            ci2.paren = p;
                        cmd->items[1] = ci2;
                        /// #2.3.e.ÝII)
                        if (!parse_cmd(cmd))
                        {
                            /// #2.3.e.ÝII.1)
                            free(tmpcmd.items);
                            /// #2.3.e.ÝII.2)
                            printf("SALAM 50");
                            print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                        }
                    }
                    /// Komanda özbaşdak manyny aňladanok. Häzirki durşyna
                    else
                    {
                        /// #2.3.e.ÝIII)
                        free(tmpcmd.items);
                        /// #2.3.e.IÜ)
                        printf("SALAM 51");
                        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                    }
                }
        }
        /// 1.b)
        else if(cmd->items[last].type==CMD_ITEM)
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
            command *lc = &cmd->items[last].cmd;
            if (lc->is_compl && lc->items_num)
            {
                /// #3.1.a)
                command newlc; init_cmd(&newlc, 0);
                newlc.items_num = 2;
                newlc.items = NULL;
                newlc.items = realloc(newlc.items, sizeof(*newlc.items)*newlc.items_num);

                /// #3.1.b)
                command newlcsub;
                newlcsub = *lc;
                newlcsub.items = subcmd_items_add(newlcsub.items_num);
                int i;
                for (i=0; i<newlcsub.items_num; ++i)
                {
                    newlcsub.items[i] = lc->items[i];
                }
                command_item fci;
                fci.type = CMD_ITEM;
                fci.cmd  = newlcsub;
                newlc.items[0] = fci;

                /// #3.1.c)
                command_item sci;
                sci.type = item_type;
                if(item_type==TOKEN_ITEM)
                    sci.tok = t;
                else if(item_type==CMD_ITEM)
                    sci.cmd = c;
                else if(item_type==PAREN_ITEM)
                    sci.paren = p;
                newlc.items[1] = sci;

                /// #3.1.d)
                if (!parse_cmd(&newlc))
                {
                    /// #3.1.d.I)
                    free(newlc.items);
                    newlc.items = NULL;
                    printf("SALAM 60");
                    print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(lc));
                }
                else
                {
                    /// #3.1.d.II)
                    if (newlc.items_num!=lc->items_num)
                    {
                        lc->items = realloc(lc->items, sizeof(*lc->items)*lc->items_num);
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
                        lc->items[i] = newlc.items[i];
                    }
                    add_to_def_var_list(lc);
                    global_called_vars_add(lc);
                    free(newlc.items);
                    newlc.items = NULL;

                    /// #3.1.da.III)
                    if (!parse_cmd(cmd))
                    {
                        /// #3.1.d.III.1)
                        printf("SALAM 60");
                        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                    }
                    //debug_cmd(cmd);
                }
            }
            else
            {
                //debug_cmd(lc);
                /// #3.1.e)
                lc->items_num++;
                long size =cmd->items[last].cmd.items_num*(sizeof(*cmd->items[last].cmd.items));
                cmd->items[last].cmd.items = realloc(cmd->items[last].cmd.items, size);

                command_item ci;
                ci.type = item_type;
                if(item_type==TOKEN_ITEM)
                    ci.tok = t;
                else
                    ci.cmd = c;
                cmd->items[last].cmd.items[cmd->items[last].cmd.items_num-1] = ci;

                /// #3.1.ä)
                if (!parse_cmd(&cmd->items[last].cmd))
                {
                    /// #3.1.ä.I)
                    debug_cmd(&cmd->items[last].cmd);
                    printf("SALAM 21");
                    print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(&cmd->items[last].cmd));
                }
                else if (!parse_cmd(cmd))
                {
                    //debug_cmd(cmd);
                     /// 1.b.ä.I)
                    printf("SALAM 22");
                    print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                }
                add_to_def_var_list(&cmd->items[last].cmd);
                global_called_vars_add(&cmd->items[last].cmd);
            }
        }
    }
    else
    {
        if (!add_to_cmd(cmd, item_type, t, p, c))
        {
            if (item_type==TOKEN_ITEM)
                print_err(CODE4_CANT_ADD_TOKEN, &t);
            else if(item_type==CMD_ITEM)
                print_err(CODE4_CANT_ADD_TOKEN, (token *)inf_get_last_token(&c));
            else if(item_type==PAREN_ITEM)
                print_err(CODE4_CANT_ADD_TOKEN, &inf_tok);
        }

        //printf("komanda barlanmana gechmeli\n");
        /// C)
        if (!parse_cmd(cmd))
        {
            cmd->items_num--;
            /// C.I
            if (cmd->items_num && parse_cmd(cmd) && cmd->is_compl)
            {
                add_to_def_var_list(cmd);
                global_called_vars_add(cmd);
                /// C.I.1)
                command new_cmd;
                new_cmd = *cmd;
                new_cmd.items = subcmd_items_add(cmd->items_num);
                int i=0;
                for (i=0; i<cmd->items_num; ++i)
                {
                    new_cmd.items[i] = cmd->items[i];
                }

                init_cmd(cmd, 0);
                cmd->items_num = 2;

                command_item *tmp = realloc(cmd->items, cmd->items_num*sizeof(command_item));
                if (tmp==NULL)
                {
                    printf("SALAM 4");
                    print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                }
                else
                {
                    cmd->items = tmp;

                    /// C.I.2)
                    // Komandanyň birinji birligi - Önki komanda goşulýar
                    command_item cmd_item = {};
                    cmd_item.type = CMD_ITEM;
                    cmd_item.cmd = new_cmd;
                    cmd->items[0] = cmd_item;

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
                    cmd->items[1] = cmd_tok_item;

                    /// C.I.4)
                    if (!parse_cmd(cmd))
                    {
                        printf("SALAM 5");
                        /// C.I.4.a)
                        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                    }
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
                new_subcmd.items[0] = cmd_tok_item;
                //debug_cmd(&new_subcmd);

                /// C.I.7)
                if (!parse_cmd(&new_subcmd))
                {
                    //debug_cmd(&new_subcmd);
                    /// C.I.7.a)
                    printf("SALAM 7");
                    print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(&new_subcmd));
                }


                // Eger ulni yglan etme bolsa, onda ichki komandany ulni yglan etmelerin ichine gechirmeli
                add_to_def_var_list(&new_subcmd);
                global_called_vars_add(&new_subcmd);

                cmd->items_num++;
                command_item *tmp = realloc(cmd->items, cmd->items_num*sizeof(*cmd->items));
                if (tmp==NULL)
                {
                    printf("SALAM 8");
                    //printf("Komanda ichki komandany goshmak uchin yer tapylmady\n");
                    print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                }
                else
                {
                    //printf("Komanda ichki komandany goshmak uchin yer bar\n");
                    cmd->items = tmp;
                    //cmd_add_subcmd(cmd, new_cmd);

                    /// C.I.8)
                        // Komandanyň birinji birligi - Önki komanda goşulýar
                    command_item cmd_subitem = {};
                    cmd_subitem.type = CMD_ITEM;
                    cmd_subitem.cmd = new_subcmd;
                    cmd->items[cmd->items_num-1] = cmd_subitem;

                    /// C.I.9)
                    if (!parse_cmd(cmd))
                    {
                        debug_cmd(cmd);
                        /// C.I.9.a)
                        printf("SALAM 9");
                        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                    }
                }
            }
        }
    }

	CUR_PART = prev_part;

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

    if (p->elems[0].type==TOKEN_ITEM)
        return p->elems[0].tok.is_compl;
    else if(p->elems[0].type==CMD_ITEM)
        return p->elems[0].cmd.is_compl;
    else if(p->elems[0].type==PAREN_ITEM)
        return is_paren_compl(&p->elems[0].paren);
}


command_item *subcmd_items_add(unsigned int items_num)
{
    /// Komandanyň pod sanawynda hökman bir birlik bolaýmaly
    if (!items_num)
    {
        printf("SALAM 41");
        CUR_PART = 4;
				//printf("Ichki komandalar uchin yer taplymady\n");
        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(&cmd));
    }
    GLOB_SUBCMDS_NUM++;
    command_item**tmp = realloc(GLOB_SUBCMD_ITEMS_LIST, sizeof(*GLOB_SUBCMD_ITEMS_LIST)*GLOB_SUBCMDS_NUM);
    if (tmp!=NULL)
        GLOB_SUBCMD_ITEMS_LIST = tmp;
    if (GLOB_SUBCMD_ITEMS_LIST==NULL)
    {
        printf("SALAM 11");
        CUR_PART = 4;
				//printf("Ichki komandalar uchin yer taplymady\n");
        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(&cmd));
    }

    long size = sizeof(**GLOB_SUBCMD_ITEMS_LIST)*items_num;
    GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1] = NULL;
    GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1] = realloc(GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1], size);

    return GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1];
}


int is_glob_decl_support_cmd(command *cmd)
{
    if (cmd->cmd_class==CMD_CLASS_DEF_VAR && cmd->ns==LOCAL)
        return 1;
    return 0;
}


void glob_vars_decl_add(command *cmd)
{
    if (!cmd->is_compl)
    {
        CUR_PART = 0;
        print_err(CODE0_UNSUPPORT_INCLUDE_FILE_CMD, (token *)inf_get_last_token(cmd));
    }
    ++GLOBAL_VAR_DECS_NUMS;
    GLOBAL_VAR_DECS = realloc(GLOBAL_VAR_DECS, sizeof(*GLOBAL_VAR_DECS)*GLOBAL_VAR_DECS_NUMS);

    token *t1 = &cmd->items[0].tok;
    token *t2 = &cmd->items[1].tok;

    glob_ident gi;
    gi.type_class = t1->potentional_types[0].type_class;
    gi.type_num   = t1->potentional_types[0].type_num;
    strncpy(gi.name, t2->potentional_types[0].value, strlen(t2->potentional_types[0].value)+1);
    gi.inf_char = t1->inf_char;
    gi.inf_char_num = t1->inf_char_num;
    gi.inf_line_num = t1->inf_line_num;
    gi.inf_file_num = t1->inf_file_num;

    GLOBAL_VAR_DECS[GLOBAL_VAR_DECS_NUMS-1] = gi;
}


int is_glob_var_dec_exist(char *ident)
{
    int i, len = strlen(ident);
    for(i=0; i<GLOBAL_VAR_DECS_NUMS; ++i)
    {
        if (strlen(GLOBAL_VAR_DECS[i].name)==len && strncmp(GLOBAL_VAR_DECS[i].name, ident, len)==0)
            return 1;
    }
    return 0;
}


void get_glob_var_dec_value_type(char *ident, int *c, int *t)
{
    int i, len = strlen(ident);
    for(i=0; i<GLOBAL_VAR_DECS_NUMS; ++i)
    {
        if (strlen(GLOBAL_VAR_DECS[i].name)==len && strncmp(GLOBAL_VAR_DECS[i].name, ident, len)==0)
        {
            *c = GLOBAL_VAR_DECS[i].type_class;
            *t = GLOBAL_VAR_DECS[i].type_num;
            return;
        }
    }
}


void  work_with_glob_decs()
{
    int j, len;
    for (j=0; j<GLOBAL_VAR_DECS_NUMS; ++j)
    {
        if (!is_glob_var_def_exist(GLOBAL_VAR_DECS[j].name))
        {
            CUR_PART = 7;
            inf_tok.inf_char     = GLOBAL_VAR_DECS[j].inf_char;
            inf_tok.inf_char_num = GLOBAL_VAR_DECS[j].inf_char_num;
            inf_tok.inf_line_num = GLOBAL_VAR_DECS[j].inf_line_num;
            inf_tok.inf_file_num = GLOBAL_VAR_DECS[j].inf_file_num;
            print_err(CODE7_GLOB_VAR_MUST_DEF, &inf_tok);
        }
        /// Yglan edilen we maglumaty yglan edilen global ülňileriň tipleri gabat gelmeli
        glob_ident *gi = glob_vars_def_get_by_name(GLOBAL_VAR_DECS[j].name);
        if (!(gi->type_class==GLOBAL_VAR_DECS[j].type_class && gi->type_num==GLOBAL_VAR_DECS[j].type_num))
        {
            CUR_PART = 7;
            inf_tok.inf_char     = GLOBAL_VAR_DECS[j].inf_char;
            inf_tok.inf_char_num = GLOBAL_VAR_DECS[j].inf_char_num;
            inf_tok.inf_line_num = GLOBAL_VAR_DECS[j].inf_line_num;
            inf_tok.inf_file_num = GLOBAL_VAR_DECS[j].inf_file_num;
            print_err(CODE7_GLOB_VAR_NOT_MATCH_DATA_TYPE, &inf_tok);
        }
    }
}


void cmd_wrapper_c_code(char **l, int *llen)
{
    char *cmd_end = "; \n";


    *llen += strlen(cmd_end);
    *l = realloc(*l, *llen);

    if ((*llen-strlen(cmd_end))==0)
        strncpy(*l,cmd_end,strlen(cmd_end)+1);
    else
        strncat(*l,cmd_end,strlen(cmd_end));
}
