#include <stdlib.h>
#include <string.h>
#include "fn_helpers.h"
#include "../fns.h"
#include "../paren/types.h"
#include "../cmd/fn_def.h"
#include "../cmd/block.h"
#include "../cmd/def_var.h"
#include "../main/glob.h"
#include "../main/user_def_type.h"
#include "../translator_to_c.h"
/** Funksiýalaryň komandalary bilen işleýän kömekçi faňksiýeler.
*/


/** Häzir funksiýa çagyrylýan wagty,
    Parseriň funksiýanyň içini parsing edýänini barlaýar*/
int is_inside_fn()
{
    if (TMP_FUNC_NUM!=-1 && TMP_FUNC_NUM==GLOB_BLOCK_INCLUDES-1)
        return 1;
    return 0;
}


/** Komandanyň funksiýanyň blogynda yglan etseň bolýanlygyny barlaýar*/
int is_unvalid_inside_fn_cmd(command *cmd)
{
    /** Funksiýanyň bloklarynda hiç hili global maglumatlary yglan ýa beýan edip bolanok. */
    if (cmd->ns==GLOB)
        return 1;

    int i;
    for (i=0; i<FN_UNVALIDE_INSIDE_CMDS_NUM; ++i)
    {
        if (cmd->cmd_class==unvalid_fn_inside_cmds[i][0] &&
            cmd->cmd_type ==unvalid_fn_inside_cmds[i][1])
            return 1;
    }
    return 0;
}


/** Funksiýany yglan edýän komandalyga barlaýar*/
int is_fn_def(command *cmd)
{
    if (cmd->cmd_class==CMD_CLASS_FN_DEF && cmd->cmd_type==CMD_FN_DEF_TYPE)
        return 1;
    return 0;
}


/** Wagtlaýyn funksiýanyň maglumatlaryny saklajak sanawa,
    Funksiýanyň baş magumatlary girizilýär*/
void prepare_tmp_fn_data_container(command *cmd)
{
    wchar_t *dquote = L"\"",
            *dot_c  = L".c",
            *dot_h  = L".h";
    TMP_FUNC_NUM=GLOB_BLOCK_INCLUDES-1;

    command_item *fci  = get_cmd_item(cmd->items, 0);
    command_item *args_ci = get_cmd_item(fci->cmd.items, 0);
    command_item *ident_ci = get_cmd_item(fci->cmd.items, 1);
    command_item *ret_val_ci  = get_cmd_item(cmd->items, 1);
    int fnum = ident_ci->tok.inf_file_num;
    wchar_t fname[MAX_FILE_LEN] = {0};
    wcsncpy(fname, FILES[fnum].c_source, wcslen(FILES[fnum].c_source)+1);
    remove_dirnames(fname);
	remove_ext(fname, dot_c);
    wchar_t *fnident = ident_ci->tok.potentional_types[0].value;
    wchar_t fn_lib[MAX_FILE_LEN] = {0};
    wcsncpy(fn_lib, dquote, wcslen(dquote));
    wcsncat(fn_lib, FILES[fnum].name, wcslen(FILES[fnum].name));
    wcsncat(fn_lib, dot_h, wcslen(dot_h));
    wcsncat(fn_lib, dquote, wcslen(dquote));

    wcsncpy(TMP_FUNC.file_name, fname, wcslen(fname)+1);
    wcsncpy(TMP_FUNC.name, fnident, wcslen(fnident)+1);
    TMP_FUNC.type_class = FUNC_CLASS_USER_DEF;
    TMP_FUNC.type_num = LOC_FUNCS_NUM;
    TMP_FUNC.args_num = args_ci->paren.elems_num;     // * ýaly argumentleri kabul edýär
    TMP_FUNC.inf_wchar_t = ident_ci->tok.inf_wchar_t;
    TMP_FUNC.inf_wchar_t_pos = ident_ci->tok.inf_wchar_t_num;
    TMP_FUNC.inf_line_pos = ident_ci->tok.inf_line_num;
    TMP_FUNC.return_class = ret_val_ci->tok.type_class;
    TMP_FUNC.return_type = ret_val_ci->tok.potentional_types[0].type_num;
    wcsncpy(TMP_FUNC.c_name, fnident, wcslen(fnident)+1);
    wcsncpy(TMP_FUNC.c_lib, fn_lib, wcslen(fn_lib)+1);
    TMP_FUNC.make_args_string = make_user_def_fn_args;

    // Argumentler
    if (TMP_FUNC.args_num)
    {
        parenthesis_elem *pe = get_paren_elems(args_ci->paren.elems);
        TMP_FUNC_PARAMS = realloc(TMP_FUNC_PARAMS, sizeof(*TMP_FUNC_PARAMS)*TMP_FUNC.args_num);
        int i;
        for (i=0; i<TMP_FUNC.args_num; ++i)
        {
            TMP_FUNC_PARAMS[i] = pe[i].cmd;
        }
    }
}


