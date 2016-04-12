#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_def_type.h"
#include "../tpl.h"
#include "inf.h"
#include "../parser.h"
#include "../cmd/user_def_type.h"
#include "../translator_to_c.h"
#include "../error.h"
#include "../pragma/pragma.h"
#include "../token/harpl.h"
#include "../fns.h"
#include "conv_basic_types.h"

int swprintf (wchar_t*, const wchar_t*, ...);

wchar_t             *USER_DEF_TYPES_SOURCE_NAME_NO_H = L"_tpl_utypes";
wchar_t             *USER_DEF_TYPES_SOURCE_NAME      = L"_tpl_utypes.h";

utype_type       *USER_DEF_TYPES = NULL;
utype_item_type **USER_DEF_TYPE_ITEMS = NULL;
int               USER_DEF_TYPES_NUM = 0;

array_inc_item ***USER_DEF_TYPES_ARRS_ITEMS = NULL;
array_item      **USER_DEF_TYPES_ARRS = NULL;
int              *USER_DEF_TYPES_ARRS_NUMS = NULL;


utype_item_type  *TMP_USER_DEF_TYPE_ITEMS = NULL;
int               TMP_USER_DEF_TYPES_NUM = 0;
array_inc_item  **TMP_USER_DEF_TYPES_ARRS_ITEMS = NULL;
array_item       *TMP_USER_DEF_TYPES_ARRS = NULL;
int               TMP_USER_DEF_TYPES_ARRS_NUMS = 0;


int get_utype_addr(wchar_t *ident, int *utype_addr)
{
    int i, len = wcslen(ident), utype_len = 0;
    for (i=0; i<USER_DEF_TYPES_NUM; ++i)
    {
        utype_len = wcslen(USER_DEF_TYPES[i].ident);
        if (utype_len==len && wcsncmp(ident, USER_DEF_TYPES[i].ident, len)==0)
        {
            *utype_addr = i;
            return 1;
        }
    }
    return 0;
}

/// Tapylsa 1, tapylmasa 0-lyk ugradýar.
int get_utype_item_addr(int utype_num, wchar_t *item_ident, int *utype_item_addr)
{
    int i, item_len = wcslen(item_ident), utypeitem_len = -1;
    for (i=0; i<USER_DEF_TYPES[utype_num].items_num; ++i)
    {
        utypeitem_len = wcslen(USER_DEF_TYPE_ITEMS[utype_num][i].ident);
        if (item_len==utypeitem_len &&
            wcsncmp(item_ident, USER_DEF_TYPE_ITEMS[utype_num][i].ident, item_len)==0)
        {
            *utype_item_addr=i;
            return 1;
        }
    }
    return 0;
}

int is_utype_arr_item(int utype_num, int item_num)
{
    return !(USER_DEF_TYPE_ITEMS[utype_num][item_num].arr_items_addr==-1);
}

