/** Funksiýalar bilen işleýän maglumatlar
**/
#include <stdlib.h>
#include <string.h>

#include "../cmds.h"
#include "fn_call.h"
#include "../translator_to_c/includes.h"
#include "../main/glob.h"
#include "../error.h"
#include "../paren/types.h"

int FN_CALL_TYPE_NUM = 0;

/**
 * Ülňi yglan etmek komandany saýgarýar
**/
int is_cmd_fn_call(command *cmd)
{
    if (cmd->items_num>CMD_MAX_ITEMS[cmd->cmd_class][cmd->cmd_type] || !cmd->items_num)
	{
		// Komandany savgaryp bolmady
		return 0;
	}

    if (cmd->items_num)
    {
       command_item *fci = get_cmd_item(cmd->items,0);
        if (fci->type==PAREN_ITEM && fci->paren.type!=PAREN_TYPE_DEF_TYPE)
            fn_call_cmd_mod(cmd, 0);
        else
            return 0;
    }

    if (cmd->items_num==2)
    {
        command_item *sci = get_cmd_item(cmd->items,1);
        if (sci->type==TOKEN_ITEM && sci->tok.potentional_types[0].type_class==TOK_CLASS_IDENT)
            fn_call_cmd_mod(cmd, 1);
        else
        {
            return 0;
        }
    }
    else if (cmd->items_num>2)
        return 0;
    return 1;
}


// Def_var komandasy uchin tokene gora maglumatlaryny uytgedyar
void fn_call_cmd_mod(command *cmd, int item_num)
{
    if (item_num==0)
    {
        cmd->cmd_class = CMD_CLASS_FN_CALL;
        cmd->cmd_type  = FN_CALL_TYPE_NUM;
        cmd->is_compl  = 0;
        cmd->parenthesis = 1;
    }
    else if (item_num==1)
    {
        cmd->is_compl  = 1;
    }
}

// Funksiça çagyryş komandasynyn semantikasy
int semantic_cmd_fn_call(command *cmd)
{
    int prev_part = CUR_PART;
    CUR_PART = 7;
    func *f = NULL;
    // KOMANDANYN CHEPE BAGLANMA GORNUSHI UCHIN

    //printf("Chepe baglanyan komandanyn semantikasy barlanmaly\n");

    // IKINJI BIRLIK BARLANÝAR
    // IDENTIFIKATOR BOLMALY, FUNKSIÝALARYŇ SANAWYNDA DUŞMALY
    // CONTINUE
    command_item *sci = get_cmd_item(cmd->items,1);

    if (!is_fn_exist(sci->tok.potentional_types[0].value))
    {
        CUR_PART = 7;
        print_err(CODE7_UNKNOWN_FNS_CALLED, &sci->tok);
        return 0;
    }
    else
        f = fn_get_by_name(sci->tok.potentional_types[0].value);

    // BIRINJI IDENTIFIKATOR BARLANÝAR
    //      Skobka bolmaly. Içinde islendik möçberde
    //          identifikator bolmaly, ýöne bir ýerde öň yglan edilmeli. tipi hökman harpl bolmaly
    //          harpl bolmaly
    //          funksiýa çagyrylyş bolmaly
    //          Ýa-da skobkanyň içi boş hem bolmaly.
    command_item *fci = get_cmd_item(cmd->items,0);
    if (!check_fn_args(f->args_num, f->args, &fci->paren))
    {
        print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, &sci->tok);
    }
    CUR_PART = prev_part;

    // ŞERT #1: Funksiýanyň prototipiniň yglan edilen faýly,
    //          funksiýanyň çagyrylan kodyna inklud etmeli faýllaryň sanawyna goşulmaly
    file_incs *fi;
    // 1) Eger içine inklud etmeli faýla öň faýl goşulmadyk bolsa
    //     a) Içine inklud etmeli faýl taýynlanýar
    if ((sci->tok.inf_file_num+1)>INCLUDES_NUM)
    {
        // 1.a)
        fi = includes_add_new();
    }
    else
    {
        // 1.b)
        fi = &INCLUDES[sci->tok.inf_file_num];
    }

    // 2)
    includes_file_add_include(fi, f->c_lib);

    return 1;
}

int fn_call_cmd_return_type(command *cmd, int *return_class, int *ret_type)
{
    *return_class = TOK_CLASS_UNDEFINED;
    return 0;
}