/** Funksiýa beýan etmelerine täzesini goşýar*/
void add_fn_dec(command *cmd)
{
    wchar_t *dquote = L"\"",
            *dot_c  = L".c",
            *dot_h  = L".h";

    command_item *fci  = get_cmd_item(cmd->items, 0);
    command_item *args_ci = get_cmd_item(fci->cmd.items, 0);
    command_item *ident_ci = get_cmd_item(fci->cmd.items, 1);
    command_item *ret_val_ci  = get_cmd_item(cmd->items, 1);
    if (is_fn_already_dec(ident_ci->tok.potentional_types[0].value))
    {
        return;
    }

    int last = DEC_FUNCS_NUM;
    ++DEC_FUNCS_NUM;
    DEC_FUNCS     = realloc(DEC_FUNCS, sizeof(*DEC_FUNCS)*DEC_FUNCS_NUM);
    DEC_FUNC_ARGS = realloc(DEC_FUNC_ARGS, sizeof(*DEC_FUNC_ARGS)*DEC_FUNCS_NUM);


    int fnum = ident_ci->tok.inf_file_num;
    wchar_t fname[MAX_FILE_LEN] = {0};
    wcsncpy(fname, FILES[fnum].c_source, wcslen(FILES[fnum].c_source)+1);
    remove_dirnames(fname);
	remove_ext(fname, dot_c);
    wchar_t *fnident = ident_ci->tok.potentional_types[0].value;
    wchar_t fn_lib[MAX_FILE_LEN] = {0};
    wcsncpy(fn_lib, dquote, wcslen(dquote));
    wcsncat(fn_lib, FILES[fnum].name, wcslen(FILES[fnum].name));
    wcsncat(fn_lib, dot_h, wcslen(dot_h));
    wcsncat(fn_lib, dquote, wcslen(dquote));

    wcsncpy(DEC_FUNCS[last].file_name, fname, wcslen(fname)+1);
    wcsncpy(DEC_FUNCS[last].name, fnident, wcslen(fnident)+1);
    DEC_FUNCS[last].type_class = FUNC_CLASS_USER_DEF;
    DEC_FUNCS[last].type_num = -1;
    DEC_FUNCS[last].args_num = args_ci->paren.elems_num;     // * ýaly argumentleri kabul edýär
    DEC_FUNCS[last].inf_wchar_t = ident_ci->tok.inf_wchar_t;
    DEC_FUNCS[last].inf_wchar_t_pos = ident_ci->tok.inf_wchar_t_num;
    DEC_FUNCS[last].inf_line_pos = ident_ci->tok.inf_line_num;
    DEC_FUNCS[last].file_num     = ident_ci->tok.inf_file_num;
    DEC_FUNCS[last].return_class = ret_val_ci->tok.type_class;
    DEC_FUNCS[last].return_type = ret_val_ci->tok.potentional_types[0].type_num;
    wcsncpy(DEC_FUNCS[last].c_name, fname, wcslen(fname)+1);
    wcsncpy(DEC_FUNCS[last].c_lib, fn_lib, wcslen(fn_lib)+1);
    DEC_FUNCS[last].make_args_string = make_user_def_fn_args;

    // Argumentler
    if (DEC_FUNCS[last].args_num)
    {
        parenthesis_elem *pe = get_paren_elems(args_ci->paren.elems);
        DEC_FUNC_ARGS[last] = realloc(DEC_FUNC_ARGS[last], sizeof(**DEC_FUNC_ARGS)*DEC_FUNCS[last].args_num);
        int i;
        for (i=0; i<DEC_FUNCS[last].args_num; ++i)
        {
            command_item *type_item;
            if (pe[i].cmd.cmd_class==CMD_CLASS_DEF_VAR)
            {
                type_item = get_cmd_item(pe[i].cmd.items, 0);
            }
            else if (pe[i].cmd.cmd_class==CMD_CLASS_ARR)
            {
                type_item = get_cmd_item(pe[i].cmd.items, 1);
            }
            func_arg fa;
            fa.type_class = type_item->tok.potentional_types[0].type_class;
            fa.type_num   = type_item->tok.potentional_types[0].type_num;
            DEC_FUNC_ARGS[last][i] = fa;
        }
    }
}


