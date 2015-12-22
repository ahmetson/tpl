/** ŞERT #1: Birsyhly sanawlar bilen işlemek:

    Birsyhly sanawlaryň iki görnüşi bar:
        1)Yglan etmek.
        2)Çatylmak.

    Birsyhly sanawy yglan etmek:
    @ (sanl) tip ident.
    @       - harpy bolsa, diýmek birsyhly (ýa ýöne) sanaw global diýmegi aňladýar.
    (sanl)  - FN_ARGS tipdäki ýaý bolup, diňe sanlardan ybarat bolmaly,
              sanlar - sanawyň näçe elementden durýanyny aňladýar, argumentleriň sany bolsa
              sanawyň näçeräk içindeligini aňladýar.
    tip     - element nämeden durýar.
    ident   - sanawyň elementlerine nädip çatylyp bolýar.


    Birsyhly sanawa çatylmak:
    @ ident sanl.
    @       - global yglan edilen birsyhly sanawa çatylmaly.
    ident   - sanawa näme arkaly çatylmaly
    sanl    - sanawyň näçinji elementine çatylmaly.
              Sanl - birnäçe sanlardan ybarat bolup bilýär.
              Her san, sanawdaky näçinji elemente çatylmalydygyny aňladýar.
              Sanlaryň sany bolsa, elementiň içindeligini aňladýar.
              Içki sanawly sanawyň diňe iň içkisiniň elementlerine çatylyp bolynýar.

    ** C dilinde ýasaljak programma-da komandalaryň bolup biljek görnüşi:
    Birsyhly sanawy yglan etmek:
    tip ident[sanl];
    tip     - sanaw nämelerden durýar.
    ident   - sanawa näme arkaly çatylsaň bolýar.
    [sanl]  - birnäçe ýaý we içinde ýerleşýän sanlardan duryp bilýär.

    Birsyhly sanawa çatylmak:
    ident[sanl];
    ident   - sanawa näme arkaly çatylsaň bolýar.
    [sanl]  - birnäçe ýaý we içinde ýerleşýän sanlardan duryp bilýär.

*/

/** TAÝYNLYK:
    GLOBAL global yglan edilen sanawlaryň sanawy bolýar:
        Sanaw: identifikatordan, içki elementleriň sanyndan, elementiň tipinden, elementiň tipiniň klasyndan durýar.

    GLOBAL global yglan edilen sanawlaryň içkiligini saklaýan sanaw bolýar:
        Sanaw: iki içkiligi bolup durýar:
            Birinji içikilik, sanawyň özüni aňladýar.
            Ikinji içkilik, sanawyň içindäki elementleriniň maglumatlaryny saklaýar.
        Sanaw: özüniň içikliginiň nomerini, we içinde näçe element barlygynyň sanyny saklaýar.

    GLOBAL lokal yglan edilen sanawlaryň sanawy bolýar:
        Sanaw: identifikatordan, içki elementleriň sanyndan, elementiň tipinden, elementiň tipiniň klasyndan durýar.

    GLOBAL lokal yglan edilen sanawlaryň içkiligini saklaýan sanaw bolýar:
        Sanaw: iki içkiligi bolup durýar:
            Birinji içikilik, sanawyň özüni aňladýar.
            Ikinji içkilik, sanawyň içindäki elementleriniň maglumatlaryny saklaýar.
        Sanaw: özüniň içikliginiň nomerini, we içinde näçe element barlygynyň sanyny saklaýar.

    GLOBAL global beýan edilen sanawlaryň sanawy bolýar:
        Sanaw: identifikatordan, içki elementleriň sanyndan, elementiň tipinden, elementiň tipiniň klasyndan durýar.

    GLOBAL global beýan edilen sanawlaryň içkiligini saklaýan sanaw bolýar:
        Sanaw: iki içkiligi bolup durýar:
            Birinji içikilik, sanawyň özüni aňladýar.
            Ikinji içkilik, sanawyň içindäki elementleriniň maglumatlaryny saklaýar.
        Sanaw: özüniň içkiliginiň nomerinden, we içinde näçe element barlygynyň sanyny saklaýar.


    ** HEMME FAÝLLAR PARSING EDILIP BOLANSOŇ
    Eger beýan edilen birsyhly sanawlar bar bolsa,
        HER beýan edilen birsyhly sanaw boýunça:
            Her GLOBAL yglan edilen birsyhly sanawlar boýunça
                Eger beýan edilen sanawyň identifikatory, tipi, tipiniň klasy, içkiliginiň sany global yglan edileniňki bilen gabat gelse
                    Her içkiligiň sany boýunça:
                        Eger global beýan edilen sanawyň içkiliginiň sany we beýan edilen içkiligiň içindäki elementleriň sanlary deň bolmasa
                            Tapylmady diýen status goýulýar.
                        Ýogsa
                            Tapyldy diýen status goýulýar.
        Eger tapylmady diýen status TAPYLMADY diýse:
            "Beýan edilen birsyhly sanaw hiç ýerde yglan edilmedi" diýen ýalňyşlyk görkezilýär.
        Ýogsa
            tapylmady diýen status TAPYLDY diýen statusa geçirilýär.

*/

