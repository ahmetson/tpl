#include <string.h>
#include <stdlib.h>
#include "user_def_type.h"
#include "array.h"
#include "../translator_to_c/includes.h"
#include "../cmds.h"
#include "../paren/types.h"
#include "../parenthesis.h"
#include "../main/glob.h"
#include "../main/user_def_type.h"
#include "../main/inf.h"
#include "../token/harpl.h"
#include "../error.h"
#include "../fns.h"

/** ŞERT #20: Täze tipi yglan edýän komandanyň ähli funksiýalary:

    ** Komandany tanaýan funksiýa
    Eger komandanyň birinji elementi ÜÇBURÇ blogy açýan bolmasa
        Ýalňyşlyk görkezilýär.
    Eger komandanyň ikinji birligi bar bolsa
        Eger komandanyň ikinji birligi IDENTIFIKATOR bolsa
            Eger identifikator öň ulanylan bolsa:
                "Identifikator öň ulanyldy" diýen ýalňyşlyk girizilýär.

    Eger komandanyň üçünji birligi bar bolsa
        Eger komandanyň üçünji birligi UTYPE tokeni bolsa
            Komanda gutarylan diýip ötürdilýär.
            Ulanyjynyň yglan eden görnüşlerine ýene biri ötürdilýär. (add_new_utype(ident, items_num))
            Wagtlaýyn birlikleri saklaýan funksiýadan ülňiler alynyp iň soňky UGyň görnüşleriniň içine salynýar. (move_tmp_utype_items_to_last)
*/

/** ŞERT #22: Komandanyň semantikasyny barlaýan funksiýada:
    ** Identifikatoryň öň ulanylandygy, komanda birlik goşulanda barlandy.
    Eger blok içkiligi faýl derejesinde bolmasa,
        "UG'lar diňe faýl derejesinde yglan edilip bolunýar" diýen ýalňyşlyk görkezilýär.
*/

/** ŞERT #23: **Parserleriň tokeni parsinglenýän ýerde
    Eger token ÜÇBURÇ BLOGY açmak bolsa:
        parse_triangle_blok_inside() diýen funksiýa çagyrylýar.
*/

/** ŞERT #24: parse_triangle_blok_inside() diýen funksiýa-da:
    ** Häzir üçburç bloklar diňe bir komanda bilen ulanylýany üçin,
       diňe şol komandanyň talap edýän maglumatlary barlanar.
    ** Üçburç bloklaryň içinde pragmalar, komandany gutarýan belgi bolup bilenok.
    Eger harp tokeniň başy bolsa:
        token ýasalýar.
        Eger token üçburçlygy ýapýan bolsa:
            Eger komanda is_triangle_block_support_cmd() diýen funksiýadan geçip bilmese:
            "Komanda üçburç bloklaryň goldaýany däl" diýen ýalňyşlyk görkezilýär.
            add_to_tmp_triangle_blok_items() diýen funksiýa çagyrylýar.
            Komanda indiki birlik üçin arassalanýar.

            exit_triangle_block_parser() diýen funksiýa çagyrylýar.
            Bu funksiýa-da, wagtlaýyn üçburç blogyň içindäki birlikleriň maglumatlary seredilýär.
            Eger boş bolsa, onda "Boş tipi yglan etmek bolanok" diýen ýalňyşlyk görkezilýär.
        Ýogsa
            komandanyň içine salynýar.
    Eger harp komandany bölüji bolsa:
        Eger komanda is_triangle_block_support_cmd() diýen funksiýadan geçip bilmese:
            "Komanda üçburç bloklaryň goldaýany däl" diýen ýalňyşlyk görkezilýär.
        add_to_tmp_triangle_blok_items() diýen funksiýa çagyrylýar. (Funksiýa-da ilki komandanyň semantikasy barlanýar)
        Komanda indiki birlik üçin arassalanýar.

*/

/** ŞERT #25: Ulanyjynyň yglan eden tipleriniň C kodyny ýazýan funksiýa:
    Her yglan edilen tipler üçin:
        tip hakda maglumaty açýan setir ýazylýar.

        bloklaryň içkiligi bir san ulalýar.

        Her yglan edilen tipiň birlikleri üçin:
            Eger birligiň tipi, identifikatory ýazylýar.
            Eger birlik sanaw bolsa: sanawyň içkiligini goşýan funksiýa çagyrylýar.

        bloklaryň içkiligi bir san kiçelýär.

        tip hakda maglumaty ýapýan setir ýazylýar.

        iki sany boş setir ýazylýar.
*/