/** Identifikatoryň eýýäm beýan edilen funksiýanyň adymydygyny barlaýar*/
int is_fn_already_dec(wchar_t *ident)
{
    int i, len = wcslen(ident);
    for (i=0; i<DEC_FUNCS_NUM; ++i)
    {
        if (wcslen(DEC_FUNCS[i].name)==len &&
            wcsncmp(DEC_FUNCS[i].name, ident, len)==0)
            return 1;
    }
    return 0;
}

/** Funksiýany gutarýan komandalyga barlaýar*/
int is_fn_close_cmd(command *cmd)
{
    if (cmd->cmd_class==CMD_CLASS_BLOCK && cmd->cmd_type==CMD_CLASS_BLOCK_CLOSE &&
        TMP_FUNC_NUM==GLOB_BLOCK_INCLUDES)
        return 1;
    return 0;
}

/** Wagtlaýyn yglan edilen maglumatlary  */
void move_tmp_fn_data_to_local()
{
    int last = LOC_FUNCS_NUM, i;
    ++LOC_FUNCS_NUM;
    LOC_FUNCS = realloc(LOC_FUNCS, sizeof(*LOC_FUNCS)*LOC_FUNCS_NUM);
    LOC_FUNCS_ALGOR = realloc(LOC_FUNCS_ALGOR, sizeof(*LOC_FUNCS_ALGOR)*LOC_FUNCS_NUM);
    LOC_FUNCS_ALGOR_NUM = realloc(LOC_FUNCS_ALGOR_NUM, sizeof(*LOC_FUNCS_ALGOR_NUM)*LOC_FUNCS_NUM);
    LOC_FUNCS_ARRS = realloc(LOC_FUNCS_ARRS, sizeof(*LOC_FUNCS_ARRS)*LOC_FUNCS_NUM);
    LOC_FUNCS_ARRS_ITEMS = realloc(LOC_FUNCS_ARRS_ITEMS, sizeof(*LOC_FUNCS_ARRS_ITEMS)*LOC_FUNCS_NUM);
    LOC_FUNCS_ARRS_NUM = realloc(LOC_FUNCS_ARRS_NUM, sizeof(*LOC_FUNCS_ARRS_NUM)*LOC_FUNCS_NUM);
    LOC_FUNCS_VARS = realloc(LOC_FUNCS_VARS, sizeof(*LOC_FUNCS_VARS)*LOC_FUNCS_NUM);
    LOC_FUNCS_VARS_NUM = realloc(LOC_FUNCS_VARS_NUM, sizeof(*LOC_FUNCS_VARS_NUM)*LOC_FUNCS_NUM);
    LOC_FUNCS_PARAMS = realloc(LOC_FUNCS_PARAMS, sizeof(*LOC_FUNCS_PARAMS)*LOC_FUNCS_NUM);

    LOC_FUNCS[last]             = TMP_FUNC;

    LOC_FUNCS_ALGOR_NUM[last]   = TMP_FUNC_ALGOR_NUM;
    LOC_FUNCS_ALGOR[last]       = NULL;

    LOC_FUNCS_ARRS_NUM[last]    = TMP_FUNC_ARRS_NUM;
    LOC_FUNCS_ARRS[last]        = NULL;
    LOC_FUNCS_ARRS_ITEMS[last]  = NULL;

    LOC_FUNCS_VARS_NUM[last]    = TMP_FUNC_VARS_NUM;
    LOC_FUNCS_VARS[last]      = NULL;

    LOC_FUNCS_PARAMS[last]    = NULL;


    if (LOC_FUNCS[last].args_num)
    {
        LOC_FUNCS_PARAMS[last] = realloc(LOC_FUNCS_PARAMS[last], sizeof(**LOC_FUNCS_PARAMS)*LOC_FUNCS[last].args_num);
        for (i=0; i<LOC_FUNCS[last].args_num; ++i)
        {
            LOC_FUNCS_PARAMS[last][i] = TMP_FUNC_PARAMS[i];
        }
    }

    if (LOC_FUNCS_ALGOR_NUM[last])
    {
        LOC_FUNCS_ALGOR[last] = realloc(LOC_FUNCS_ALGOR[last], sizeof(**LOC_FUNCS_ALGOR)*LOC_FUNCS_ALGOR_NUM[last]);
        for (i=0;i<LOC_FUNCS_ALGOR_NUM[last]; ++i)
        {
            LOC_FUNCS_ALGOR[last][i] = TMP_FUNC_ALGOR[i];
        }
    }
    if(LOC_FUNCS_VARS_NUM[last])
    {
        LOC_FUNCS_VARS[last] = realloc(LOC_FUNCS_VARS[last], sizeof(*LOC_FUNCS_VARS[last])*LOC_FUNCS_VARS_NUM[last]);
        for (i=0; i<LOC_FUNCS_VARS_NUM[last]; ++i)
        {
            LOC_FUNCS_VARS[last][i] = TMP_FUNC_VARS[i];
        }
    }
    if(LOC_FUNCS_ARRS_NUM[last])
    {
        int loc_arrs_num = LOC_FUNCS_ARRS_NUM[last];
        LOC_FUNCS_ARRS[last]       = malloc(sizeof(**LOC_FUNCS_ARRS)*loc_arrs_num);
        LOC_FUNCS_ARRS_ITEMS[last] = malloc(sizeof(*LOC_FUNCS_ARRS_ITEMS)*loc_arrs_num);
        for (i=0; i<loc_arrs_num; ++i)
        {
            LOC_FUNCS_ARRS[last][i] = TMP_FUNC_ARRS[i];
            LOC_FUNCS_ARRS_ITEMS[last][i] = NULL;
            int loc_arr_incs_num = LOC_FUNCS_ARRS[last][i].incs;    /// Näçe sany sanawyň içkiligi bar
            /// Ülňiniň içkiliginiň maglumatlary
            if (loc_arr_incs_num)
            {

                LOC_FUNCS_ARRS_ITEMS[last][i] = malloc(sizeof(***LOC_FUNCS_ARRS_ITEMS)*loc_arr_incs_num);
                int j;
                for (j=0; j<loc_arr_incs_num; ++j)
                {
                    LOC_FUNCS_ARRS_ITEMS[last][i][j] = TMP_FUNC_ARRS_ITEMS[i][j];
                }
            }
        }
    }
    if(LOC_FUNCS_VARS_NUM[last])
    {
        LOC_FUNCS_VARS[last] = realloc(LOC_FUNCS_VARS[last], sizeof(*LOC_FUNCS_VARS[last])*LOC_FUNCS_VARS_NUM[last]);
        for (i=0; i<LOC_FUNCS_VARS_NUM[last]; ++i)
        {
            LOC_FUNCS_VARS[last][i] = TMP_FUNC_VARS[i];
        }
    }

    free_tmp_fn();

    add_to_func_list();
}

