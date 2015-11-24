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

int get_var_def_value_type(char *ident, int *type_class, int *type_num, char glob)
{
    int i, len, num;
    glob_ident *items;
    if (glob)
    {
        num = GLOBAL_VAR_DEFS_NUMS;
        items = GLOBAL_VAR_DEFS;
    }
    else
    {
        num = LOCAL_VAR_DEFS_NUMS;
        items = LOCAL_VAR_DEFS;
    }

    for(i=0; i<num; ++i)
    {
        len = strlen(ident)>strlen(items[i].name)?strlen(ident):strlen(items[i].name);
        if (strncmp(items[i].name, ident, len)==0)
        {
            *type_class = items[i].type_class;
            *type_num   = items[i].type_num;
            return 1;
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