/** ŞERT #2: Birsyhly sanawy yglan etmeginiň komandasyny tanaýan funksiýa:
    Eger birinji komanda birligi birlik (şu bölümde ýöne birlik) global diýen birlik bolsa,
        Indiki elementiň sanawy 1 diýip goýulýar.
        Komanda global diýilýär.
    Ýogsa
        Indiki elementiň sanawy 0 diýip goýulýar.
        Komanda lokal diýilýär.

    ** ÝAÝ
    Eger komanda-da indiki görmeli birlige çenli birlikler bar bolsa
        Eger indiki birlik ýaý ýa FUNKSIÝANYP ARGUMENTLERI diýen tipde bolmasa ýa is_param_has_ints() ýalňyş bolsa
            yzyna ŞOWSUZLYK gaýtarylýar.
        Ýogsa
            Komandanyň maglumatlaryny üýtgedýän funksiýa çagyrylýar (çmd, 0).
            Indiki elementi barlamak üçin san ulalýar.

    ** TIP
    Eger komanda-da indiki görmeli birlige çenli birlikler bar bolsa
        Eger indiki birlik tip bolmasa
            yzyna ŞOWSUZLYK gaýtarylýar.
        Ýogsa
            Komandanyň maglumatlaryny üýtgedýän funksiýa çagyrylýar. (çmd, 1).
            Indiki elementi barlamak üçin san ulalýar.

    ** IDENT
    Eger komanda-da indiki görmeli birlige çenli birlikler bar bolsa
        Eger indiki birlik identifikator bolmasa
            yzyna ŞOWSUZLYK gaýtarylýar.
        Ýogsa
            Komandanyň maglumatlaryny üýtgedýän funksiýa çagyrylýar. (çmd, 1).
            Indiki elementi barlamak üçin san ulalýar.

    Eger komanda-da indiki görmeli birlige çenli birlikler bar bolsa
        "Komanda-da artykmaç birlik bar" diýen ýalňyşlyk görkezilýär.
*/

/** KOMANDA HIÇ HILI MAGLUMAT GAÝTARMAÝAR. */

/** ŞERT #3: Birsyhly sanawy yglan etmegiň C kodyny ýazýan bölek:
        tip ýazylýar,
        boş ýer ýazylýar,
        identifikator ýalyýar,
        Her ýaýdaky elementler boýunça:
            dörtburç ýaý we içinde san ýazylýar.

        Eger kod Ç faýlynda ýazylýan we tipiň klasy ulanyjy tarapyndan ýasalmadyk bolsa bolsa:
            deňdir ýazylýar,
            tipiň başlangyç maglumaty ýazylýar.
*/

/** ŞERT #6: Eýýäm ulanylan identifikatorlary barlaýan funksiýa-da
        Global, lokal, beýan edilen birsyhly sanawlaryň yglan edilen sanawlarynyň içinden hem identifikatorlary gözleýär.
*/

/** ŞERT #7: Birsyhly sanawy yglan etmekde, global birsyhly sanawlaryň sanawyna goşýan funksiýa yglan edilýär.
    Bu yglan edilen funksiýa, nirede ülňi yglan etmegi yglan edilen ülňileriň sanawyna goşýan ýer bolsa,
    şol ýerde-de çagyrylar ýaly edilýär.

    Funksiýa-da
        Eger komandanyň görnüşi birsyhly sanawy yglan etmek bolmasa ýa gutarylan bolmasa
            funksiýadan çykylýar.
        Ýogsa
            Eger komanda lokal bolsa:
                1) Lokal yglan edilen birsyhly sanawlaryň sanyny ýene bire köpeldýär.
                   (Lokal yglan edilen sanawlaryň sanyny, we içindeligini saklajak maglumatlaryň sanawyny köpeldýär.)
                2) Iň soňky lokal yglan edilen sanawa maglumatlar goşulýar.
                3) Iň soňky içindeliginiň maglumatlaryny saklaýan sanawyň elementine:
                    Komandanyň ýaýynyň içine deň bolýança FOR gaýtalama bilen:
                        Täze element goşulýar.
                        Elemente içindelginiň sany, ýaýdaky içindelgine gabat gelýän ýerdäki maglumat goýulýar.
            Ýogsa
                1) Global yglan edilen birsyhly sanawlaryň sanyny ýene bire köpeldýär.
                   (Global yglan edilen sanawlaryň sanyny, we içindeligini saklajak maglumatlaryň sanawyny köpeldýär.)
                2) Iň soňky global yglan edilen sanawa maglumatlar goşulýar.
                3) Iň soňky içindeliginiň maglumatlaryny saklaýan sanawyň elementine:
                    Komandanyň ýaýynyň içine deň bolýança FOR gaýtalama bilen:
                        Täze element goşulýar.
                        Elemente içindelginiň sany, ýaýdaky içindelgine gabat gelýän ýerdäki maglumat goýulýar.
*/

/** ŞERT #8: Beýän edilýän faýllarda bolup bilýän komandalaryň sanawyna ýene biri goşulýar.
    Olam birsyhly sanawlary yglan etmek.

    Haçanda pragma bölümde, birsyhly sanawyň yglan etmegine duşylan wagty,
    Global yglan edilen birsyhly sanawlaryň sanawynda, identifikatory gözlenýär.
    Eger tapylmasa, onda
        identifikatoryň öň ulanylandygy barlanýar.
        Eger ulanylan bolsa:
            "Barlagda ulanyljak bolan identifikator eýýäm ulanylan" diýen ýalňyşlyk görkezilýär.
        Ýogsa
            1) Yglan edilen birsyhly sanawlaryň beýan edilmeleriniň sanyny ýene bire köpeldýär.
              (Yglan edilen sanawlaryň beýan edilmeleriniň  sanyny, we içindeligini saklajak maglumatlaryň sanawyny köpeldýär.)
            2) Iň soňky yglan edilen beýan edilmeleriniň sanawyndaky elemente maglumatlar goşulýar.
            3) Iň soňky içindeliginiň maglumatlaryny saklaýan beýan edilmeleriniň sanawynyň elementine:
                Komandanyň ýaýynyň içine deň bolýança FOR gaýtalama bilen:
                    Täze element goşulýar.
                    Elemente içindelginiň sany, ýaýdaky içindelgine gabat gelýän ýerdäki maglumat goýulýar.
*/