/** Çagyrylan funksiýanyň kabul edýän argumentleriniň, funksiýanyň garaşýan argument tiplerini gabat gelýänini barlaýar

    \argn  - funksiýanyň garaşýan argumentleriniň sany

    \args  - funksiýanyň garaşýan argumentleri

    \paren - funksiýa çagyrylynda berlen maglumatlar
**/
int check_fn_args(int argn, int args_num, parenthesis *paren)
{
    func_arg *args = get_fn_args(args_num);
    parenthesis_elem *paren_elems = get_paren_elems(paren->elems);
    // funksiýa çagyrylanda berlen maglumatlaryň barlananynyň nomeri
    int checked_arg_pos = -1, i, j, ret_class = -1, ret_type = -1;

    if (argn==FN_ARGS_MULTIPLE)
    {
        for(j=0; j<paren->elems_num; ++j)
        {
            if (paren_elems[j].type==TOKEN_ITEM)
            {
                token_type *tt = &paren_elems[j].tok.potentional_types[0];
                return_tok_type(&paren_elems[j].tok, &ret_class, &ret_type);
            }
            else if (paren_elems[j].type==CMD_ITEM)
                CMD_RETURN_TYPE[paren_elems[j].cmd.cmd_class][paren_elems[j].cmd.cmd_type](&paren_elems[j].cmd, &ret_class, &ret_type);
            else if (paren_elems[j].type==PAREN_ITEM)
                PAREN_RETURN_TYPE[paren_elems[j].paren.type-1](&paren_elems[j].paren, &ret_class, &ret_type);

            if(ret_class==args[0].type_class && ret_type==args[0].type_num)
            {
                checked_arg_pos = j;
            }
            else
            {
                CUR_PART = 7;
                if(paren_elems[j].type==TOKEN_ITEM)
                    print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, &paren_elems[j].tok);
                else if(paren_elems[j].type==CMD_ITEM)
                    print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, (token *)inf_get_last_token(&paren_elems[j].cmd));
                else if(paren_elems[j].type==TOKEN_ITEM && ret_class==TOK_CLASS_UNKNOWN)
                    print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, (token *)inf_get_parens_last_token(&paren_elems[j].paren));
            }
        }
    }
    else
    {
        for(i=0; i<argn; i++)
        {
            for(j=0; j<paren->elems_num; ++j)
            {
                if (paren_elems[j].type==TOKEN_ITEM)
                {
                    token_type *tt = &paren_elems[j].tok.potentional_types[0];
                    return_tok_type(&paren_elems[j].tok, &ret_class, &ret_type);
                 }
                else if(paren_elems[j].type==CMD_ITEM)
                    CMD_RETURN_TYPE[paren_elems[j].cmd.cmd_class][paren_elems[j].cmd.cmd_type](&paren_elems[j].cmd, &ret_class, &ret_type);
                else if(paren_elems[j].type==PAREN_ITEM)
                    PAREN_RETURN_TYPE[paren_elems[j].paren.type-1](&paren_elems[j].paren, &ret_class, &ret_type);

                if (ret_class==args[i].type_class && ret_type==args[i].type_num)
                    checked_arg_pos = j;
                else
                {
                    CUR_PART = 7;
                    if(paren_elems[j].type==TOKEN_ITEM)
                        print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, &paren_elems[j].tok);
                    else if(paren_elems[j].type==CMD_ITEM)
                        print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, (token *)inf_get_last_token(&paren_elems[j].cmd));
                    else if(paren_elems[j].type==TOKEN_ITEM && ret_class==TOK_CLASS_UNKNOWN)
                        print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, (token *)inf_get_parens_last_token(&paren_elems[j].paren));
                }
            }
        }
    }

    // Hemme argumentler barlanan bolmaly
    if ((checked_arg_pos+1)!=paren->elems_num)
    {
        CUR_PART = 7;
        print_err(CODE7_FN_ARG_NUMES_NOT_MATCH, &inf_tok);
    }
    return 1;
}




/** Faýla degişli kody C koda ýazýar
**/
void cmd_fn_call_c_code(command *cmd, char **line, int *llen)
{
    // Eger birinji birlik ülňi yglan etmek bolsa, komandanyň içinden tokeniň ady alynýar
    // Eger birinji ülňi identifikator bolsa, özi alynýar.
    command_item *sci = get_cmd_item(cmd->items,1);
    func *func_params = fn_get_by_name(sci->tok.potentional_types[0].value);

    if (!(*llen))
    {
        *llen += strlen(func_params->c_name)+1;
        *line = realloc(*line, *llen);
        strncpy(*line,func_params->c_name,strlen(func_params->c_name)+1);
    }
    else
    {
        *llen += strlen(func_params->c_name);
        *line = realloc(*line, *llen);
        strncat(*line,func_params->c_name,strlen(func_params->c_name));
    }

    *llen += strlen("(");
    *line = realloc(*line, *llen);
    strncat(*line,"(", strlen("("));


    /** Ýasaljak koddaky funksiýa çagyrylmagynda, argumentleriň C dilindäki analogyny edýär.

    \funcParams - argumentleri ýerleşdirmegiň analogy
    \paren      - C dilindäki analogyny ýasamaly argumentleriň saklaýjysy

    \return     - taýyn C dilindäki analogy
    **/
    command_item *fci = get_cmd_item(cmd->items,0);
    func_params->make_args_string(&fci->paren, line, llen);

    // Funksiýany ýapýar
    *llen += strlen(")");
    *line = realloc(*line, *llen);
    strncat(*line,")",strlen(")"));

    // Setir faýla ýazylan soň, setir üçin berlen ýer boşadylýar.
}


int cmd_fn_call_return_type(command *cmd, int *return_class, int *return_type)
{
    if (cmd->items_num<2)
    {
        *return_class = TOK_CLASS_UNKNOWN;
        *return_type  = 0;
        return 1;
    }
    command_item *sci = get_cmd_item(cmd->items,1);
    char *n = sci->tok.potentional_types[0].value;
    if(!is_fn_exist(n))
    {
        *return_class = TOK_CLASS_UNDEFINED;
        return 0;
    }
    func *f = fn_get_by_name(n);
    *return_class = f->return_class;
    *return_type  = f->return_type;

    return 1;
}
