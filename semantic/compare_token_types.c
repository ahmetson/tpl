/***
 * Maglumatlaryň token tipleri barlanýar
***/
#include <stdio.h>
#include <string.h>

#include "../main/glob.h"
#include "../main/inf.h"
#include "compare_token_types.h"
#include "../cmd/def_var.h"
#include "../error.h"
#include "../dev_debug.h"


// Sag tarapy konstanta maglumat we çep tarapy komanda birligi bolan
// komandalaryň bir token tipdedigi barlanýar
void compare_token_types_right_data()
{
    int i, left_class, left_type, right_class, right_type;
    for(i=0; i<GLOB_RIGHT_DATA_CMDS_NUM; ++i)
    {
        // Çep tarap
        if(GLOB_RIGHT_DATA_CMDS_LIST[i].left.type==TOKEN_ITEM)
        {
            token *t = &GLOB_RIGHT_DATA_CMDS_LIST[i].left.tok;
            token_type *p = &t->potentional_types[0];
            //debug_token(&GLOB_RIGHT_DATA_CMDS_LIST[i].left.tok);
            TOK_RETURN_TYPE[p->type_class][p->type_num](t, &left_class, &left_type);
            set_def_type_alias_const_data(&left_class, &left_type);
        }
        else
        {
            command *c = &GLOB_RIGHT_DATA_CMDS_LIST[i].left.cmd;
            CMD_RETURN_TYPE[c->cmd_class][c->cmd_type](c, &left_class, &left_type);
            //printf("SHUL LEFT: %d %d %d\n", c->cmd_type, left_class, left_type);
            set_def_type_alias_const_data(&left_class, &left_type);
        }

        // Sag tarap
        right_class = GLOB_RIGHT_DATA_CMDS_LIST[i].right.type_class;
        right_type  = GLOB_RIGHT_DATA_CMDS_LIST[i].right.type_num;
        //printf("SHUL RIGHT: %d %d\n", right_class, right_type);

        // Deňeşdirme
        if ( !(right_class==left_class && right_type==left_type) )
        {
            CUR_PART = 7;
            if (GLOB_RIGHT_DATA_CMDS_LIST[i].left.type==CMD_ITEM)
            {
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, (token*)inf_get_last_token(&GLOB_RIGHT_DATA_CMDS_LIST[i].left.cmd));
            }
            else
            {
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, &GLOB_RIGHT_DATA_CMDS_LIST[i].left.tok);
            }
        }
    }
}


// Çep tarapy konstanta maglumat we sag tarapy komanda birligi bolan
// komandalaryň bir token tipdedigi barlanýar
/*void compare_token_types_left_data()
{
    int i, left_class, left_type, right_class, right_type;
    for(i=0; i<GLOB_RIGHT_DATA_CMDS_NUM; ++i)
    {
        // Çep tarap
        if(GLOB_RIGHT_DATA_CMDS_LIST[i].left.type==TOKEN_ITEM)
        {
            //debug_token(&GLOB_RIGHT_DATA_CMDS_LIST[i].left.tok);
            get_user_var_def_value_type(GLOB_RIGHT_DATA_CMDS_LIST[i].left.tok.potentional_types[0].value, &left_class, &left_type,
                                GLOB_RIGHT_DATA_CMDS_LIST[i].inf_file_name);
            set_def_type_alias_const_data(&left_class, &left_type);
        }
        else
        {
            get_cmd_value_type(&GLOB_RIGHT_DATA_CMDS_LIST[i].left.cmd, &left_class, &left_type);
            set_def_type_alias_const_data(&left_class, &left_type);
        }

        // Sag tarap
        //debug_token_type(&GLOB_RIGHT_DATA_CMDS_LIST[i].right);
        right_class = GLOB_RIGHT_DATA_CMDS_LIST[i].right.type_class;
        right_type  = GLOB_RIGHT_DATA_CMDS_LIST[i].right.type_num;
//printf("%d %d, %d %d", left_class, left_type, right_class, right_type);
        // Deňeşdirme
        if ( !(right_class==left_class && right_type==left_type) )
        {
            CUR_PART = 7;
            if (GLOB_RIGHT_DATA_CMDS_LIST[i].left.type==CMD_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, inf_get_last_token(&GLOB_RIGHT_DATA_CMDS_LIST[i].cmd));
            else
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, &GLOB_RIGHT_DATA_CMDS_LIST[i].tok);
        }
    }
}*/