/** ŞERT #9: Birsyhly sanawy çagyrmagyň komandasyny tanaýan funksiýa:
    Eger birinji komanda birligi birlik (şu bölümde ýöne birlik) global diýen birlik bolsa,
        Indiki elementiň sanawy 1 diýip goýulýar.
        Komanda global diýilýär.
    Ýogsa
        Indiki elementiň sanawy 0 diýip goýulýar.
        Komanda lokal diýilýär.

    ** IDENT
    Eger komanda-da indiki görmeli birlige çenli birlikler bar bolsa
        Eger indiki birlik identifikator bolmasa
            yzyna ŞOWSUZLYK gaýtarylýar.
        Ýogsa
            Komandanyň maglumatlaryny üýtgedýän funksiýa çagyrylýar. (çmd, 0).
            Indiki elementi barlamak üçin san ulalýar.

    ** SAN
    Eger komanda-da indiki görmeli birlige çenli birlikler bar bolsa
        Her komandanyň indiki birliginiň sanyndan başlap tä soňkusyna çenli:
            Eger indiki birlik tip bolmasa
                yzyna ŞOWSUZLYK gaýtarylýar.
            Ýogsa
                Komandanyň maglumatlaryny üýtgedýän funksiýa çagyrylýar. (çmd, 1>=).
                (Komandanyň maglumatlaryny üýtgedýän funksiýa-da: eger element iň soňky bolsa:
                    onda, komandanyň ähli tiplerini goşup, hemme tipleriň deregine
                    diňe bir söz tokeni ýasalýar. Söz tokeniniň maglumaty diýip: tipler '_' arkaly
                    bölünip goýulýar.)
                Indiki elementi barlamak üçin san ulalýar.
*/

/**  ŞERT #10: Birsyhly sanawy çagyrmagyň C kodyny ýazýan bölek:
        identifikator ýalyýar,
        Soňky birlikdäki söz maglumat '_' boýunça bölünip, her bölünen jogap boýunça:
            dörtburç ýaý we içinde san ýazylýar.

        Eger kod Ç faýlynda ýazylýan we tipiň klasy ulanyjy tarapyndan ýasalmadyk bolsa bolsa:
            deňdir ýazylýar,
            tipiň başlangyç maglumaty ýazylýar.
*/

/** ŞERT #11: Birsyhly sanawy çagyrmagyň semantikasyny barlaýan funksiýa
    Eger komanda global bolsa:
        Eger identifikator global yglan edilen birsihly sanawlaryň ýa beýan edilen birsyhly sanawlaryň arasynda bolmsa:
            "Näbelli birsyhly sanaw çagyryldy" diýen ýalňyşlyk görkezilýär.
        Ýogsa
            Soňky birlikdäki söz maglumat '_' boýunça bölünýär.
            Bölünen sanlaryň jemi, çagyrylan sanawyň içkiliginiň jemi diýilýär.
            Identifikator boýunça, birsyhly sanawyň sanawyndan, element alynýar.
            Eger elementdäki içkiliginiň sany jemden uly bolsa:
                "Birsyhly sanawyň soňky däl elementine çatlaşyk boldy, diňe soňky elementine çatlaşyp bolýar" diýen ýalňyşlyk görkezilýär.
            Ýa elementäki içkiliginiň sany jemden kiçi bolsa:
                "Birsyhly sanawyň içkiliginde bolmadyk elemente goşulma boldy" diýen ýalňyşlyk görkezilýär.
            Identifikator boýunça, birsyhly sanawyň içgiliniň maglumatlarynyň sanawyndan, element alynýar.
            Tä jemi bolýança, birden başlap:
                Eger '_' bölünen söz sanaw öwrülip, içkiliginiň maglumatlarynyň elementindäki bolup biljek birlikleriň sanyndan uly bolsa
                    "Birsyhly sanawyň birligi bolmadyga çatylýar" diýen ýalňyşlyk görkezilýär.

*/

/** ŞERT #12: Birsyhly sanawy çagyrmak maglumat gaýtarýar.
    Maglumatyny gaýtarýan funksiýa-da:
    Eger komanda gutarylan bolsa:
        Eger komanda global bolsa
            ikinji birligi alynýar.
            Alnan birlige görä, global yglan edilen ülňileriň arasyndan birlik alynýar
        ýogsa
            birinji birligi alynýar.
            Alynan birlige görä, lokal yglan edilen ülňileriň arasyndan birlik alynýar.
        Alnan birligiň tipi we klasy gaýtarylýar.
        Şowlylyk gaýtarylýar.
    Ýogsa
        Şowsuzlyk gaýtarylýar.
*/
#include <string.h>
#include <stdlib.h>
#include "array.h"
#include "user_def_type.h"
#include "../translator_to_c/includes.h"
#include "../cmds.h"
#include "../paren/types.h"
#include "../parenthesis.h"
#include "../main/glob.h"
#include "../token/harpl.h"
#include "../error.h"
#include "../fns.h"
#include "../main/user_def_type.h"

