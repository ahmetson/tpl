/*
 * Yalnyshlyklar bilen ishleyan funksiyalaryn jemi
**/
#include <stdio.h>																// Make_algol.h fayllar bilen ishleyar
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

#include "tpl.h"
#include "translator_to_c.h"															// Yalnyshlykly faylyn we setirin adresleri
#include "error.h"
#include "fns.h"

#define NO 0
#define YES
#define NO_FILE 0
#define YES_FILE 1
#define NO_LINE 0
#define YES_LINE 1
#define NO_CHAR 0
#define YES_CHAR 1
#define NO_CMD 0
#define YES_CMD 1

/* TPL şu bölümlerden durýar.*/
wchar_t *parts_names[] = {
L"0. TPL ulgamy",
L"1. Giriş we faýllar bilen işleme",
L"2. Harplar",
L"3. Tokenler",
L"4. Komandalar",
L"5. Algoritm",
L"6. Global sanawlar",
L"7. Semantika",
L"8. C translator"
};

/* Yalnyshlykly tekstlerin nomerleri */
int CODE0_PRAGMA_ACT_NOT_IDENTIFIED     = 0;
int CODE0_MAIN_FILE_ALREADY_CHOSED      = 1;
int CODE0_NOT_FOUND_MAIN_FILE           = 2;
int CODE0_IDENT_NOT_DEFINED             = 3;
int CODE0_IDENT_CANT_ADD                = 4;
int CODE0_INCLUDES_CANT_ADD             = 5;
int CODE0_CANT_OPEN_FILE                = 6;
int CODE0_UNSUPPORT_INCLUDE_FILE_CMD    = 7;
int CODE0_MISSING_REQUIRED_FILES        = 8;
int CODE0_CANT_PREPARE_TPL        = 9;
int CODE0_CANT_CREATE_EXE        = 10;

int CODE1_FILE_NOT_FOUND                = 0;
int CODE1_FILE_CANT_OPEN                = 1;
int CODE1_FILE_NAME_RESERVED            = 2;

int CODE2_UNKNOWN_TOKENS_CHAR           = 0;
int CODE2_PRAGMA_NOT_END                = 1;
int CODE2_PRAGMA_NOT_IDENT              = 2;
int CODE2_REMAIN_TOKEN                  = 3;
int CODE2_TOKEN_TOO_BIG                 = 4;
int CODE2_PREV_TOKEN_INCORRECT          = 5;
int CODE2_STRING_IS_WRONG               = 6;
int CODE2_REQUIRED_END_BLOCK            = 7;
int CODE2_PRAGMA_NOT_SUPPORT_IN_TRIANGLE_BLOCK = 8;
int CODE2_CMD_END_CHAR_NOT_SUPPORT_IN_TRIANGLE_BLOCK = 9;
int CODE2_TRIANGLE_BLOCK_NOT_SUPPORT_CMD = 10;
int CODE2_TRIANGLE_BLOCK_CANT_BE_EMPTY   = 11;
int CODE2_CANT_IDENT_CMD_IN_PAREN        = 12;

int CODE3_PREV_TOK_INCORRECT            = 0;

int CODE4_CMD_HASNT_FIRST_TOKEN         = 0;
int CODE4_CANT_ADD_TOKEN                = 1;
int CODE4_CANT_IDENT_CMD                = 2;
int CODE4_VARS_IDENT_USED               = 3;
int CODE4_TOK_TYPES_NOT_MATCH           = 4;
int CODE4_UNSUPPORT_CMD_INSIDE_FN       = 5;

