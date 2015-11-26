/** 3rdParty/STD funksiýa
    Çap etmek
**/
#include <stdlib.h>
#include <string.h>

#include "../../fn.h"
#include "chap_et.h"
#include "../../../token/token_types.h"
#include "../../../token/token_types/const_data.h"

int add_std_chap_et()
{
    func std_chap_et;
    std_chap_et.file_num = -1;
    strncpy(std_chap_et.file_name, "tpl_std.c", strlen("tpl_std.c")+1);
    strncpy(std_chap_et.name, "chap_et", strlen("chap_et")+1);
    std_chap_et.type_class = FUNC_CLASS_3RD_PARTY;
    std_chap_et.type_num = FUNC_TYPE_STD;
    std_chap_et.args_num = -1;     // * ýaly argumentleri kabul edýär
    std_chap_et.inf_char = 0;
    std_chap_et.inf_char_pos = 0;
    std_chap_et.inf_line_pos = 0;
    std_chap_et.return_class = TOK_CLASS_CONST_DATA;
    std_chap_et.return_type = INT_CONST_DATA_TOK_NUM;
    strncpy(std_chap_et.c_name, "printf", strlen("printf")+1);
    strncpy(std_chap_et.c_lib, "<stdio.h>", strlen("<stdio.h>")+1);
    std_chap_et.make_args_string = make_std_chap_et_args;
    add_func_to_list(std_chap_et);

    // Argumentler
    func_arg std_chap_et_arg;
    std_chap_et_arg.type_class = TOK_CLASS_CONST_DATA;
    std_chap_et_arg.type_num   = STRING_CONST_DATA_TOK_NUM;
    add_arg_to_last_func(std_chap_et_arg, 0);

    return 1;
}


void make_std_chap_et_args(parenthesis *paren, char **line, int *llen)
{
    char *first = NULL, *second = NULL;
    int flen = strlen("\"")+1, slen = strlen("")+1;
    first = realloc(first, flen);
    second = realloc(second, slen);
    strncpy(first, "\"", strlen("\"")+1);
    strncpy(second, "", strlen("")+1);
    int i;

    for(i=0; i<paren->elems_num; ++i)
    {
        flen+=strlen("%s");
        first = realloc(first, flen);
        strncat(first, "%s", strlen("%s"));
        slen+=strlen(", ");
        second = realloc(second, slen);
        strncat(second, ", ", strlen(", "));
        if (paren->elems[i].type==TOKEN_ITEM)
            TOK_GET_C_CODE[paren->elems[i].tok.potentional_types[0].type_class]
                                [paren->elems[i].tok.potentional_types[0].type_num](&paren->elems[i].tok, &second, &slen);
        else if(paren->elems[i].type==CMD_ITEM)
            CMD_GET_C_CODE[paren->elems[i].cmd.cmd_class]
                                [paren->elems[i].cmd.cmd_type](&paren->elems[i].cmd, &second, &slen);

        //else if(paren->elems[i].type==PAREN_ITEM)
         //   tmp = PAREN_GET_C_CODE[paren->elems[i].paren.type](&paren->elems[i].paren);
    }
    flen += strlen("\"");
    first = realloc(first, flen);
    strncat(first, "\"", strlen("\""));

    *llen += flen + slen;
    *line = realloc(*line, *llen);
    strncat(*line, first, strlen(first));
    strncat(*line, second, strlen(second));

    free(first);
    free(second);
}