/** Eger komanda ulny yglan etmek bolsa, ulni yglan etmanin sanawynda goshyar.*/
int add_to_tmp_fn_def_var_list(command *cmd)
{
	if (!is_def_var_cmd(cmd))
	{
        // Ulni yglan etme komanda dal
        return 0;
	}
    add_to_tmp_fn_var_def(cmd);
    return 1;
}

/** Eger komanda ulny yglan etmek bolsa, ulni yglan etmanin sanawynda goshyar.*/
int add_to_tmp_fn_def_arr_list(command *cmd)
{
	if (!is_def_arr_cmd(cmd))
	{
        // Ulni yglan etme komanda dal
        return 0;
	}
    add_to_tmp_fn_arr_def(cmd);
    return 1;
}

/** Eger komanda ulny yglan etmek bolsa, ulni yglan etmanin sanawynda goshyar.*/
int add_to_tmp_fn_algor_list(command *cmd)
{
    add_to_tmp_fn_algor(cmd);
    return 1;
}


/** Funksiýanyň wagtlaýyn ülňilerine, täze ülňi yglan etmäni goşýar*/
void add_to_tmp_fn_var_def(command *cmd)
{
    check_semantics(cmd);

    int last = TMP_FUNC_VARS_NUM;
    ++TMP_FUNC_VARS_NUM;
    TMP_FUNC_VARS = realloc(TMP_FUNC_VARS, sizeof(*TMP_FUNC_VARS)*TMP_FUNC_VARS_NUM);

    int ident_tok_pos = 1;  //  LOKAL: def_type, ident

    command_item *ci = get_cmd_item(cmd->items, ident_tok_pos);
    wchar_t *tok_name = ci->tok.potentional_types[0].value;

	// Identifikator eyyam yglan edilen eken.
	command_item *prevci = get_cmd_item(cmd->items, ident_tok_pos-1);
	glob_ident new_def = {
	    prevci->tok.potentional_types[0].type_class,
		prevci->tok.potentional_types[0].type_num,
		L"",
		prevci->tok.inf_file_num,
        prevci->tok.inf_wchar_t_num,
		prevci->tok.inf_wchar_t,
		prevci->tok.inf_line_num
	};

	wcsncpy(new_def.name, tok_name, wcslen(tok_name)+1);

    TMP_FUNC_VARS[last] = new_def;
}