// Iki tarapy hem komanda birligi bolan
// komandalaryň birlikleriniň bir token tipdedigi barlanýar
void compare_token_types_both_ident()
{
    int i, left_class, left_type, right_class, right_type;
    for(i=0; i<GLOB_BOTH_IDENT_CMDS_NUM; ++i)
    {
        // Çep tarap
        if(GLOB_BOTH_IDENT_CMDS_LIST[i].left.type==TOKEN_ITEM)
        {
            //
            token *t = &GLOB_BOTH_IDENT_CMDS_LIST[i].left.tok;
            token_type *p = &t->potentional_types[0];
            TOK_RETURN_TYPE[p->type_class][p->type_num](t, &left_class, &left_type);
            set_def_type_alias_const_data(&left_class, &left_type);
        }
        else
        {
            command *c = &GLOB_BOTH_IDENT_CMDS_LIST[i].left.cmd;
            //debug_cmd(c);
            CMD_RETURN_TYPE[c->cmd_class][c->cmd_type](c, &left_class, &left_type);
                    //printf("SHUL LEFT: %d: %d %d\n", c->cmd_type, left_class, left_type);

            set_def_type_alias_const_data(&left_class, &left_type);
        }

        // Sag tarap
        if(GLOB_BOTH_IDENT_CMDS_LIST[i].right.type==TOKEN_ITEM)
        {
            token *t = &GLOB_BOTH_IDENT_CMDS_LIST[i].right.tok;
            token_type *p = &t->potentional_types[0];
            TOK_RETURN_TYPE[p->type_class][p->type_num](t, &right_class, &right_type);
            //printf("SHUL RIGHT: %d->%d %d %d\n", p->type_class, p->type_num, right_class, right_type);
            set_def_type_alias_const_data(&right_class, &right_type);
        }
        else
        {
            command *c = &GLOB_BOTH_IDENT_CMDS_LIST[i].right.cmd;
            CMD_RETURN_TYPE[c->cmd_class][c->cmd_type](c, &right_class, &right_type);
            set_def_type_alias_const_data(&right_class, &right_type);
        }

        // Deňeşdirme
        if ( !(right_class==left_class && right_type==left_type) )
        {
            CUR_PART = 7;
            if (GLOB_BOTH_IDENT_CMDS_LIST[i].left.type==CMD_ITEM)
            {
                //debug_cmd(&GLOB_BOTH_IDENT_CMDS_LIST[i].left.cmd);
                print_err(CODE7_TYPES_NOT_MATCH_BOTH_IDENT, (token *)inf_get_last_token(&GLOB_BOTH_IDENT_CMDS_LIST[i].left.cmd));
            }
            else
            {
                print_err(CODE7_TYPES_NOT_MATCH_BOTH_IDENT, (token *)&GLOB_BOTH_IDENT_CMDS_LIST[i].left.tok);
            }
        }
    }
}


int get_user_var_def_value_type(char *ident, int *type_class, int *type_num, char *file_name)
{
    int i, len, flen;
    for (i=0; i<USER_VAR_DEFS_NUM; ++i)
    {
        //printf("%d. %s %s, %s %s\n", USER_VAR_DEFS_NUM, ident, file_name, USER_VAR_DEFS[i].ident, USER_VAR_DEFS[i].file_name);
        // Lokal ülňiler üçin, funksiýanyň ady gabat gelmeli.
        if (USER_VAR_DEFS[i].ns==LOCAL)
        {
            flen = strlen(file_name)>strlen(USER_VAR_DEFS[i].file_name)?
                strlen(file_name):
                strlen(USER_VAR_DEFS[i].file_name);
            if (strncmp(USER_VAR_DEFS[i].file_name, file_name, flen)==0)
            {
                len = strlen(ident)>strlen(USER_VAR_DEFS[i].ident)?strlen(ident):strlen(USER_VAR_DEFS[i].ident);
                if (strncmp(USER_VAR_DEFS[i].ident, ident, len)==0)
                {

                    *type_class = USER_VAR_DEFS[i].tok_class;
                    *type_num  = USER_VAR_DEFS[i].tok_type;
                    return 1;
                }
            }
        }
        else
        {
            len = strlen(ident)>strlen(USER_VAR_DEFS[i].ident)?strlen(ident):strlen(USER_VAR_DEFS[i].ident);
            if (strncmp(USER_VAR_DEFS[i].ident, ident, len)==0)
            {
                *type_class = USER_VAR_DEFS[i].tok_class;
                *type_num   = USER_VAR_DEFS[i].tok_type;
                return 1;
            }
        }
    }
    return 0;
}


// Komandanyň görnüşini tanaýar
int get_cmd_value_type(command *cmd, int *type_class, int *type_num)
{
    int item_pos = 0;
    // Yglan etmek komandanyň identifikatorynyň
    if (is_def_var_cmd(cmd))
    {
        if (is_glob_def_var_cmd(cmd))
        {
            item_pos = 1;
        }

        *type_class = cmd->items[item_pos].tok.potentional_types[0].type_class;
        *type_num   = cmd->items[item_pos].tok.potentional_types[0].type_num;
        return 1;
    }

    return 0;
}