/** Komanda - UG-yň birligine çatylma bolmaly. Funksiýa birinji birlige görä, UG-yň tipini tanap, yglan edilen
    tipleriň sanawyndaky nomerini gaýtarýar.

*/
int get_utype_addr_by_cmd(command *cmd, int *utype_addr)
{
    int tmp_type=-1, tmp_class=-1;
    command_item *fci = get_cmd_item(cmd->items, 0);
    command_item *tci = get_cmd_item(cmd->items, 2);
    token        *tok = &tci->tok;
    if (fci->type==CMD_ITEM)
    {
        /// MESELEM: ug1 "adam" "süýdemriji"
        /// Eger Çatylynýan UG, başga bir UG-yň birliginiň tipi bolsa,
        ///     get_utype_item_type_by_cmd(fçi->çmd, tmp_çlass, tmp_type) diýen funksiýa arkaly elementiň tipi alynýar.
        ///     Eger funksiýa şowsuzlygy gaýtarsa:
        ///         "Ata birliginiň tipini tanap bolmady" ýalňyşlygy gaýtarylýar.
        if (fci->cmd.cmd_class==CMD_CLASS_UTYPE && fci->cmd.cmd_type==CMD_UTYPE_CON_TYPE)
        {
            if (!get_utype_item_type_by_cmd(&fci->cmd, &tmp_class, &tmp_type))
                printf("%s %d setirde: ATA birligini[ tipini tanap bolmady\n", __FILE__, __LINE__);
        }
        /** MESELEM: ug1 0 "nolynjy"
            Eger çatylynýan UG, sanawyň birligi bolsa:
                get_arr_item_type_by_çmd(fçi->çmd, tmp_çlass, tmp_type) diýen funksiýa arkaly elementiň tipi alynýar.
                Eger funksiýa şowsuzlyk gaýtarsa:
                    "Ata birliginiň tipini tanap bolmady" ýalňyşlygy görkezilýär.
        */
        else if (fci->cmd.cmd_class==CMD_CLASS_ARR && fci->cmd.cmd_type==CMD_CLASS_ARR_CON)
        {
            if (!get_arr_item_type_by_cmd(&fci->cmd, &tmp_class, &tmp_type))
                printf("%s %d setirde: ATA birligini[ tipini tanap bolmady\n", __FILE__, __LINE__);
        }
        /** MESELEM @ülňi "birligi"
            Eger global ülňä çatylma bolsa:
                get_token_type() funksiýa arkaly ülňiniň tipi alynýar.
                Eger tipini alyp bolmasa:
                    "Ülňiniň UG tipini tanap bolmady" ýalňyşlygy görkezilýär.
        */
        else if (fci->cmd.cmd_class==CMD_CLASS_CALL_GLOB_VAR)
        {
            if (!CMD_RETURN_TYPE[fci->cmd.cmd_class][fci->cmd.cmd_type](&fci->cmd, &tmp_class, &tmp_type))
                printf("%s %d setirde: Ulňiniň UG tipini tanap bolmady\n", __FILE__, __LINE__);
        }
        /** Näbelli birlik. Beýle bolanok ahyry.
            "Birinji birligi tanap bolmady" ýalňyşlygy görkezilýär.
        */
        else
        {
            printf("%s %d setirde: Birinji birligi tanap bolmady\n", __FILE__, __LINE__);
        }
    }
    else if(fci->type==TOKEN_ITEM && fci->tok.type_class==TOK_CLASS_IDENT)
    {
        /** Tokeniň tipi alynýar. Eger tipi alynmasa onda:
                "Tokeniň tipini tanap bolmady" ýalňyşlygy görkezilýär.
        */
        if (!return_tok_type(&fci->tok, &tmp_class, &tmp_type))
        {
            printf("%s %d setirde: Tokeniň tipini tanap bolmady\n", __FILE__, __LINE__);
        }
    }


    /** Eger tipiň klasy UG bolmasa:
            "Sada tipdäki (san, drob, harp, harpl) ülňiniň birligine çatyljak bolundy" ýalňyşlygy görkezilýär.
        Ýogsa
            Tipiň nomeri ötürdilip, yzyna şowlylyk gaýtarylýar.
    */
    if (tmp_class!=TOK_CLASS_UTYPE_CON)
        printf("%s %d setirde: Sada tipdäki (san, drob, harp, harpl) ülňiniň birligine çatyljak bolundy\n", __FILE__, __LINE__);
    else
    {
        *utype_addr = tmp_type;
        return 1;
    }
}


/** get_utype_item_addr_by_cmd()      funksiýasy çagyrylýar.
    get_utype_addr_by_çmd() funksiýasy arkaly tipiň nomeri alynýar.

    get_utype_item_addr() diýen funksiýa arkaly tipiň birliginiň nomeri alynyp gaýtarylýar.
*/
int get_utype_item_addr_by_cmd(command *cmd, int *utype_item_addr)
{
    int tmp_num = -1;
    if (!get_utype_addr_by_cmd(cmd, &tmp_num))
        return 0;

    command_item *tci = get_cmd_item(cmd->items, 2);
    wchar_t *item_ident = tci->tok.potentional_types[0].value;
    return get_utype_item_addr(tmp_num, item_ident, utype_item_addr);
}