/** ŞERT #26: UG'a çatylma ýasalyp bolansoň,
    UG'y yglan edilende, içindäki birlikleriň semantikasy hem barlanýar.
    Semantika üç burç blogyň içini parsing edilen soň, wagtlaýyn sanawa salynýan funksiýa-da barlanýar.
    Ilki öň şeýle identifikatorly birlik barlygy barlanýar.
    Eger identifikator eýýäm tipiň içinde ulanylan bolsa:
        "Şol bir identifikator gaýtalanyp ulanmak bolanok" diýen ýalňyşlyk görkezilýär.
    Eger birligiň tipi UG bolsa, onda UG-laryň arasynda tip barlanýar.
    Eger tip tapylmasa:
        "Näbelli tip ulanylýar" diýen ýalňyşlyk görkezilýär.
*/



/** Ulanyjynyň tipi bilen baglanyşykly (ýagny UG klasly) komandadygyny barlaýar.
*/
int is_cmd_utype(command *cmd)
{
    if (!cmd->items_num)
        return 0;
    if (is_cmd_utype_con(cmd))
        return 1;
    return is_cmd_utype_def(cmd);
}

/** Ulanyjynyň tipindäki ülňiniň birligini çatylma komandalygyny barlaýar.*/
int is_cmd_utype_con(command *cmd)
{
    /** Birinji element: token, sanawyň elementine ýa ug elementine çatylma bolmaly.*/
    command_item *fci = get_cmd_item(cmd->items, 0);
    int answer = (fci->type==TOKEN_ITEM && fci->tok.type_class==TOK_CLASS_IDENT);
    if (answer)
    {
        cmd_utype_con_mod(cmd, 0);
    }
    else
    {
        answer    = (fci->type==CMD_ITEM   && ((fci->cmd.cmd_class==CMD_CLASS_ARR && fci->cmd.cmd_type==CMD_CLASS_ARR_CON)||
                                               (fci->cmd.cmd_class==CMD_CLASS_UTYPE && fci->cmd.cmd_type==CMD_UTYPE_CON_TYPE)));
        if (answer)
        {
            cmd_utype_con_mod(cmd, 0);
        }
        else
        {
            return 0;
        }
    }

    /** Ikinji element: Birilikleri bölüji token bolmaly*/
    if (cmd->items_num>1)
    {
        command_item *sci = get_cmd_item(cmd->items, 1);
        if (sci->type==TOKEN_ITEM && sci->tok.type_class==TOK_CLASS_UTYPE &&
            sci->tok.potentional_types[0].type_num      ==UTYPE_ITEM_SEPARATOR)
        {
            cmd_utype_con_mod(cmd, 1);
        }
        else
        {
            return 0;
        }
    }
    /** Ikinji element: IDENT görnüşdäki token bolmaly*/
    if (cmd->items_num>2)
    {
        command_item *tci = get_cmd_item(cmd->items, 2);
        if (tci->type==TOKEN_ITEM && tci->tok.type_class==TOK_CLASS_IDENT)
        {
            cmd_utype_con_mod(cmd, 2);
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

/** Ulanyjynyň täze tipini yglan edýän komandalygyny barlaýar.*/
int is_cmd_utype_def(command *cmd)
{
    command_item *fci = get_cmd_item(cmd->items, 0);

    wchar_t type = 0;
    if (fci->type==TOKEN_ITEM && fci->tok.potentional_types[0].type_class==TOK_CLASS_TRIANGLE_BLOCK &&
                                 fci->tok.potentional_types[0].type_num==  TOKEN_TRIANGLE_BLOCK_OPEN_TYPE)
    {
        type = CMD_UTYPE_DEF_TYPE;
        cmd_utype_def_mod(cmd, 0);
    }
    else
    {
        return 0;
    }

    if (cmd->items_num-1>=1)
    {
        command_item *sci = get_cmd_item(cmd->items, 1);
        if (sci->type==TOKEN_ITEM && sci->tok.potentional_types[0].type_class==TOK_CLASS_IDENT)
        {
            if (is_ident_used(&sci->tok, 0))
            {
                CUR_PART = 4;
                print_err(CODE4_VARS_IDENT_USED, &sci->tok);
            }
            cmd_utype_def_mod(cmd, 1);
        }
        else
        {
            return 0;
        }
    }

    if (cmd->items_num-1>=2)
    {
        command_item *tci = get_cmd_item(cmd->items, 2);
        if (tci->type==TOKEN_ITEM && tci->tok.potentional_types[0].type_class==TOK_CLASS_UTYPE &&
            tci->tok.potentional_types[0].type_num==UTYPE_DEF_SIGNIFIER)
        {
            cmd_utype_def_mod(cmd, 2);
        }
        else
        {
            return 0;
        }
    }

    if (CMD_MAX_ITEMS[cmd->cmd_class][cmd->cmd_type]<cmd->items_num)
    {
        /// GATY KAN BIRLIKLER BAR
        /// TODO "Komanda-da artykmaç birlik bar" diýen ýalňyşlyk görkezilýär.
        return 0;
    }

    return 1;
}

/** Komandanyň düzmelerini, täze tipi yglan edýän komandanyň talabyna görä üýtgedýär*/
void cmd_utype_def_mod(command *cmd, int item_num)
{
    if (item_num==0)
    {
        cmd->cmd_class = CMD_CLASS_UTYPE;
        cmd->cmd_type  = CMD_UTYPE_DEF_TYPE;
        cmd->is_compl  = 0;
    }
    else if (item_num==1)
    {

    }
    else if (item_num==2)
    {
        add_new_utype(cmd);
        move_tmp_utype_items_to_last();
        cmd->is_compl = 1;
    }
}

/** Komandanyň düzmelerini, Ulanyjynyň tipindäki ülňiniň birligine çatylmak komandanyň
    talabyna görä üýtgedýär.*/
void cmd_utype_con_mod(command *cmd, int item_num)
{
    if (item_num==0)
    {
        cmd->cmd_class = CMD_CLASS_UTYPE;
        cmd->cmd_type  = CMD_UTYPE_CON_TYPE;
        cmd->is_compl  = 0;
    }
    else if (item_num==1)
    {
    }
    else if (item_num==2)
    {
        cmd->is_compl = 1;
    }
}


/** Ulanyjynyň görnüşindäki ülňiniň çatylýan birliginiň tipini gaýtarýar.*/
int cmd_utype_con_return_type(command *cmd, int *type_class, int *type_num)
{
    return get_utype_item_type_by_cmd(cmd, type_class, type_num);
}

/** Ulanyjynyň görnüşindäki ülňiniň birligine çatylanda,
    görnüşde birligiň barlygyny, görnüşiň dogrylygyny barlaýar*/
int semantic_cmd_utype_con(command *cmd)
{
    /// Birinji berlen at boýunça tapylan tipde ikinji berlen birlik bolmaly.
    /// Ikinji elementi tanamak üçin, birinji birlik boýunça ikinji elementiň degişli bolan tipiniň adresini almaly.

    /** Birinji element barlanýar.
        Eger birlik token bolsa
            get_ident_type diýen funksiýadan geçirilýär.
            Eger identifikatoryň tipiniň klasy UG bolmasa
                "Diňe UG tipidäki ülňileriň birliklerine çatylyp bolýar" diýen ýalňyşlyk görkezilýär.
            Ýogsa
                identifikatoryň tipini, komandanyň ikinji birliginiň degişli bolan tipi diýip goýulýar.
        Eger birlik komanda bolsa
            Eger UG-yň birligine çatylma bolsa:
                get_utype_item_type_by_cmd() diýen funksiýa çagyrylýar.
                Funksiýanyň gaýtarýan tipi, komandanyň ikinji birliginiň degişli bolan tipi diýip goýulýar.
            Ýa Sanaw birligine çatylma bolsa:
                get_arr_item_type_by_cmd() diýen funksiýa çagyrylýar.
                Funksiýanyň gaýtarýan tipi, komandanyň ikinji birligine degişli bolan tipi diýip gaýtarylýar.
            Ýogsa
                "Haýsy ülňiniň birligi çagyrylýany tanalmady" diýen ýalňyşlyk görkezilýär.
        Ýogsa
            "Haýsy ülňiniň birligi çagyrylýany tanalmady" diýen ýalňyşlyk görkezilýär.

        Eger get_utype_item_addr() diýen funksiýa nädogrylygy gaýtarsa
            "Ugyň çagyrylan birligi duş gelmedi" diýen ýalňyşlyk görkezilýär.
        Şowlylyk gaýtarylýar.
    */
    int tmp_class = -1, tmp_type = -1;
    command_item *fci = get_cmd_item(cmd->items, 0);

    if (fci->type==TOKEN_ITEM)
    {
        return_tok_type(&fci->tok, &tmp_class, &tmp_type);
        if (tmp_class!=TOK_CLASS_UTYPE_CON)
        {
            printf("%s %d setirde: Diňe UG tipidäki ülňileriň birliklerine çatylyp bolýar\n", __FILE__, __LINE__);
            return 0;
        }
    }
    else if(fci->type==CMD_ITEM)
    {
        if(fci->cmd.cmd_class==CMD_CLASS_UTYPE && fci->cmd.cmd_type==CMD_UTYPE_CON_TYPE)
        {
            if (!get_utype_item_type_by_cmd(&fci->cmd, &tmp_class, &tmp_type))
            {
                printf("%s %d setirde: Diňe UG tipidäki ülňileriň birliklerine çatylyp bolýar\n", __FILE__, __LINE__);
                return 0;
            }
        }
        else if (fci->cmd.cmd_class==CMD_CLASS_ARR && fci->cmd.cmd_type==CMD_CLASS_ARR_CON)
        {
            if (!get_arr_item_type_by_cmd(&fci->cmd, &tmp_class, &tmp_type))
            {
                printf("%s %d setirde: Diňe UG tipidäki ülňileriň birliklerine çatylyp bolýar\n", __FILE__, __LINE__);
                return 0;
            }
        }
        else
        {
                printf("%s %d setirde: Haýsy ülňiniň birligi çagyrylýany tanalmady\n", __FILE__, __LINE__);
                return 0;
        }
    }

    command_item *tci = get_cmd_item(cmd->items, 2);
    wchar_t *item_ident = tci->tok.potentional_types[0].value;
    if (!get_utype_item_addr(tmp_type, item_ident, &tmp_class))
    {
        printf("%s %d setirde: Ugyň çagyrylan birligi duş gelmedi\n", __FILE__, __LINE__);
        return 0;
    }
    return 1;
}

/** Täze ulanyjynyň tipi yglan edilende,
    Global ülňiler, sanawlar ülňiniň içinde yglan edilmedigi barlanýar*/
int semantic_cmd_utype_def(command *cmd)
{
    if (cmd->ns==GLOB && GLOB_BLOCK_INCLUDES)
    {
        CUR_PART = 7;
        print_err(CODE7_GLOB_DEF_IN_BLOCK, (token *)inf_get_last_token(cmd));
    }
    return 1;
}

/** Faýla degişli kody C koda ýazýar
*/
void cmd_utype_con_c_code(command *cmd, wchar_t **line, int *line_len)
{
    int ident_pos = 0;
    if (cmd->ns==GLOB)
        ident_pos = 1;

    command_item *fci = get_cmd_item(cmd->items, ident_pos);

    if (fci->type==CMD_ITEM)
    {
        CMD_GET_C_CODE[fci->cmd.cmd_class][fci->cmd.cmd_type](&fci->cmd, line, line_len);
    }
    else if (fci->type==TOKEN_ITEM)
    {
        TOK_GET_C_CODE[fci->tok.potentional_types[0].type_class][fci->tok.potentional_types[0].type_num](&fci->tok, line, line_len);
    }

    wchar_t *item_separator = L".";
    wcsadd_on_heap( line, line_len, item_separator );

    command_item *sci = get_cmd_item(cmd->items, cmd->items_num-1);
    wchar_t *item_ident = sci->tok.potentional_types[0].value;
    wcsadd_on_heap( line, line_len, item_ident );
}