int CODE7_TYPES_NOT_MATCH_RIGHT_DATA    = 0;
int CODE7_TYPES_NOT_MATCH_LEFT_DATA     = 1;
int CODE7_TYPES_NOT_MATCH_BOTH_IDENT    = 2;
int CODE7_FN_ARG_TYPES_NOT_MATCH        = 3;
int CODE7_FN_ARG_NUMES_NOT_MATCH        = 4;
int CODE7_UNKNOWN_FNS_CALLED            = 5;
int CODE7_UNKNOWN_DATA_USED             = 6;
int CODE7_A_TO_B_B_TO_A                 = 7;
int CODE7_UNKNOWN_GLOB_VAR_CALLED       = 8;
int CODE7_GLOB_VAR_MUST_DEF             = 9;
//int CODE7_IDENT_NOT_DEFINED = 0;
int CODE7_LEFT_IDENT_NOT_DEFINED        = 10;
int CODE7_RIGHT_IDENT_NOT_DEFINED       = 11;
int CODE7_GLOB_VAR_NOT_MATCH_DATA_TYPE  = 12;
int CODE7_NEED_IF_OR_IFELSE_CMD         = 13;
int CODE7_GLOB_DEF_IN_BLOCK             = 14;
int CODE7_NEED_OPEN_BLOCK_CMD           = 15;
int CODE7_ARRAY_DOESNT_EXIST            = 16;
int CODE7_UNDEFINED_ARRAY_INCLUDE       = 17;
int CODE7_CONNECTING_TO_ARRAY_NOT_TO_ITEM = 18;
int CODE7_OUT_OF_ARRAY_ITEMS            = 19;
int CODE7_GLOB_ARR_MUST_DEF             = 20;
int CODE7_GLOB_ARR_NOT_MATCH_DATA_TYPE  = 21;
int CODE7_FN_RET_TYPE_NOT_MATCH         = 22;
int CODE7_FN_NOT_DEF                    = 23;
int CODE7_MUST_BE_DEF_TYPE              = 24;
int CODE7_MUST_RETURN_TYPE              = 25;
int CODE7_RETURN_CMD_CAN_BE_USED_INSIDE_FUNCTION  = 26;
int CODE7_RETURN_CMD_TYPE_NOT_MATCH     = 27;

