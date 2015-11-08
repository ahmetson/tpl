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

    size = sizeof(char ***)*CUR_FILE_NUM;
    GLOB_SOURCE_CODES = realloc(GLOB_SOURCE_CODES, size);

    // Faýlda setiriň goşmaly
    GLOB_SOURCE_CODES[flast] = NULL;
}



/**
 * Tokene, onuň faýl bilen baglanyşykly maglumatlary goýulýar.
**/
void inf_add_to_token(token *tok, char c, int c_pos, int line)
{

    if (tok->inf_char_num<1)
    {
        tok->inf_char = c;
        tok->inf_char_num = c_pos;
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
void inf_next_char()
{
    CUR_CHAR_POS++;
}


token *inf_get_last_token(command *cmd)
{
    if (cmd->items_num)
    {
        if(cmd->items[cmd->items_num-1].type==CMD_ITEM)
            return inf_get_last_token(&cmd->items[cmd->items_num-1].cmd);
        return &(cmd->items[cmd->items_num-1].tok);
    }
    return &inf_tok;
}


void update_inf()
{
    if (CUR_CHAR=='\n')
        inf_next_line();
    else
        inf_next_char();
}