/** Komanda - Ug'yň birligine çatylma bolmaly. Funksiýa çatylynýan biriligiň tipini gaýtarýar
*
*   get_utype_addr_by_cmd(cmd, utype_num); funksiýa arkaly, komandanyň adresi alynýar.
*   Eger adresi alyp bolmasa,
        "UG-tipi tapyp bolmady" diýen ýalňyşlyk görkezilýär.
    get_utype_item_type(utype_num, tci->tok.pontentional_types[0].value, ret_class, ret_type) funksiýa çagyrylýar.

    Şowlylyk yzyna gaýtarylýar.

    !!
*/
int get_utype_item_type_by_cmd(command *cmd, int *ret_class, int *ret_type)
{
    int utype_num = -1;
    if (!get_utype_addr_by_cmd(cmd, &utype_num))
    {
        printf("%s %d setirde: UG-tipi tapyp bolmady\n", __FILE__, __LINE__);
        return 0;
    }

    command_item *tci = get_cmd_item(cmd->items, 2);
    wchar_t *item_ident = tci->tok.potentional_types[0].value;
    return get_utype_item_type(utype_num, item_ident, ret_class, ret_type);
}

/** UG birliginiň tipini tanaýar.

    get_utype_item_addr(utype_num, item_ident, &item_num) funksiýa çagyrylýar.
    Eger funksiýa şowsuzlyk gaýtarsa,
        "Komandanyň birligi ýok" ýalňyşlygy görkezilýär.
    UG-laryň birlikleriniň sanawlaryndan birligiň maglumatlary alynýar.
    Tip üçin ülňiler gerekli maglumatlar ötürdilýär.

    Şowlylyk gaýtarylýar.

    !!
*/
int get_utype_item_type(int utype_num, wchar_t *item_ident, int *ret_class, int *ret_type)
{
    int item_num = -1;
    if (!get_utype_item_addr(utype_num, item_ident, &item_num))
    {
        printf("%s %d setirde: Komandanyň birligi ýok\n", __FILE__, __LINE__);
        get_utype_item_addr(utype_num, item_ident, &item_num);
        return 0;
    }
    utype_item_type *uti = &USER_DEF_TYPE_ITEMS[utype_num][item_num];
    *ret_class = uti->type_class;
    *ret_type  = uti->type_num;
    return 1;
}


int get_arr_item_type_by_cmd(command *cmd, int *ret_class, int *ret_type)
{
    check_semantics(cmd);
    return CMD_RETURN_TYPE[cmd->cmd_class][cmd->cmd_type](cmd, ret_class, ret_type);
}


void add_utypes_c_code_file()
{
    wchar_t *dquote = L"\\";
    wchar_t h_path[MAX_FILE_LEN] = {0};
	wcsncat(h_path, C_SOURCE_FOLDER, wcslen(C_SOURCE_FOLDER));
	wcsncat(h_path, dquote, wcslen(dquote));
	wcsncat(h_path, USER_DEF_TYPES_SOURCE_NAME, wcslen(USER_DEF_TYPES_SOURCE_NAME));

	FILE *h_source = _wfopen(h_path, L"w, ccs=UTF-8");

	prepare_h_source(h_source, USER_DEF_TYPES_SOURCE_NAME_NO_H);

    if (USER_DEF_TYPES_NUM)
    {
        add_utype_define_c_code(h_source);
    }

	finishize_h_source(h_source);
}