/* Ýalňyşlyklaryň tekstleri */
error_item err_items[] = {
    // 0. ULGAM ********************************************************
    {0, 0, YES_FILE, YES_FILE, YES_FILE, YES_FILE, NO_CMD,
        "TPL'e niýetlenen komandany açyp bolmady"},
    {0, 1, YES_FILE, YES_FILE, YES_FILE, YES_FILE, NO_CMD,
        "Eýýäm baş faýl ötürdildi"},
    {0, 2, NO_FILE, 0, 0, 0, NO_CMD,
        "Ýazyljak programmanyň baş faýly tanalmady.\n \
Baş faýlyň kodly faýlynda ýörite pragmany ýazyň"},
    {0, 3, YES_FILE, YES_FILE, YES_FILE, YES_FILE, NO_CMD,
        "Näbelli ülňi ulanylşy tapyldy. Ülňiler ulanmazdan öň yglan edilmeli. \nBaşga faýllarda yglan edilen bolsa umumy bolmaly"},
    {0, 4, NO_FILE, 0, 0, 0, NO_CMD,
        "Ülňi üçin faýly tapyp bolmady"},
    {0, 5, NO_FILE, 0, 0, 0, NO_CMD,
        "C translýatorynda faýllary birikdirip bolmady"},
    {0, 6, YES_FILE, YES_FILE, YES_FILE, YES_FILE, NO_CMD,
        "Pragma yglan edilen maglumatlaryň faýlyny açyp bilmedi"},
    {0, 7, YES_FILE, YES_FILE, YES_FILE, YES_FILE, NO_CMD,
        "Pragma arkaly görülýän faýllarda diňe yglan etme komandalary bolup bilýär"},
    {0, 8, YES_FILE, YES_FILE, YES_FILE, YES_FILE, NO_CMD,
        "Programa gerek bolan faýllar ýetenok"},
    {0, 9, YES_FILE, YES_FILE, YES_FILE, YES_FILE, NO_CMD,
        "TPL ulanylmana taýynlap bolmady. C kompilýatoryny kompýuteriňize ötürdip bolmady"},
    {0, 10, YES_FILE, YES_FILE, YES_FILE, YES_FILE, NO_CMD,
        ".EXE faýly ýasap bolmady. C kompilýatory bilen bagly näsazlyklar ýüze çykdy"},
    // 1. FAÝL ******************************************************
    {1, 0, NO_FILE, 0, 0, 0, NO_CMD,
        "Kodly faýl gelmedi"},
    {1, 1, YES_FILE, 0, 0, 0, NO_CMD,
        "Kodly faýly açmak başartmady"},
    {1, 2, YES_FILE, 0, 0, 0, NO_CMD,
        "Faýly parsing edip bolmady. Sebäbi TPL-e gerek at hasaplanylýar"},
    // 2. HARPLAR (PARSER) ******************************************************
    {2, 0, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Şeýle symboldan harp başlanok"},
    {2, 1, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Pragma dolylygyna ýazylmady"},
    {2, 2, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Pragma tanalmady"},
    {2, 3, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Komandany gutarýan token ýetenok"},
    {2, 4, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Token gaty uzyn"},
    {2, 5, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Token gutarylmady"},
    {2, 6, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Söz tokeninde ýalňyşlyk tapyldy"},
    {2, 7, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Blogy ýapýan komanda ýetenok"},
    {2, 8, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Pragmalary üçburç bloklaryň içinde çagyrmak bolanok"},
    {2, 9, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Üçburç bloklaryň içinde komandanyň gutarandygyny aňladýan belgi ulanylanok. Onuň üçin birlikleri bölüji belgi ulanylýar"},
    {2, 10, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Üçburç bloklaryň içinde beýle komandalary ulanmak bolanok"},
    {2, 11, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Üçburç bloklar boş bolmaly däl"},
    {2, 12, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Ýaýlaryň içindäki komandany tanap bolmady"},
    // 3. TOKENLER ******************************************************
    {3, 0, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Nädogry komanda birligi gabat gelindi"},
    // 4. KOMANDALAR ******************************************************
    {4, 0, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Hiç komandanyň birinji tokeniniň görnüşi şeýle token bolup bilenok"},
    {4, 1, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Kompýuteriň ýadynda ýer ýetmedi"},
    {4, 2, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Komanda tanalmady"},
    {4, 3, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Ülňi üçin identifikator eýýäm ulanyldy"},
    {4, 4, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Çep we sag tarapdaky komanda birlikleriniň tipleri gabat gelenok"},
    {4, 5, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Funksiýanyň blogynda bolmaly däl komanda gabat geldi"},
    // 5. Algoritmler ******************************************************
	// 6. Global sanawlar ******************************************************
	// 7. Semantika ******************************************************
    {7, 0, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Komandanyň sag tarapky token tipi çep tarapdaky maglumata gabat gelenok"},   // 7.Semantika
    {7, 1, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Komandanyň çep tarapky token tipi sag tarapdaky maglumata gabat gelenok"},
    {7, 2, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Komandanyň sag we çep tarapdaky birlikleriniň tipleri gabat gelenok"},
    {7, 3, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Funksiýanyň argumentleriniň tipi funksiýanyň talap edýän tiplerini gabat gelenok"},
    {7, 4, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Funksiýanyň argumentleriniň sany funksiýanyň talap edýän argument sanyna gabat gelenok"},
    {7, 5, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Näbelli funksiýe çagyryldy"},
    {7, 6, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Näbelli maglumat ulanyldy"},
    {7, 7, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "'A'->'B', 'B'->'A' ýagdaýy boldy. TPL'de bu mümkin däl"},
    {7, 8, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Näbelli global ülňi çagyrldy"},
    {7, 9, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Maglumatlara laýyk global ülňi bir ýerlerde yglan edilmeli"},
    {7, 10, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Çep tarapdaky identifikator yglan edilmedi"},
    {7, 11, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Sag tarapdaky identifikator yglan edilmedi"},
    {7, 12, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Yglan edilen global ülňiniň we bu ülňiniň yglan edilen maglumatlarynyň tipleri gabat gelenok"},
    {7, 13, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Başlangyç 'eger' komandasy ýok"},
    {7, 14, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Global ülňi bloklaryň içinde yglan edildi.\nGlobal ülňiler diňe faýl derejesinde yglan edilmeli"},
    {7, 15, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Blogy açýan komanda ýetenok"},
    {7, 16, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Birsyhly sanaw, çatylynmadan öň yglan edilmeli"},
    {7, 17, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Birsyhly sanawyň çatylan birlige görä içki sanawlary duşanok"},
    {7, 18, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Birsyhly sanawyň sanaw birligine çatylyndy. Diňe iň içki sanawlarynyň iň soňky birligine çatylyp bolýar"},
    {7, 19, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Birsyhly sanawda saklanylýan birlikleriň möçberi, çatylylan birlikden azyny saklaýar"},
    {7, 20, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Maglumatlara laýyk global birsyhly sanaw bir ýerlerde yglan edilmeli"},
    {7, 21, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Yglan edilen global birsyhly sanawyň we bu birsyhly sanawyň yglan edilen maglumatlarynyň tipleri gabat gelenok"},
    {7, 22, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Beýan edilýän we yglan edilýän funksiýanyň gaýtarmaly maglumatynyň tipleri gabat gelmedi"},
    {7, 23, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Beýan edilen funksiýanyň yglan edilen görnüşi tapylmady"},
    {7, 24, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Sada tipdäki birlik bolmaly"},
    {7, 25, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Maglumatyň tipi bolmaly"},
    {7, 26, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "'yza' komandasy diňe funksiýanyň içinde gabat gelmeli"},
    {7, 27, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "'yza' komandanyň gaýtarýan maglumatynyň tipi, funksiýanyň gaýtarmaly maglumatynyň tipleri gabat gelenok"}
    // 8. C transleýtor
};

/** TPL'iň işlän wagty ýalňyşlyk duşanda, peýdaly maglumatlar çap edilýär.
 *
 * Funksiýanyň içindäki ulanylýan ülňiler:
 * @part - TPL'iň bölümi
 * @num  - Ýalňyşlygyň bölümiň içindäki nomeri
 * @file - Ýalňyşlyk faýlda çykan bolsa, onuň ady
 * @line - Ýalňyşlyk faýlda çykan bolsa, çykan setiri
 * @ch   - Ýalňyşlyk çykan wagty, parseriň saklanan symwoly
**/
void print_err(int num, token *tok)
{
	printf("ÝALŇYŞLYKlar #%d.%d\n", CUR_PART, num);								// Yalnyshlyk hakda title
	error_item *err = get_err_inf(num);
	if (err->file_num || err->line_num || err->wchar_t_num)
	//if (CUR_PART>=2 && CUR_PART<=7)
	{
        if (err->file_num)
            printf("Faýl: '%S'; ", (tok->inf_file_num!=-1) ?
                   FILES[tok->inf_file_num].source : CUR_FILE_NAME );	// Source faylda yalnyshlygyn tapylan yeri,
		if (err->line_num)
        {
            printf("Setir: %d; ", tok->inf_line_num>-1 ? tok->inf_line_num : CUR_LINE);
        }
        if (err->wchar_t_num)
            printf("Harp: '%c'; Setirdäki orny: %d", (tok->inf_wchar_t_num>0) ? tok->inf_wchar_t : CUR_CHAR,
                                                     (tok->inf_wchar_t_num)>0 ? tok->inf_wchar_t_num : CUR_CHAR_POS);
		printf("\n");													// yalnyshlygyn tapylan yeri
	}

	printf("%s\n\n", err->msg);

	sys_rmdir( DIR_C_FOLDER );

	exit(num);
}



/**  Ýüze çykan ýalňyşlyk kody boýunça, ýalňyşlyk üçin gerekli maglumatlary yzyna gaýtarýar*/
error_item *get_err_inf(int num)
{
    int i, items;
    items = sizeof(err_items)/sizeof(err_items[0]);
    for (i=0; i<items; ++i)
    {
        //printf(L"HAZIR: %d, SHU WAGT: %d, BARLANYAR:%d, JEMI: %d\n", i, CUR_PART, err_items[i].tpl_part, items);
        if (err_items[i].tpl_part==CUR_PART && err_items[i].err_code==num)
            return &err_items[i];
    }
    return err_items;
}