/** Funksiýanyň wagtlaýyn algoritmini saklaýan maglumata ýene bir komanda goşulýar*/
void add_to_tmp_fn_algor(command *cmd)
{
    check_semantics(cmd);
    int i;
    int last = TMP_FUNC_ALGOR_NUM;
    ++TMP_FUNC_ALGOR_NUM;
    TMP_FUNC_ALGOR = realloc(TMP_FUNC_ALGOR, sizeof(*TMP_FUNC_ALGOR)*TMP_FUNC_ALGOR_NUM);
    command new_cmd;
    make_cmd_copy(cmd, &new_cmd);
    TMP_FUNC_ALGOR[last] = new_cmd;
}


/** Funksiýanyň wagtlaýyn algoritmini saklaýan maglumata ýene bir komanda goşulýar*/
void add_to_tmp_fn_arr_def(command *cmd)
{
    check_semantics(cmd);
    int last = TMP_FUNC_ARRS_NUM;

    ++TMP_FUNC_ARRS_NUM;
    TMP_FUNC_ARRS = realloc(TMP_FUNC_ARRS, sizeof(*TMP_FUNC_ARRS)*TMP_FUNC_ARRS_NUM);
    TMP_FUNC_ARRS_ITEMS = realloc(TMP_FUNC_ARRS_ITEMS, sizeof(*TMP_FUNC_ARRS_ITEMS)*TMP_FUNC_ARRS_NUM);
    TMP_FUNC_ARRS_ITEMS[TMP_FUNC_ARRS_NUM-1] = NULL;

    int ident_tok_pos = cmd->items_num-1;  // Identifikator - bu komandadaky iň soňky element

    command_item *ci = get_cmd_item(cmd->items, ident_tok_pos);
    wchar_t *tok_name = ci->tok.potentional_types[0].value;

	command_item *prevci = get_cmd_item(cmd->items, ident_tok_pos-1);
    command_item *yay_ci = get_cmd_item(cmd->items, ident_tok_pos-2);

    array_item ai = {
        prevci->tok.inf_file_num,
        prevci->tok.inf_wchar_t_num,
        prevci->tok.inf_wchar_t,
        prevci->tok.inf_line_num,
        L"",
        yay_ci->paren.elems_num,
        prevci->tok.potentional_types[0].type_class,
		prevci->tok.potentional_types[0].type_num
    };

	wcsncpy(ai.ident, tok_name, wcslen(tok_name)+1);

    add_to_last_tmp_func_arr_items(cmd);

    TMP_FUNC_ARRS[last] = ai;
}


/** Funksiýanyň wagtlaýyn maglumatlarynda eýýäm ülňi ulanylanlygy barlanylýar*/
int is_tmp_fn_ident_used(wchar_t *ident)
{
    int i, len = wcslen(ident);
    for(i=0; i<TMP_FUNC_VARS_NUM; ++i)
    {
        wchar_t *ident_in_tmp = TMP_FUNC_VARS[i].name;
        if (wcslen(ident_in_tmp)==len &&
            wcsncmp(ident_in_tmp, ident, len)==0)
            return 1;
    }
    for(i=0; i<TMP_FUNC_ARRS_NUM; ++i)
    {
        wchar_t *ident_in_tmp = TMP_FUNC_ARRS[i].ident;
        if (wcslen(ident_in_tmp)==len &&
            wcsncmp(ident_in_tmp, ident, len)==0)
            return 1;
    }
    for (i=0; i<TMP_FUNC.args_num; ++i)
    {
        command_item *ident_item = get_cmd_item(TMP_FUNC_PARAMS[i].items, 1);
        wchar_t *ident_in_tmp = ident_item->tok.potentional_types[0].value;
        if (wcslen(ident_in_tmp)==len &&
            wcsncmp(ident_in_tmp, ident, len)==0)
            return 1;
    }
    return 0;
}


