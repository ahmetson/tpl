/** Funksiýalar bilen işleýän maglumatlar
**/
#include <stdlib.h>
#include <string.h>

#include "../cmds.h"
#include "fn_call.h"
#include "../translator_to_c/includes.h"
#include "../main/glob.h"
#include "../error.h"

int FN_CALL_TYPE_NUM = 0;

/**
 * Ülňi yglan etmek komandany saýgarýar
**/
int is_cmd_fn_call(command *cmd)
{
    if (cmd->items_num>DEF_VAR_MAX_TOKENS || !cmd->items_num)
	{
		// Komandany savgaryp bolmady
		return 0;
	}

   if (cmd->items_num)
   {
        if (cmd->items[0].type==PAREN_ITEM)
            fn_call_cmd_mod(cmd, 0);
        else
            return 0;
   }

   if (cmd->items_num==2)
   {

        if (cmd->items[1].type==TOKEN_ITEM &&
            cmd->items[1].tok.potentional_types[0].type_class==TOK_CLASS_IDENT)
            fn_call_cmd_mod(cmd, 1);
        else
        {
            return 0;
        }

   }
   return 1;
}


// Def_var komandasy uchin tokene gora maglumatlaryny uytgedyar
int fn_call_cmd_mod(command *cmd, int item_num)
{
    if (item_num==0)
    {
        cmd->cmd_class = CMD_CLASS_FN;
        cmd->cmd_type  = FN_CALL_TYPE_NUM;
        cmd->is_compl  = 0;
        cmd->parenthesis = 1;

        return 1;
    }
    else if (item_num==1)
    {
        cmd->is_compl  = 1;

        return 1;
    }
    return 0;
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
    if (!is_fn_exist(cmd->items[1].tok.potentional_types[0].value))
    {
        unknown_fn_call_add(*cmd);
        return 0;
    }
    else
        f = fn_get_by_name(cmd->items[1].tok.potentional_types[0].value);

    // BIRINJI IDENTIFIKATOR BARLANÝAR
    //      Skobka bolmaly. Içinde islendik möçberde
    //          identifikator bolmaly, ýöne bir ýerde öň yglan edilmeli. tipi hökman harpl bolmaly
    //          harpl bolmaly
    //          funksiýa çagyrylyş bolmaly
    //          Ýa-da skobkanyň içi boş hem bolmaly.
    if (!check_fn_args(f->args_num, f->args, &cmd->items[0].paren))
    {
        print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, &cmd->items[1].tok);
    }
    CUR_PART = prev_part;

    // ŞERT #1: Funksiýanyň prototipiniň yglan edilen faýly,
    //          funksiýanyň çagyrylan kodyna inklud etmeli faýllaryň sanawyna goşulmaly
    file_incs *fi;
    // 1) Eger içine inklud etmeli faýla öň faýl goşulmadyk bolsa
    //     a) Içine inklud etmeli faýl taýynlanýar
    if ((cmd->items[1].tok.inf_file_num+1)>INCLUDES_NUM)
    {
        // 1.a)
        fi = includes_add_new();
    }
    else
    {
        // 1.b)
        fi = &INCLUDES[cmd->items[1].tok.inf_file_num];
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
int check_fn_args(int argn, func_arg *args, parenthesis *paren)
{
    // funksiýa çagyrylanda berlen maglumatlaryň barlananynyň nomeri
    int checked_arg_pos = -1, i, j, ret_class, ret_type;

    if (argn==FN_ARGS_MULTIPLE)
    {
        for(j=0; j<paren->elems_num; ++j)
        {
            if (((paren->elems[j].type==TOKEN_ITEM && TOK_RETURN_TYPE[paren->elems[j].tok.potentional_types[0].type_class]
                    [paren->elems[j].tok.potentional_types[0].type_num](&paren->elems[j].tok, &ret_class, &ret_type) &&
                     set_def_type_alias_const_data(&ret_class, &ret_type)) &&
                     ret_class==args[0].type_class && ret_type==args[0].type_num) ||
                ((paren->elems[j].type==CMD_ITEM && CMD_RETURN_TYPE[paren->elems[j].cmd.cmd_class]
                    [paren->elems[j].cmd.cmd_type](&paren->elems[j].cmd, &ret_class, &ret_type) &&
                 set_def_type_alias_const_data(&ret_class, &ret_type)) &&
                     ret_class==args[0].type_class && ret_type==args[0].type_num) ||
                (paren->elems[j].type==PAREN_ITEM && PAREN_RETURN_TYPE[paren->elems[j].paren.type-1](&paren->elems[j].paren, &ret_class, &ret_type) &&
                     set_def_type_alias_const_data(&ret_class, &ret_type) &&
                     ret_class==args[0].type_class && ret_type==args[0].type_num)
                )
            {
                checked_arg_pos = j;
            }
            else
            {
                if(paren->elems[j].type==TOKEN_ITEM && ret_class==TOK_CLASS_UNKNOWN)
                    unknown_tok_add(&paren->elems[j].tok, CMD_CLASS_FN, FN_CALL_TYPE_NUM, args[0].type_class, args[0].type_num);
                else if(paren->elems[j].type==CMD_ITEM && ret_class==TOK_CLASS_UNKNOWN)
                    unknown_cmd_add(&paren->elems[j].cmd, CMD_CLASS_FN, FN_CALL_TYPE_NUM, args[0].type_class, args[0].type_num);
                else if(paren->elems[j].type==TOKEN_ITEM && ret_class==TOK_CLASS_UNKNOWN)
                    unknown_paren_add(&paren->elems[j].paren, CMD_CLASS_FN, FN_CALL_TYPE_NUM, args[0].type_class, args[0].type_num);
                else
                {
                    CUR_PART = 7;
                    print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, &inf_tok);
                }
                checked_arg_pos = j;
            }
        }
    }
    else
    {
        for(i=0; i<argn; i++)
        {
            for(j=0; j<paren->elems_num; ++j)
            {
                if ((paren->elems[j].type==TOKEN_ITEM && TOK_RETURN_TYPE[paren->elems[j].tok.potentional_types[0].type_class]
                        [paren->elems[j].tok.potentional_types[0].type_num](&paren->elems[j].tok, &ret_class, &ret_type) &&
                     ret_class==args[i].type_class && ret_type==args[i].type_num) ||
                    (paren->elems[j].type==CMD_ITEM && CMD_RETURN_TYPE[paren->elems[j].cmd.cmd_class]
                        [paren->elems[j].cmd.cmd_type](&paren->elems[j].cmd, &ret_class, &ret_type) &&
                     ret_class==args[i].type_class && ret_type==args[i].type_num) ||
                    (paren->elems[j].type==PAREN_ITEM && PAREN_RETURN_TYPE[paren->elems[j].paren.type-1](&paren->elems[j].paren, &ret_class, &ret_type) &&
                     ret_class==args[i].type_class && ret_type==args[i].type_num)
                    )
                    checked_arg_pos = j;
                else
                {
                    if(paren->elems[j].type==TOKEN_ITEM && ret_class==TOK_CLASS_UNKNOWN)
                        unknown_tok_add(&paren->elems[j].tok, CMD_CLASS_FN, FN_CALL_TYPE_NUM, args[0].type_class, args[0].type_num);
                    else if(paren->elems[j].type==CMD_ITEM && ret_class==TOK_CLASS_UNKNOWN)
                        unknown_cmd_add(&paren->elems[j].cmd, CMD_CLASS_FN, FN_CALL_TYPE_NUM, args[0].type_class, args[0].type_num);
                    else if(paren->elems[j].type==TOKEN_ITEM && ret_class==TOK_CLASS_UNKNOWN)
                        unknown_paren_add(&paren->elems[j].paren, CMD_CLASS_FN, FN_CALL_TYPE_NUM, args[0].type_class, args[0].type_num);
                    else
                    {
                        CUR_PART = 7;
                        print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, &inf_tok);
                    }
                    checked_arg_pos = j;
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
    if (!(*llen))
    {
        // Çepe baglanma:
        *llen += strlen("\t")+1;
        *line = realloc(*line, *llen);

        // Içki funksiýanyň içinde bolany üçin, tab goýulyp blokdadygy görkezilýär.
        strncpy(*line, "\t", strlen("\t")+1);
    }

    // Eger birinji birlik ülňi yglan etmek bolsa, komandanyň içinden tokeniň ady alynýar
    // Eger birinji ülňi identifikator bolsa, özi alynýar.
    func *func_params = fn_get_by_name(cmd->items[1].tok.potentional_types[0].value);

    *llen += strlen(func_params->c_name);
    *line = realloc(*line, *llen);
    strncat(*line,func_params->c_name,strlen(func_params->c_name));

    *llen += strlen("(");
    *line = realloc(*line, *llen);
    strncat(*line,"(", strlen("("));


    /** Ýasaljak koddaky funksiýa çagyrylmagynda, argumentleriň C dilindäki analogyny edýär.

    \funcParams - argumentleri ýerleşdirmegiň analogy
    \paren      - C dilindäki analogyny ýasamaly argumentleriň saklaýjysy

    \return     - taýyn C dilindäki analogy
    **/

    func_params->make_args_string(&cmd->items[0].paren, line, llen);

    // Funksiýany ýapýar
    *llen += strlen(")");
    *line = realloc(*line, *llen);
    strncat(*line,")",strlen(")"));

    char *cmd_end = "; \n";
    *llen += strlen(cmd_end);
    *line = realloc(*line, *llen);
    strncat(*line,cmd_end,strlen(cmd_end));

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
    char *n = cmd->items[1].tok.potentional_types[0].value;
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