int CMD_CLASS_ARR_DEF = 0;
int CMD_CLASS_ARR_CON = 1;

int is_cmd_arr(command *cmd)
{
    if (!cmd->items_num)
        return 0;
    command_item *fci = get_cmd_item(cmd->items, 0);
    int next_item = 0;

    if (fci->type==TOKEN_ITEM && fci->tok.potentional_types[0].type_class==TOK_CLASS_GLOB)
    {
        cmd->ns = GLOB;
        ++next_item;
    }
    else
    {
        cmd->ns = LOCAL;
    }

    if (next_item+1<=cmd->items_num)
    {
        command_item *ci = get_cmd_item(cmd->items, next_item);
        if (ci->type==PAREN_ITEM && ci->paren.type==PAREN_TYPE_FNS_ARGS && is_param_item_int(&ci->paren))
        {
            ++next_item;
            debug_paren(&ci->paren);
            cmd_arr_def_mod(cmd, 0);
        }
        else if ((ci->type==TOKEN_ITEM && ci->tok.potentional_types[0].type_class==TOK_CLASS_IDENT) ||
                 (ci->type==CMD_ITEM   && ci->cmd.cmd_class==CMD_CLASS_UTYPE && ci->cmd.cmd_type==CMD_UTYPE_CON_TYPE) ||
                 (ci->type==CMD_ITEM   && ci->cmd.cmd_class==CMD_CLASS_ARR   && ci->cmd.cmd_type==CMD_CLASS_ARR_CON))
        {
            ++next_item;
            cmd_arr_con_mod(cmd, 0);
        }
        else
        {
            return 0;
        }
    }

    if (next_item+1<=cmd->items_num)
    {
        if (cmd->cmd_type==CMD_CLASS_ARR_DEF)
        {
            command_item *sci = get_cmd_item(cmd->items, next_item);
            if (sci->type==TOKEN_ITEM && (sci->tok.potentional_types[0].type_class==TOK_CLASS_DEF_TYPE ||
                                          sci->tok.potentional_types[0].type_class==TOK_CLASS_UTYPE_CON) )
            {
                ++next_item;
                cmd_arr_def_mod(cmd, 1);
            }
            else
            {
                return 0;
            }
        }
        else if (cmd->cmd_type==CMD_CLASS_ARR_CON)
        {
            int i, type_class = -1, type_num = -1;

            /** ŞERT #15: Eger ikinji elementiň görnüşi HARPL identifikatory bolsa
                Onda next_item bir san ulalýar.

                ** Komandanyň maglumatlaryny üýtgedýän funksiýada
                Eger ikinji elementi üýtgetmeli diýen komanda bolsa:
                    Eger ikinji birlik HARPL görnüşde bolsa,
                        Onda, üçünji birlikden |SAN diýen sözlem ýasalýar.
                        Sözlem iň Global HARPL-laryň sanawyndaky iň soňky birligiň içine salynýar.
                    Ýogsa
                        Täze HARPL görnüşde token ýasalýar.
                        Içine ikinji birlikdäki maglumat salynýar.
                        Täze ýasalan token, komanda-daky ikinji birligiň deregine ulanylýar.
            */
            command_item *sci = get_cmd_item(cmd->items, next_item);
            if (sci->type==TOKEN_ITEM && sci->tok.type_class==TOK_CLASS_CONST_DATA &&
                sci->tok.potentional_types[0].type_num==STRING_CONST_DATA_TOK_NUM)
                    ++next_item;

            for (i=next_item; i<cmd->items_num; ++i)
            {
                command_item *ci = get_cmd_item(cmd->items, i);

                if ((ci->type==TOKEN_ITEM && return_tok_type(&ci->tok, &type_class, &type_num)) ||
                    (ci->type==CMD_ITEM && CMD_RETURN_TYPE[ci->cmd.cmd_class][ci->cmd.cmd_type](&ci->cmd, &type_class, &type_num)) ||
                    (ci->type==PAREN_ITEM && PAREN_RETURN_TYPE[ci->paren.type](&ci->paren, &type_class, &type_num)))
                {
                    if (type_class!=TOK_CLASS_DEF_TYPE && type_num!=INT_CONST_DATA_TOK_NUM)
                        return 0;
                    else
                    {
                        type_class = type_num = -1;
                    }
                }
                else
                {
                    return 0;
                }
            }
            if (!(sci->type==TOKEN_ITEM && sci->tok.type_class==TOK_CLASS_CONST_DATA &&
                sci->tok.potentional_types[0].type_num==STRING_CONST_DATA_TOK_NUM))
                ++next_item;
            cmd_arr_con_mod(cmd, 1);

        }
    }

    if (next_item+1<=cmd->items_num)
    {
        if (cmd->cmd_type==CMD_CLASS_ARR_DEF)
        {
            command_item *tci = get_cmd_item(cmd->items, next_item);
            if (tci->type==TOKEN_ITEM && tci->tok.potentional_types[0].type_class==TOK_CLASS_IDENT)
            {
                ++next_item;
                cmd_arr_def_mod(cmd, 2);
            }
        }
        else if (cmd->cmd_type==CMD_CLASS_ARR_CON)
        {
            /// GATY KAN BIRLIKLER BAR
            /// TODO "Komanda-da artykmaç birlik bar" diýen ýalňyşlyk görkezilýär.
            return 0;
        }
    }

    if (next_item+1<=cmd->items_num)
    {
        /// TODO "Komanda-da artykmaç birlik bar" diýen ýalňyşlyk görkezilýär.
        return 0;
    }
    return 1;
}