/** Funksiýa-da yglan edilen ülňi ady däldigi barlanýar*/
int is_tmp_fn_var_ident_used(wchar_t *ident)
{
    int i, len = wcslen(ident);
    for(i=0; i<TMP_FUNC_VARS_NUM; ++i)
    {
        wchar_t *ident_in_tmp = TMP_FUNC_VARS[i].name;
        if (wcslen(ident_in_tmp)==len &&
            wcsncmp(ident_in_tmp, ident, len)==0)
            return 1;
    }
    return 0;
}

/** Funksiýa-da yglan edilen ülňi ady däldigi barlanýar*/
int is_tmp_fn_arr_ident_used(wchar_t *ident)
{
    int i, len = wcslen(ident);
    for(i=0; i<TMP_FUNC_ARRS_NUM; ++i)
    {
        wchar_t *ident_in_tmp = TMP_FUNC_ARRS[i].ident;
        if (wcslen(ident_in_tmp)==len &&
            wcsncmp(ident_in_tmp, ident, len)==0)
            return 1;
    }
    return 0;
}


/** Sanawyň ady boýunça, sanawyň nomerini alýar.*/
int get_tmp_func_arr_address_by_ident(wchar_t *ident, int *type)
{
    int i, len = wcslen(ident);
    for(i=0; i<TMP_FUNC_ARRS_NUM; ++i)
    {
        wchar_t *ident_in_tmp = TMP_FUNC_ARRS[i].ident;
        if (wcslen(ident_in_tmp)==len &&
            wcsncmp(ident_in_tmp, ident, len)==0)
        {
            *type = i;
            return 1;
        }
    }
    return 0;
}


/** Ülňiniň tipini gaýtarýar*/
int get_tmp_fn_var_def_value_type(wchar_t *ident, int *type_class, int *type_num)
{
    int i, len = wcslen(ident);
    for(i=0; i<TMP_FUNC_VARS_NUM; ++i)
    {
        wchar_t *ident_in_tmp = TMP_FUNC_VARS[i].name;
        if (wcslen(ident_in_tmp)==len &&
            wcsncmp(ident_in_tmp, ident, len)==0)
        {
            *type_class = TMP_FUNC_VARS[i].type_class;
            *type_num   = TMP_FUNC_VARS[i].type_num;
            set_def_type_alias_const_data(type_class, type_num);
            return 1;
        }
    }

    return 0;
}


/** Yglanlanan ýa beýanlanan funksiýalaryň identifikatory ulandymy ýa ýokdygynyň jogabyny aýdýar.*/
int is_fn_name_used(wchar_t *ident)
{
    int i, len = wcslen(ident);

    // Yglanlanan funksiýalaryň arasynda gözlenýär
    for (i=0; i<FUNCS_NUM; ++i)
    {
        if (wcslen(FUNCS[i].name)==len &&
            wcsncmp(FUNCS[i].name, ident, len)==0)
            return 1;
    }

    return 0;
}


/** Global ülňileriň baş maglumatlaryndan, ady boýunça sanawdaky funksiýanyň nomeri gaýtarylýar*/
int get_fn_by_ident(wchar_t *ident)
{
    int i, len = wcslen(ident);

    // Yglanlanan funksiýalaryň arasynda gözlenýär
    for (i=0; i<FUNCS_NUM; ++i)
    {
        if (wcslen(FUNCS[i].name)==len &&
            wcsncmp(FUNCS[i].name, ident, len)==0)
            return i;
    }

    return 0;
}


/** Global funksiýalaryň hataryna, mümkin bolar ýaly maglumatlary goşýar*/
void add_to_func_list()
{
    add_func_to_list(LOC_FUNCS[LOC_FUNCS_NUM-1]);

    // Argumentler
    int i;
    for (i=0; i<LOC_FUNCS[LOC_FUNCS_NUM-1].args_num; ++i)
    {
        command_item *type_ci;
        command *cmd = &LOC_FUNCS_PARAMS[LOC_FUNCS_NUM-1][i];
        if (cmd->cmd_class==CMD_CLASS_ARR)
        {
            type_ci = get_cmd_item(cmd->items, 1);
        }
        else if (cmd->cmd_class==CMD_CLASS_DEF_VAR)
        {
            type_ci = get_cmd_item(cmd->items, 0);
        }
        func_arg fargs;
        fargs.type_class = type_ci->tok.type_class;
        fargs.type_num   = type_ci->tok.potentional_types[0].type_num;
        add_arg_to_last_func(fargs, i);
    }
}

