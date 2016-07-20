/**
 * Programma hakdaky maglumatlar bilen, kompilýasiýa edilip duran wagty görkezmek üçin ýasalan ülňiler we funksiýalar
**/
#include <stdlib.h>
#include "glob.h"

#include "inf.h"

/**
 * Source kodlaryň sanawyna täzesini goşýar.
**/
void inf_add_source_code_last_file()
{
    long size;
    int  flast=0;
    if (CUR_FILE_NUM<1)
        return;

    size = sizeof(wchar_t ***)*CUR_FILE_NUM;
    GLOB_SOURCE_CODES = realloc(GLOB_SOURCE_CODES, size);

    // Faýlda setiriň goşmaly
    GLOB_SOURCE_CODES[flast] = NULL;
}



/** Tokene, onuň faýl bilen baglanyşykly maglumatlary goýulýar.
**/
void inf_add_to_token(token *tok, wchar_t c, int c_pos, int line)
{

    if (tok->inf_wchar_t_num<1)
    {
        tok->inf_wchar_t = c;
        tok->inf_wchar_t_num = c_pos;
        tok->inf_line_num = line;
        tok->inf_file_num = CUR_FILE_NUM-1;
    }
}

/**
 * Indiki setire geçdi
**/
void inf_next_line()
{
    CUR_CHAR_POS = 0;
    CUR_LINE++;
}

/**
 * Indiki setire geçdi
**/
void inf_next_wchar_t()
{
    CUR_CHAR_POS++;
}


token *inf_get_last_token(command *cmd)
{
    if (cmd->items_num)
    {
        command_item *lci = get_cmd_item(cmd->items , cmd->items_num-1);
        if(lci->type==CMD_ITEM)
            return inf_get_last_token(&lci->cmd);
        else if (lci->type==PAREN_ITEM)
            return inf_get_parens_last_token(&lci->paren);
        return &lci->tok;
    }
    return &inf_tok;
}

token *inf_get_parens_last_token(parenthesis *par)
{
    if (par->elems_num)
    {
        parenthesis_elem *par_es = get_paren_elems(par->elems);

        if(par_es[par->elems_num-1].type==CMD_ITEM)
            return inf_get_last_token(&par_es[par->elems_num-1].cmd);
        else if (par_es[par->elems_num-1].type==PAREN_ITEM)
            return inf_get_parens_last_token(&par_es[par->elems_num-1].paren);
        return &par_es[par->elems_num-1].tok;
    }
    return &inf_tok;
}

void update_inf()
{
    if ( CUR_CHAR==L'\n' )
        inf_next_line();
    else
        inf_next_wchar_t();
}


/** Maglumatlar üçin ulanylan ülňiler täze faýl üçin taýynlanýar*/
void free_inf()
{
    CUR_LINE = 1;
    CUR_CHAR_POS = 1;
}
