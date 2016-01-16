/** 3rdParty/STD funksiýa
    Çap etmek
**/
#include <stdlib.h>
#include <string.h>

#include "../../fn.h"
#include "../../../fns.h"
#include "../../../translator_to_c.h"
#include "chap_et.h"
#include "../../../token/token_types.h"
#include "../../../token/token_types/const_data.h"

int add_std_chap_et()
{
    wchar_t *file_name = L"tpl_std.c",
            *fn_tpl_name   = L"çap_et",
            *fn_c_name = L"printf",
            *c_lib_name = L"<stdio.h>";
    func std_chap_et;
    std_chap_et.file_num = -1;
    wcsncpy(std_chap_et.file_name, file_name, wcslen(file_name)+1);
    wcsncpy(std_chap_et.name, fn_tpl_name, wcslen(fn_tpl_name)+1);
    std_chap_et.type_class = FUNC_CLASS_3RD_PARTY;
    std_chap_et.type_num = FUNC_TYPE_STD;
    std_chap_et.args_num = -1;     // * ýaly argumentleri kabul edýär
    std_chap_et.inf_wchar_t = 0;
    std_chap_et.inf_wchar_t_pos = 0;
    std_chap_et.inf_line_pos = 0;
    std_chap_et.return_class = TOK_CLASS_CONST_DATA;
    std_chap_et.return_type = INT_CONST_DATA_TOK_NUM;
    wcsncpy(std_chap_et.c_name, fn_c_name, wcslen(fn_c_name)+1);
    wcsncpy(std_chap_et.c_lib, c_lib_name, wcslen(c_lib_name)+1);
    std_chap_et.make_args_string = make_std_chap_et_args;
    add_func_to_list(std_chap_et);

    // Argumentler
    func_arg std_chap_et_arg;
    std_chap_et_arg.type_class = TOK_CLASS_CONST_DATA;
    std_chap_et_arg.type_num   = STRING_CONST_DATA_TOK_NUM;
    add_arg_to_last_func(std_chap_et_arg, 0);

    return 1;
}


void make_std_chap_et_args(parenthesis *paren, wchar_t **line, int *llen)
{
    wchar_t *first = NULL,
            *second = NULL,
            *dquote = L"\"",
            *empty = L"",
            *fplace = L"%ls",
            *item_separator = L", ";
    int flen = 0, slen = 0;

    wcsadd_on_heap( &first, &flen, dquote );
    wcsadd_on_heap( &second, &slen, empty );

    int i;
    for(i=0; i<paren->elems_num; ++i)
    {
        parenthesis_elem *p_es = get_paren_elems(paren->elems);

        wcsadd_on_heap( &first, &flen, fplace );
        wcsadd_on_heap( &second, &slen, item_separator );
        paren_item_get_c_code( &p_es[i], &second, &slen );
    }
    wcsadd_on_heap( &first, &flen, dquote );

    wcsadd_on_heap( line, llen, first );
    wcsadd_on_heap( line, llen, second );

    free(first);
    free(second);
}
