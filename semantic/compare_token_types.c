/***
 * Maglumatlaryň token tipleri barlanýar
***/
#include <stdio.h>
#include <string.h>

#include "../main/glob.h"
#include "compare_token_types.h"
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
            //debug_token(&GLOB_RIGHT_DATA_CMDS_LIST[i].left.tok);
            get_user_var_def_type(GLOB_RIGHT_DATA_CMDS_LIST[i].left.tok.potentional_types[0].value, &left_class, &left_type,
                                GLOB_RIGHT_DATA_CMDS_LIST[i].inf_file_name);
            set_def_type_alias_const_data(&left_class, &left_type);
        }
        else
        {
            left_class = GLOB_RIGHT_DATA_CMDS_LIST[i].left.cmd.cmd_class;
            left_type  = GLOB_RIGHT_DATA_CMDS_LIST[i].left.cmd.cmd_type;
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
            print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA);
        }
    }
}


int get_user_var_def_type(char *ident, int *type_class, int *type_num, char *file_name)
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
                    *type_num   = USER_VAR_DEFS[i].tok_type;
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