void cmd_arr_def_mod(command *cmd, int item_num)
{
    int item_pos = item_num;
    if (cmd->ns==LOCAL)
    {
        item_pos = item_num+1;
    }
    if (item_num==0)
    {
        cmd->cmd_class = CMD_CLASS_ARR;
        cmd->cmd_type  = CMD_CLASS_ARR_DEF;
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


void cmd_arr_con_mod(command *cmd, int item_num)
{

    int item_pos = item_num;
    if (cmd->ns==GLOB)
    {
        item_pos = item_num+1;
    }
    if (item_num==0)
    {
        cmd->cmd_class = CMD_CLASS_ARR;
        cmd->cmd_type  = CMD_CLASS_ARR_CON;
        cmd->is_compl  = 0;
    }
    else if (item_num==1)
    {
        /** Eger ikinji elementiň görnüşi HARPL identifikatory bolsa
                Onda next_item bir san ulalýar.

                ** Komandanyň maglumatlaryny üýtgedýän funksiýada
                Eger ikinji elementi üýtgetmeli diýen komanda bolsa:
                    Eger ikinji birlik HARPL görnüşde bolsa,
                        Onda, üçünji birlikden |SAN diýen sözlem ýasalýar.
                        Sözlem iň Global HARPL-laryň sanawyndaky iň soňky birligiň içine salynýar.
                    Ýogsa
                        Täze HARPL görnüşde token ýasalýar.
                        Içine ikinji birlikdäki maglumat salynýar.
                        Täze ýasalan token, komanda-daky ikinji birligiň deregine ulanylýar. */
        cmd->is_compl = 1;
        int i, len = 0;

        command_item *sci = get_cmd_item(cmd->items, item_pos);
        if (sci->type==TOKEN_ITEM && sci->tok.type_class==TOK_CLASS_CONST_DATA &&
                sci->tok.potentional_types[0].type_num==STRING_CONST_DATA_TOK_NUM)
        {
            // item_pos 0-dan başlaýar, items_num bolsa 1-den. Şonuň üçin item_pos+1
            if (cmd->items_num>item_pos+1)
            {
                command_item *tci = get_cmd_item(cmd->items, cmd->items_num-1);
                char *line = NULL;
                char *separator = "|";
                len += strlen(separator)+1;
                line = realloc(line, len);

                strncpy(line, separator, strlen(separator)+1);

                TOK_GET_C_CODE[tci->tok.potentional_types[0].type_class][tci->tok.potentional_types[0].type_num]
                (&tci->tok, &line, &len);

                add_string_to_last_string(line);
                free(line);

                cmd->items_num--;
                change_cmd_items_num(cmd->items, cmd->items_num);
            }
        }
        else
        {
            token string_tok;
            string_prepare(&string_tok);
            free_last_string();

            char *last = NULL;

            command_item *ci = get_cmd_item(cmd->items, cmd->items_num-1);

            TOK_GET_C_CODE[ci->tok.potentional_types[0].type_class][ci->tok.potentional_types[0].type_num]
                    (&ci->tok, &last, &len);
/*
            char *separator = "|";
            len += strlen(separator);
            last = realloc(last, len);
            strncat(last, separator, strlen(separator));*/

            add_string_to_last_string(last);
            free(last);

            string_tok.is_compl = 1;
            string_tok.potentional_types[0].is_compl = 1;

            command_item put_me;
            put_me.type = TOKEN_ITEM;
            put_me.tok = string_tok;

            put_cmd_item(cmd->items, cmd->items_num-1, put_me);
        }

    }
}


int cmd_arr_con_return_type(command *cmd, int *type_class, int *type_num)
{
    if (!cmd->is_compl)
        return 0;

        int type = -1, num = -1, ident_pos = 1;

        if (cmd->ns==LOCAL)
            ident_pos = 0;
        command_item *ci = get_cmd_item(cmd->items, ident_pos);
        char *ident = ci->tok.potentional_types[0].value;

        get_arr_address_by_ident(ident, &type, &num);
        if (type==-1 || num==-1 )
        {
            return 0;
        }

        array_item *ai  = NULL;
        if (type==0)
        {
            ai = &GLOBAL_ARR_DEFS[num];
        }
        else if (type==1)
        {
            ai = &LOCAL_ARR_DEFS[num];
        }
        else
        {
            ai = &GLOBAL_ARR_DECS[num];
        }


        *type_class = ai->type_class;
        *type_num = ai->type_num;
        set_def_type_alias_const_data(type_class, type_num);
        return 1;
}


int semantic_cmd_arr_con(command *cmd)
{
    int ident_num, elems_num = -1;
    if (cmd->ns==GLOB)
    {
        ident_num = 1;
    }
    else
    {
        ident_num = 0;
    }

    command_item *ident_ci = get_cmd_item(cmd->items, ident_num);
    command_item *elems_ci = get_cmd_item(cmd->items, ident_num+1);

    char *elems = get_string_item(elems_ci->tok.potentional_types[0].string_value);

    int type = -1, num = -1, utype_addr = -1, utype_item_addr = -1;
    ///
    if (ident_ci->type==CMD_ITEM && ident_ci->cmd.cmd_class==CMD_CLASS_UTYPE && ident_ci->cmd.cmd_type==CMD_UTYPE_CON_TYPE)
    {
        if (get_utype_addr_by_cmd(&ident_ci->cmd, &utype_addr) && get_utype_item_addr_by_cmd(&ident_ci->cmd, &utype_item_addr))
        {
            type = 3;
            if (!is_utype_arr_item(utype_addr, utype_item_addr))
            {
                printf("%s %d: Ulanyjynyň ýasan tipiniň sanaw däl ülňisine çatylyndy\n", __FILE__, __LINE__);

            }
            else
            {
                num = USER_DEF_TYPE_ITEMS[utype_addr][utype_item_addr].arr_items_addr;
            }
        }
    }
    else
    {
        get_arr_address_by_ident(ident_ci->tok.potentional_types[0].value, &type, &num);
    }

    if (type==-1 || num==-1)
    {
        CUR_PART = 7;
        print_err(CODE7_ARRAY_DOESNT_EXIST, (token *)inf_get_last_token(cmd));
    }

    array_item *ai = NULL;
    if (type==0)
    {
        // GLOBAL YGLAN EDILEN SANAW EKEN
        ai = &GLOBAL_ARR_DEFS[num];
    }
    else if (type==1)
    {
        // LOKAL YGLAN EDILEN SANAW EKEN
        ai = &LOCAL_ARR_DEFS[num];
    }
    else if (type==2)
    {
        // BASHGA KODLY FAYLDA YGLAN EDILEN SANAW EKEN
        ai = &GLOBAL_ARR_DECS[num];
    }
    else if (type==3)
    {
        ai = &USER_DEF_TYPES_ARRS[utype_addr][num];
    }


    char delims = '|';
    char **items = NULL; // INT tipdäki maglumatlaryň bolup biljek ulylygy
    int items_num = 0;

    divide_string(elems, delims, &items, &items_num);

    if (ai->incs>items_num)
    {
        if (items_num)
        {
            int i;
            for (i=0; i<items_num; ++i)
            {
                free(items[i]);
            }
            free(items);
        }
        CUR_PART = 7;
        print_err(CODE7_UNDEFINED_ARRAY_INCLUDE, (token *)inf_get_last_token(cmd));
    }
    else if (ai->incs<items_num)
    {
        if (items_num)
        {
            int i;
            for (i=0; i<items_num; ++i)
            {
                free(items[i]);
            }
            free(items);
        }
        CUR_PART = 7;
        print_err(CODE7_CONNECTING_TO_ARRAY_NOT_TO_ITEM, (token *)inf_get_last_token(cmd));
    }
    else
    {
        int i;
        for (i=0; i<items_num; ++i)
        {
            array_inc_item *aii = NULL;
            if (type==0)
            {
                // GLOBAL YGLAN EDILEN SANAW EKEN
                aii = &GLOBAL_ARR_DEFS_ITEMS[num][i];
            }
            else if (type==1)
            {
                // LOKAL YGLAN EDILEN SANAW EKEN
                aii = &LOCAL_ARR_DEFS_ITEMS[num][i];
            }
            else if (type==2)
            {
                // BASHGA KODLY FAYLDA YGLAN EDILEN SANAW EKEN
                aii = &GLOBAL_ARR_DECS_ITEMS[num][i];
            }
            else if (type==3)
            {
                aii = &USER_DEF_TYPES_ARRS_ITEMS[utype_addr][num][i];
            }

            if (atoi(items[i])>aii->elem_num)
            {
                if (items_num)
                {
                    int i;
                    for (i=0; i<items_num; ++i)
                    {
                        free(items[i]);
                    }
                    free(items);
                }
                CUR_PART = 7;
                print_err(CODE7_OUT_OF_ARRAY_ITEMS, (token *)inf_get_last_token(cmd));
            }
        }
        if (items_num)
        {
            int i;
            for (i=0; i<items_num; ++i)
            {
                free(items[i]);
            }
            free(items);
        }
    }

}

int semantic_cmd_arr_def(command *cmd)
{
    if (cmd->ns==GLOB && GLOB_BLOCK_INCLUDES)
    {
        CUR_PART = 7;
        print_err(CODE7_GLOB_DEF_IN_BLOCK, (token *)inf_get_last_token(cmd));
    }
    return 1;
}

/** Faýla degişli kody C koda ýazýar
**/
void cmd_arr_con_c_code(command *cmd, char **line, int *line_len)
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

    command_item *sci = get_cmd_item(cmd->items, cmd->items_num-1);
    char *elems = get_string_item(sci->tok.potentional_types[0].string_value);

    char delims = '|';
    char **items = NULL; // INT tipdäki maglumatlaryň bolup biljek ulylygy
    int items_num = 0;

    divide_string(elems, delims, &items, &items_num);

    char *o = "[";
    char *c = "]";

    int i;
    for (i=0; i<items_num; ++i)
    {
        *line_len += strlen(o);
        *line = realloc(*line, *line_len);
        strncat(*line, o, strlen(o));

        *line_len += strlen(items[i]);
        *line = realloc(*line, *line_len);
        strncat(*line, items[i], strlen(items[i]));

        *line_len += strlen(c);
        *line = realloc(*line, *line_len);
        strncat(*line, c, strlen(c));
    }

    free(items);
}


void add_to_last_loc_arr_items(command *cmd)
{
    int yay_pos = cmd->items_num-3;

    command_item *ci = get_cmd_item(cmd->items, yay_pos);

    parenthesis *p = &ci->paren;

    int i;

    LOCAL_ARR_DEFS_ITEMS[LOCAL_ARR_DEFS_NUMS-1] = realloc(LOCAL_ARR_DEFS_ITEMS[LOCAL_ARR_DEFS_NUMS-1], sizeof(**LOCAL_ARR_DEFS_ITEMS)*p->elems_num);
    array_inc_item **last = &LOCAL_ARR_DEFS_ITEMS[LOCAL_ARR_DEFS_NUMS-1];

    parenthesis_elem *pes = get_paren_elems(p->elems);

    for(i=0; i<p->elems_num; ++i)
    {
        array_inc_item add = {LOCAL_ARR_DEFS_NUMS-1, i, atoi(pes[i].tok.potentional_types[0].value)};

        (*last)[i] = add;
    }
}

void add_to_last_tmp_arr_items(command *cmd)
{
    int yay_pos = cmd->items_num-3;

    command_item *ci = get_cmd_item(cmd->items, yay_pos);

    parenthesis *p = &ci->paren;

    int i;

    TMP_USER_DEF_TYPES_ARRS_ITEMS[TMP_USER_DEF_TYPES_ARRS_NUMS-1] = realloc(TMP_USER_DEF_TYPES_ARRS_ITEMS[TMP_USER_DEF_TYPES_ARRS_NUMS-1], sizeof(**TMP_USER_DEF_TYPES_ARRS_ITEMS)*p->elems_num);
    array_inc_item **last = &TMP_USER_DEF_TYPES_ARRS_ITEMS[TMP_USER_DEF_TYPES_ARRS_NUMS-1];

    parenthesis_elem *pes = get_paren_elems(p->elems);

    for(i=0; i<p->elems_num; ++i)
    {
        array_inc_item add = {TMP_USER_DEF_TYPES_ARRS_NUMS-1, i, atoi(pes[i].tok.potentional_types[0].value)};

        (*last)[i] = add;
    }
}


void add_to_last_glob_arr_items(command *cmd)
{
    int yay_pos = cmd->items_num-3;

    command_item *ci = get_cmd_item(cmd->items, yay_pos);

    parenthesis *p = &ci->paren;

    int i;

    GLOBAL_ARR_DEFS_ITEMS[GLOBAL_ARR_DEFS_NUMS-1] = realloc(GLOBAL_ARR_DEFS_ITEMS[GLOBAL_ARR_DEFS_NUMS-1], sizeof(**GLOBAL_ARR_DEFS_ITEMS)*p->elems_num);
    array_inc_item **last = &GLOBAL_ARR_DEFS_ITEMS[GLOBAL_ARR_DEFS_NUMS-1];

    parenthesis_elem *pes = get_paren_elems(p->elems);

    for(i=0; i<p->elems_num; ++i)
    {
        array_inc_item add = {GLOBAL_ARR_DEFS_NUMS-1, i, atoi(pes[0].tok.potentional_types[0].value)};

        (*last)[i] = add;
    }
}

void add_to_last_dec_arr_items(command *cmd)
{
    int yay_pos = cmd->items_num-3;
    command_item *ci = get_cmd_item(cmd->items, yay_pos);

    parenthesis *p = &ci->paren;

    int i;

    GLOBAL_ARR_DECS_ITEMS[GLOBAL_ARR_DECS_NUMS-1] = realloc(GLOBAL_ARR_DECS_ITEMS[GLOBAL_ARR_DECS_NUMS-1], sizeof(**GLOBAL_ARR_DECS_ITEMS)*p->elems_num);
    array_inc_item **last = &GLOBAL_ARR_DECS_ITEMS[GLOBAL_ARR_DECS_NUMS-1];

    parenthesis_elem *pes = get_paren_elems(p->elems);

    for(i=0; i<p->elems_num; ++i)
    {
        array_inc_item add = {GLOBAL_ARR_DECS_NUMS-1, i, atoi(pes[0].tok.potentional_types[0].value)};
        (*last)[i] = add;
    }
}

void get_arr_address_by_ident(char *ident, int *type, int *num)
{
    int i, len = strlen(ident);
    // GLOBAL YGLAN EDILEN SANAWLARYN ARASYNDAN GOZLENILYAR:
    for (i=0; i<LOCAL_ARR_DEFS_NUMS; ++i)
    {
        if (len==strlen(LOCAL_ARR_DEFS[i].ident) && strncmp(ident, LOCAL_ARR_DEFS[i].ident, len)==0)
        {
            *type = 1;
            *num  = i;
            return;
        }
    }
    for (i=0; i<GLOBAL_ARR_DEFS_NUMS; ++i)
    {
        if (len==strlen(GLOBAL_ARR_DEFS[i].ident) && strncmp(ident, GLOBAL_ARR_DEFS[i].ident, len)==0)
        {
            *type = 0;
            *num  = i;
            return;
        }

    }
    for (i=0; i<GLOBAL_ARR_DECS_NUMS; ++i)
    {
        if (len==strlen(GLOBAL_ARR_DECS[i].ident) && strncmp(ident, GLOBAL_ARR_DECS[i].ident, len)==0)
        {
            *type = 2;
            *num  = i;
            return;
        }
    }
}


/** Ýasalan kodda başga faýllarda yglan edilen global ülňileriň çagyrylanlarynyň sanawyna täze ülňini goşýar
    Bu sanaw C kody üçin bölümde gerek. Sebäbi ýasalan kodda çagyrylan ülňiniň yglan edilen ýeriniň .h hem goşulmaly.
*/
void global_called_arrs_add(command *cmd)
{
    if (!cmd->is_compl || !(cmd->cmd_class==CMD_CLASS_ARR && cmd->cmd_type==CMD_CLASS_ARR_CON && cmd->ns==GLOB))
        return;
    command_item *ci = get_cmd_item(cmd->items, 1); // lci - last command item
    int  *fnum = &ci->tok.inf_file_num;
    char *ident= ci->tok.potentional_types[0].value;

    /// Eger ülňi yglan edilen faýlynda çagyrylan bolsa, onda ülňiniň yglan edilen .h faýly eýýäm inklud edildi.
    int i, len;
    for(i=0; i<GLOBAL_ARR_DEFS_NUMS; ++i)
    {
        if ((strlen(GLOBAL_ARR_DEFS[i].ident)==strlen(ident) &&
            strncmp(GLOBAL_ARR_DEFS[i].ident, ident, strlen(ident))==0) &&
            *fnum==GLOBAL_ARR_DEFS[i].inf_file_num)
            return;
    }

    if (*fnum+1<=GLOBAL_CALLED_ARRS_NUM)
    {
        called_var *cv = &GLOBAL_CALLED_ARRS[*fnum];

        /// Eger eýýäm şeýle ülňi öňem çagyrylan bolsa, onda ikinji gezek goşmak nämä gerek?
        for(i=0; i<cv->num; ++i)
        {
            if (strlen(cv->ident[i])==strlen(ident) && strncmp(cv->ident[i], ident, strlen(ident)==0))
                return;
        }

        cv->num++;
        cv->ident = realloc(cv->ident, sizeof(*cv->ident)*cv->num);

        strncpy(cv->ident[cv->num-1], ident, strlen(ident)+1);
    }
    else
    {
        /// Eger çagyrylan ülňiniň faýlynda intäk hiç hili ülňi çagyrylmadyk bolsa
        /// Ýasaljak kody çagyrylan ülňileri bolan faýllaryň sanawyna goşulýar
        ++GLOBAL_CALLED_ARRS_NUM;

        GLOBAL_CALLED_ARRS = realloc(GLOBAL_CALLED_ARRS, sizeof(*GLOBAL_CALLED_ARRS)*GLOBAL_CALLED_ARRS_NUM);
        called_var newf;
        newf.ident = NULL;
        newf.num   = 0;
        GLOBAL_CALLED_ARRS[GLOBAL_CALLED_ARRS_NUM-1] = newf;
        GLOBAL_CALLED_ARRS[GLOBAL_CALLED_ARRS_NUM-1].ident = malloc(sizeof(*GLOBAL_CALLED_ARRS[GLOBAL_CALLED_ARRS_NUM-1].ident));
        GLOBAL_CALLED_ARRS[GLOBAL_CALLED_ARRS_NUM-1].num   = 1;

        strncpy(GLOBAL_CALLED_ARRS[GLOBAL_CALLED_ARRS_NUM-1].ident[0], ident, strlen(ident)+1);
    }
}

/// Ýasalan kodda çagyrylan global ülňileriň yglan edilen .h faýlynyň çagyrylan ýerinde inklud etmeli
void work_with_called_glob_arrs()
{
    int i, j;
    file_incs *fi = NULL;
    for (i=0; i<GLOBAL_CALLED_ARRS_NUM; ++i)
    {
        if (i+1>INCLUDES_NUM)
        {
            fi = includes_add_new();
        }
        else
        {
            fi = &INCLUDES[i];
        }

        for(j=0; j<GLOBAL_CALLED_ARRS[i].num; ++j)
        {
            array_item *gi = glob_arrs_def_get_by_name(GLOBAL_CALLED_ARRS[i].ident[j]);
            char arr_def_f[MAX_FILE_LEN] = {0};
            strncpy(arr_def_f, "\"", strlen("\"")+1);
            strncat(arr_def_f, FILES[gi->inf_file_num].name, strlen(FILES[gi->inf_file_num].name));
            strncat(arr_def_f, ".h", strlen(".h"));
            strncat(arr_def_f, "\"", strlen("\""));
            //printf("%s\n", var_def_f);

            includes_file_add_include(fi, arr_def_f);
        }
    }
}


void add_arr_elem_inf_c_code(char **line, int *line_len, int arr_type, int arr_num, int max_items)
{
    char *o = "[";
    char *c = "]";

    int i;
    array_inc_item *aii = NULL;
    if (arr_type==0)
    {
        aii = GLOBAL_ARR_DEFS_ITEMS[arr_num];
    }
    else if (arr_type==1)
    {
        aii = LOCAL_ARR_DEFS_ITEMS[arr_num];
    }
    else if (arr_type==2)
    {
        aii = GLOBAL_ARR_DECS_ITEMS[arr_num];
    }

    for (i=0; i<max_items; ++i)
    {
        *line_len += strlen(o);
        *line = realloc(*line, *line_len);
        strncat(*line, o, strlen(o));

        array_inc_item *aii_e = &aii[i];
        char val[21]; // SAN tipde maksimum ulanyp boljak sanyn uzynlygy
        sprintf(val, "%d", aii_e->elem_num);
        *line_len += strlen(val);
        *line = realloc(*line, *line_len);
        strncat(*line, val, strlen(val));

        *line_len += strlen(c);
        *line = realloc(*line, *line_len);
        strncat(*line, c, strlen(c));

    }
}