/** Wagtlaýyn funksiýanyň içindäki sanawlaryň soňkusyna goşýar*/
void add_to_last_tmp_func_arr_items(command *cmd)
{
    int yay_pos = 0;

    command_item *ci = get_cmd_item(cmd->items, yay_pos);
    parenthesis *p = &ci->paren;

    int i;

    TMP_FUNC_ARRS_ITEMS[TMP_FUNC_ARRS_NUM-1] = realloc(TMP_FUNC_ARRS_ITEMS[TMP_FUNC_ARRS_NUM-1], sizeof(**TMP_FUNC_ARRS_ITEMS)*p->elems_num);
    array_inc_item **last = &TMP_FUNC_ARRS_ITEMS[TMP_FUNC_ARRS_NUM-1];

    parenthesis_elem *pes = get_paren_elems(p->elems);

    for(i=0; i<p->elems_num; ++i)
    {
        array_inc_item add = {TMP_FUNC_ARRS_NUM-1, i, wcstol(pes[i].tok.potentional_types[0].value, NULL, 10)};

        (*last)[i] = add;
    }
}


/** TRANSLATOR TO C*/
/** Lokal yglan edilen funksiýalaryň .h faýlda ýazylmaly prototipleri*/
void write_to_hsource_loc_fns_proto(FILE *f)
{
    if (!LOC_FUNCS_NUM)
    {
        return;
    }
    int i;
    for (i=0; i<LOC_FUNCS_NUM; ++i)
    {
        wchar_t *p = NULL;
        int len = 0;
        get_loc_fn_prototype(i, &p, &len);

        wchar_t *end = L"; \n";
        wcsadd_on_heap( &p, &len, end );

        fputws(p, f);
        free(p);
    }
}

/** Lokal funksiýanyň nomeri bovunca prototipini ýasaýar*/
void get_loc_fn_prototype(int n, wchar_t **p, int *l)
{
    if (LOC_FUNCS[n].return_class==TOK_CLASS_VOID)
    {
        wcsadd_on_heap( p, l, TOK_VOID_CHARS[TOK_VOID_NUM][2] );
    }
    else if (LOC_FUNCS[n].return_class==TOK_CLASS_DEF_TYPE)
    {
        wcsadd_on_heap( p, l, def_type_list[LOC_FUNCS[n].return_type].value );
    }
    else if (LOC_FUNCS[n].return_class==TOK_CLASS_UTYPE)
    {
        wcsadd_on_heap( p, l, USER_DEF_TYPES[LOC_FUNCS[n].return_type].ident );
    }

    wchar_t *space = L" ";
    wcsadd_on_heap( p, l, space );

    wcsadd_on_heap( p, l, LOC_FUNCS[n].name );

    write_fn_args(n, p, l);

}

/** Funksiýany yglan edilmeginiň argumentleriniň kodly bölegi*/
void write_fn_args(int n, wchar_t **p, int *l)
{
    wchar_t *o = L"(";
    wcsadd_on_heap( p, l, o );

    int i;
    for (i=0; i<LOC_FUNCS[n].args_num; ++i)
    {
        /// Wagtlaýyn ülňi 100-lige goýulsa, bu ülňiniň näme üçin ulanylýanyny
        /// başga hiç zat bilen çalşyp bolmaz.
        ///
        /// Ülňi ulanylýan bolsa, onda ülňi yglan etmegi dolylygyna tipi bilen ýaly ýazýar
        TEST = 100;
        cmd_get_c_code( &LOC_FUNCS_PARAMS[n][i], p, l );
        TEST = 0;


        if (i!=LOC_FUNCS[n].args_num-1)
        {
            wchar_t *comma_space = L", ";
            wcsadd_on_heap( p, l, comma_space );
        }
    }

    wchar_t *c = L")";
    wcsadd_on_heap( p, l, c );
}

/** Funksiýanyň bedeniniň kodyny ýazýar*/
void write_to_csource_loc_fns(FILE *f)
{
    if (!LOC_FUNCS_NUM)
    {
        return;
    }
    int i;
    for (i=0; i<LOC_FUNCS_NUM; ++i)
    {
        wchar_t *p = NULL;
        int len = 0;
        get_loc_fn_prototype(i, &p, &len);
        wchar_t *end = L"{ \n";
        wcsadd_on_heap( &p, &len, end );

        fputws(p, f);
        free(p);

        ++TRANS_C_BLOCK_DEPTH;

        c_trans_source_add_fn_def_var(f, i);
        c_trans_source_add_fn_def_arr(f, i);
        c_trans_source_fn_add_algor(f, i);

        --TRANS_C_BLOCK_DEPTH;
        fputws(L"} \n", f);
    }
}