void add_utype_define_c_code(FILE *f)
{
    int i, j, llen = 0, closer_len = 0;
    wchar_t *utype_opener = L"typedef struct{\n",
            *utype_closer_first = L"} ",
            *space = L" ";
    wchar_t *line = NULL;
    for (i=0; i<USER_DEF_TYPES_NUM; ++i)
    {
        // Tip açýan kodly setir
        write_code_line(f, &line, &llen , utype_opener);

        for (j=0; j<USER_DEF_TYPES[i].items_num; ++j)
        {
            wchar_t *elem_line = NULL;
            int   elem_line_len = 0;

            /// Birligiň tipi
            int type_class, type_num;
            type_class = USER_DEF_TYPE_ITEMS[i][j].type_class;
            type_num   = USER_DEF_TYPE_ITEMS[i][j].type_num;

            get_type_c_code(type_class, type_num, &elem_line, &elem_line_len);

            /// Birligiň identifikatory
            wcsadd_on_heap( &elem_line, &elem_line_len, space );

            wcsadd_on_heap( &elem_line, &elem_line_len, USER_DEF_TYPE_ITEMS[i][j].ident );

            /// Birlik sanaw bolsa, sanawyň içkiligini görkezijiler
            if (USER_DEF_TYPE_ITEMS[i][j].arr_items_addr!=-1)
            {
                wchar_t *o = L"[";
                wchar_t *c = L"]";

                int z, arr_pos = USER_DEF_TYPE_ITEMS[i][j].arr_items_addr;
                for (z=0; z<USER_DEF_TYPES_ARRS[i][arr_pos].incs; ++z)
                {
                    wcsadd_on_heap( &elem_line, &elem_line_len, o );

                    array_inc_item *aii_e = &USER_DEF_TYPES_ARRS_ITEMS[i][arr_pos][z];
                    wchar_t val[21]; // SAN tipde maksimum ulanyp boljak sanyn uzynlygy
                    swprintf(val, L"%d", aii_e->elem_num);
                    wcsadd_on_heap( &elem_line, &elem_line_len, val );

                    wcsadd_on_heap( &elem_line, &elem_line_len, c );
                }

            }

            /// Birligi gutaryjy
            get_cmd_end_c_code(&elem_line, &elem_line_len);

            write_code_line(f, &line, &llen , elem_line);
            free(elem_line);
            elem_line = NULL;
            elem_line_len = 0;
        }

        // Tipi ýapýan kodly setir
        wchar_t *closer = NULL,
                *end = L"; \n";
        closer_len = 0;

        wcsadd_on_heap( &closer, &closer_len, utype_closer_first );
        wcsadd_on_heap( &closer, &closer_len, USER_DEF_TYPES[i].ident );
        wcsadd_on_heap( &closer, &closer_len, end );

        write_code_line(f, &line, &llen , closer);
        free(closer);
        closer = NULL;
        closer_len = 0;
        // Owadanlyk we kodyň gowy okalmagy üçin
        fputws(L"\n\n", f);
    }
}

int is_reserved_source_name(wchar_t *fname)
{
    if (wcslen(fname)==wcslen(USER_DEF_TYPES_SOURCE_NAME) &&
        wcsncmp(fname, USER_DEF_TYPES_SOURCE_NAME, wcslen(USER_DEF_TYPES_SOURCE_NAME))==0)
        return 1;
    if (wcslen(fname)==wcslen(CONV_DEF_TYPES_SOURCE_NAME) &&
        wcsncmp(fname, CONV_DEF_TYPES_SOURCE_NAME, wcslen(CONV_DEF_TYPES_SOURCE_NAME))==0)
        return 1;
    return 0;
}

void add_addtn_headers(FILE *f)
{
    wchar_t *line = malloc(MAX_PREP_LEN),
            *include = L"#include \"",
            *dquote_space = L"\" \n";
    wcsncpy(line, include, wcslen(include)+get_null_size());
    wcsncat(line, USER_DEF_TYPES_SOURCE_NAME, wcslen(USER_DEF_TYPES_SOURCE_NAME));
    wcsncat(line, dquote_space, wcslen(dquote_space));

    fputws(line, f);

    free(line);

    line = malloc(MAX_PREP_LEN);
    wcsncpy(line, include, wcslen(include)+get_null_size());
    wcsncat(line, CONV_DEF_TYPES_SOURCE_NAME, wcslen(CONV_DEF_TYPES_SOURCE_NAME));
    wcsncat(line, dquote_space, wcslen(dquote_space));

    fputws(line, f);

    free(line);
}