/** Funksiýanyň içinde yglan edilen ülňiler*/
void c_trans_source_add_fn_def_var(FILE *f, int fn)
{
    wchar_t *l = NULL,
            *space = L" ";
    int llen = 0;

	if (!LOC_FUNCS_VARS_NUM[fn])
	{
        return;
	}

    wchar_t *line1 = L"// Dine yglan edilen funksiyanyn ichinde ulanyp bolyan ulniler\n";
    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line1);

    int i, type_num;
    long line_len;

    for(i=0; i<LOC_FUNCS_VARS_NUM[fn]; ++i)
    {
        int  line_len = 0;
        wchar_t *line = NULL;
        get_type_c_code(LOC_FUNCS_VARS[fn][i].type_class, LOC_FUNCS_VARS[fn][i].type_num, &line, &line_len);

        wcsadd_on_heap( &line, &line_len, space );

        // Ulninin ady goshulyar
        wcsadd_on_heap( &line, &line_len, LOC_FUNCS_VARS[fn][i].name );

        // Ülňiniň başlangyç maglumatlary goýulýar.
        get_type_init_val_c_code(0, LOC_FUNCS_VARS[fn][i].type_class, LOC_FUNCS_VARS[fn][i].type_num, &line, &line_len);

        // Komanda gutardy
        get_cmd_end_c_code(&line, &line_len);

        write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line);

        free(line);
    }

    fputws(L"\n\n", f);
}

/** Funksiýanyň içinde yglan edilen sanawlary goşýar*/
void c_trans_source_add_fn_def_arr(FILE *f, int fn)
{
    wchar_t *l = NULL,
            *space = L" ";
    int llen = 0;

	if (!LOC_FUNCS_ARRS_NUM[fn])
	{
        return ;
	}

    wchar_t *line1 = L"// Dine yglan edilen funksiyanyn ichinde ulanyp bolyan sanawlar\n";
    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line1);

    int i, type_num, line_len;

    for(i=0; i<LOC_FUNCS_ARRS_NUM[fn]; ++i)
    {
        type_num = LOC_FUNCS_ARRS[fn][i].type_num;

        int line_len = 0;
        wchar_t *line = NULL;
        get_type_c_code(LOC_FUNCS_ARRS[fn][i].type_class, LOC_FUNCS_ARRS[fn][i].type_num, &line, &line_len);

        wcsadd_on_heap( &line, &line_len, space );

        // Ulninin ady goshulyar
        wcsadd_on_heap( &line, &line_len, LOC_FUNCS_ARRS[fn][i].ident );

        // sanawyň elelemntleri hakda maglumat goşulýar
        TEST = fn;
        add_arr_elem_inf_c_code(&line, &line_len, 4, i, LOC_FUNCS_ARRS[fn][i].incs);

        // Ülňilere başlangyç maglumatlar baglamaly
        get_type_init_val_c_code(1, LOC_FUNCS_ARRS[fn][i].type_class, LOC_FUNCS_ARRS[fn][i].type_num, &line, &line_len);

        // Komanda gutardy
        get_cmd_end_c_code(&line, &line_len);

        write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line);

        free(line);
    }

	fputws(L"\n\n", f);
}

/** Funksiýa yglan edilenmeginiň bedenindäki algoritmler*/
void c_trans_source_fn_add_algor(FILE *f, int fn)
{
    int i, len=0; wchar_t *l = NULL;
    wchar_t *putl = NULL;
    int putllen = 0;

    for (i=0; i<LOC_FUNCS_ALGOR_NUM[fn]; ++i)
    {
        cmd_get_c_code( &LOC_FUNCS_ALGOR[fn][i], &l, &len );
        if (!is_open_block_cmd(&LOC_FUNCS_ALGOR[fn][i]) && !is_close_block_cmd(&LOC_FUNCS_ALGOR[fn][i]))
            cmd_wrapper_c_code(&l, &len);
        else
            cmd_block_wrapper_c_code(&l, &len);

        /// #7.5)
        if (is_close_block_cmd(&LOC_FUNCS_ALGOR[fn][i]))
            --TRANS_C_BLOCK_DEPTH;

        write_code_line(f, &putl, &putllen, TRANS_C_BLOCK_DEPTH, l);

        /// #7.4)
        if (is_open_block_cmd(&LOC_FUNCS_ALGOR[fn][i]))
            ++TRANS_C_BLOCK_DEPTH;

        if (l!=NULL)
        {
            free(l);
            l = NULL;
            len = 0;
        }
    }
    fputws(L"\n\n", f);
}