void parse_triangle_block_inside(FILE *source)
{
    ///  Häzir üçburç bloklar diňe Ug'laryň yglan etmegine ulanylýany üçin,
    ///  diňe şol komandanyň talap edýän maglumatlary barlanýar.


/** ŞERT #24: parse_triangle_blok_inside() diýen funksiýa-da:
    ** Üçburç bloklaryň içinde pragmalar, komandany gutarýan belgi bolup bilenok.
    Eger harp tokeniň başy bolsa:
        token ýasalýar.
        Eger token üçburçlygy ýapýan bolsa:
            Eger komanda is_triangle_block_support_cmd() diýen funksiýadan geçip bilmese:
            "Komanda üçburç bloklaryň goldaýany däl" diýen ýalňyşlyk görkezilýär.
            add_to_tmp_triangle_blok_items() diýen funksiýa çagyrylýar.

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
    int prev_part = CUR_PART;
	CUR_PART = 2;
	command item; init_cmd(&item, 0);
	item.items = -2;

    // Adaty parser komandalary saýgarýar
	while( 1 )
	{
	    if ( !process_char( source, CHECK_VALID_CHAR ) )
            break;

        if ( iswspace(CUR_CHAR) ||
             parser_mode_paren( source, &item ) ||
             parser_mode_string( source, &item ) )
        {
            continue;
        }

        // 2. Pragma modyna geçmeli
        if( CUR_CHAR==PRAGMA_START_CHAR )
        {
            print_err(CODE2_PRAGMA_NOT_SUPPORT_IN_TRIANGLE_BLOCK, &inf_tok);
        }
        else if (CUR_CHAR==PARENTHESIS_ELEM_SEPARATOR)
        {
            if (!item.items_num)
            {
                print_err(CODE2_TRIANGLE_BLOCK_CANT_BE_EMPTY, &inf_tok);
            }
            else if (!is_triangle_block_support_cmd(&item))
            {
                print_err(CODE2_TRIANGLE_BLOCK_NOT_SUPPORT_CMD, (token *)inf_get_last_token(&item));
            }
            add_to_tmp_triangle_blok_items(&item);
            if (item.items_num)
                free(TMP_CMD_ITEMS_LIST);
            TMP_CMD_ITEMS_LIST = NULL;
            init_cmd(&item, 0);
            item.items = -2;
        }
        else if (CUR_CHAR==CMD_END)
        {
            print_err(CODE2_CMD_END_CHAR_NOT_SUPPORT_IN_TRIANGLE_BLOCK, &inf_tok);
        }
        else
        {
            CUR_CMD = &item;
            token tok = parse_token(source);
            // Komanda goşulýar
            if (tok.type_class==TOK_CLASS_TRIANGLE_BLOCK && tok.potentional_types[0].type_num==TOKEN_TRIANGLE_BLOCK_CLOSE_TYPE)
            {
                if (!item.items_num)
                {
                    print_err(CODE2_TRIANGLE_BLOCK_CANT_BE_EMPTY, &inf_tok);
                }
                else if (!is_triangle_block_support_cmd(&item))
                {
                    print_err(CODE2_TRIANGLE_BLOCK_NOT_SUPPORT_CMD, (token *)inf_get_last_token(&item));
                }
                add_to_tmp_triangle_blok_items(&item);
                if (item.items_num)
                    free(TMP_CMD_ITEMS_LIST);
                TMP_CMD_ITEMS_LIST = NULL;
                init_cmd(&item, 0);
                exit_triangle_block_parser();
                return ;
            }
            else
            {
                work_with_token( &tok, &item );
            }
        }

	}
	CUR_PART = prev_part;
}


void exit_triangle_block_parser()
{

}


int is_triangle_block_support_cmd(command *cmd)
{
    if (!cmd->is_compl || cmd->ns!=LOCAL)
        return 0;
    if ((cmd->cmd_class==CMD_CLASS_ARR && cmd->cmd_type==CMD_CLASS_ARR_DEF) ||
        cmd->cmd_class==CMD_CLASS_DEF_VAR)
        return 1;
    return 0;
}



void add_to_tmp_triangle_blok_items(command *cmd)
{
    ++TMP_USER_DEF_TYPES_NUM;
    TMP_USER_DEF_TYPE_ITEMS = realloc(TMP_USER_DEF_TYPE_ITEMS, sizeof(*TMP_USER_DEF_TYPE_ITEMS)*TMP_USER_DEF_TYPES_NUM);

    utype_item_type add;

    if (cmd->cmd_class==CMD_CLASS_ARR && cmd->cmd_type==CMD_CLASS_ARR_DEF)
    {
        --TMP_USER_DEF_TYPES_NUM;
        arr_def_add(cmd, -1);
        ++TMP_USER_DEF_TYPES_NUM;

        command_item *sci = get_cmd_item(cmd->items, 1);
        add.type_class = sci->tok.type_class;
        add.type_num   = sci->tok.potentional_types[0].type_num;
        command_item *tci = get_cmd_item(cmd->items, 2);
        wcsncpy(add.ident, tci->tok.potentional_types[0].value, wcslen(tci->tok.potentional_types[0].value)+1);
        add.arr_items_addr = TMP_USER_DEF_TYPES_ARRS_NUMS-1;
    }
    else
    {
        command_item *fci = get_cmd_item(cmd->items, 0);
        add.type_class = fci->tok.type_class;
        add.type_num   = fci->tok.potentional_types[0].type_num;
        command_item *sci = get_cmd_item(cmd->items, 1);
        wcsncpy(add.ident, sci->tok.potentional_types[0].value, wcslen(sci->tok.potentional_types[0].value)+1);
        add.arr_items_addr = -1;
    }

    TMP_USER_DEF_TYPE_ITEMS[TMP_USER_DEF_TYPES_NUM-1] = add;

}


void add_new_utype(command *cmd)
{
    ++USER_DEF_TYPES_NUM;
    int last = USER_DEF_TYPES_NUM-1;
    USER_DEF_TYPES = realloc(USER_DEF_TYPES, sizeof(*USER_DEF_TYPES)*USER_DEF_TYPES_NUM);
    USER_DEF_TYPE_ITEMS = realloc(USER_DEF_TYPE_ITEMS, sizeof(*USER_DEF_TYPE_ITEMS)*USER_DEF_TYPES_NUM);
    USER_DEF_TYPES_ARRS = realloc(USER_DEF_TYPES_ARRS, sizeof(*USER_DEF_TYPES_ARRS)*USER_DEF_TYPES_NUM);
    USER_DEF_TYPES_ARRS_NUMS = realloc(USER_DEF_TYPES_ARRS_NUMS, sizeof(*USER_DEF_TYPES_ARRS_NUMS)*USER_DEF_TYPES_NUM);
    USER_DEF_TYPES_ARRS_ITEMS = realloc(USER_DEF_TYPES_ARRS_ITEMS, sizeof(*USER_DEF_TYPES_ARRS_ITEMS)*USER_DEF_TYPES_NUM);

    /// Täze tipiň baş maglumatlary
    utype_type ut;
    command_item *sci = get_cmd_item(cmd->items, 1);
    wcsncpy(ut.ident, sci->tok.potentional_types[0].value, wcslen(sci->tok.potentional_types[0].value)+1);
    ut.items_num = TMP_USER_DEF_TYPES_NUM;

    USER_DEF_TYPES[last] = ut;
    /// Täze tipiň birlikleriniň maglumaty.
    USER_DEF_TYPE_ITEMS[last] = malloc(sizeof(**USER_DEF_TYPE_ITEMS)*USER_DEF_TYPES[last].items_num);
    /// Täze tipiň sanaw birlikleri barada maglumat
    USER_DEF_TYPES_ARRS[last] = NULL;
    /// Täze tipiň sanaw birlikleriniň içindäki birlikler barada maglumat
    USER_DEF_TYPES_ARRS_ITEMS[last] = NULL;
    /// Täze tipiň sanaw birlikleriniň möçberleri barada maglumat
    USER_DEF_TYPES_ARRS_NUMS[last] = 0;        /// Täze tipde 0 sany sanaw birlik bar

}

void move_tmp_utype_items_to_last()
{
    int i, j, last = USER_DEF_TYPES_NUM-1, last_arr_num, arr_addr = -1;

    for (i=0; i<USER_DEF_TYPES[last].items_num; ++i)
    {
        USER_DEF_TYPE_ITEMS[last][i] = TMP_USER_DEF_TYPE_ITEMS[i];
        arr_addr = USER_DEF_TYPE_ITEMS[last][i].arr_items_addr;
        if (arr_addr!=-1)
        {

            USER_DEF_TYPES_ARRS_NUMS[last] += 1;
            last_arr_num = USER_DEF_TYPES_ARRS_NUMS[last];
            /// Häzirki birlige degişli sanawlara ýene biri goşulýar
            USER_DEF_TYPES_ARRS[last] = realloc(USER_DEF_TYPES_ARRS[last], sizeof(**USER_DEF_TYPES_ARRS)*last_arr_num);
            USER_DEF_TYPES_ARRS_ITEMS[last] = realloc(USER_DEF_TYPES_ARRS_ITEMS[last], sizeof(**USER_DEF_TYPES_ARRS_ITEMS)*last_arr_num);

            USER_DEF_TYPES_ARRS[last][last_arr_num-1] = TMP_USER_DEF_TYPES_ARRS[arr_addr];
            USER_DEF_TYPES_ARRS_ITEMS[last][last_arr_num-1] = malloc(sizeof(***USER_DEF_TYPES_ARRS_ITEMS)*USER_DEF_TYPES_ARRS[last][last_arr_num-1].incs);
            for (j=0; j<USER_DEF_TYPES_ARRS[last][last_arr_num-1].incs; ++j)
            {
                USER_DEF_TYPES_ARRS_ITEMS[last][last_arr_num-1][j] = TMP_USER_DEF_TYPES_ARRS_ITEMS[arr_addr][j];
            }
        }
    }
    free_tmp_user_type();
}

void free_tmp_user_type()
{
    int i;
    if (TMP_USER_DEF_TYPES_NUM)
    {
        /// Eger tipiň sanaw birlikleri bar bolsa
        if (TMP_USER_DEF_TYPES_ARRS_NUMS)
        {
            int j;
            for (j=0; j<TMP_USER_DEF_TYPES_ARRS_NUMS; ++j)
                free(TMP_USER_DEF_TYPES_ARRS_ITEMS[j]);
            free(TMP_USER_DEF_TYPES_ARRS_ITEMS);
            free(TMP_USER_DEF_TYPES_ARRS);

            TMP_USER_DEF_TYPES_ARRS_ITEMS = NULL;
            TMP_USER_DEF_TYPES_ARRS = NULL;
            TMP_USER_DEF_TYPES_ARRS_NUMS = 0;
        }
        free(TMP_USER_DEF_TYPE_ITEMS);
        TMP_USER_DEF_TYPE_ITEMS = NULL;
        TMP_USER_DEF_TYPES_NUM = 0;
    }

}


int is_utype_ident(wchar_t *ident)
{
    int i, len = wcslen(ident);
    for (i=0; i<USER_DEF_TYPES_NUM; ++i)
    {
        if ( is_wcseq( USER_DEF_TYPES[i].ident, ident ) )
            return 1;
    }
    return 0;
}

