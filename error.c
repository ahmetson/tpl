/*
 * Yalnyshlyklar bilen ishleyan funksiyalaryn jemi
**/
#include <stdio.h>																// Make_algol.h fayllar bilen ishleyar
#include <stdlib.h>

#include "tpl.h"
#include "translator_to_c.h"															// Yalnyshlykly faylyn we setirin adresleri
#include "error.h"

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
char *parts_names[] = {
"0. TPL ulgamy",
"1. Giriş we faýllar bilen işleme",
"2. Harplar",
"3. Tokenler",
"4. Komandalar",
"5. Algoritm",
"6. Global sanawlar",
"7. Semantika",
"8. C translator"
};

/* Yalnyshlykly tekstlerin nomerleri */
int CODE0_PRAGMA_ACT_NOT_IDENTIFIED     = 0;
int CODE0_MAIN_FILE_ALREADY_CHOSED      = 1;
int CODE0_NOT_FOUND_MAIN_FILE           = 2;
int CODE0_IDENT_NOT_DEFINED             = 3;
int CODE0_IDENT_CANT_ADD                = 4;
int CODE0_INCLUDES_CANT_ADD             = 5;

int CODE1_FILE_NOT_FOUND                = 0;
int CODE1_FILE_CANT_OPEN                = 1;

int CODE2_UNKNOWN_TOKENS_CHAR           = 0;
int CODE2_PRAGMA_NOT_END                = 1;
int CODE2_PRAGMA_NOT_IDENT              = 2;
int CODE2_REMAIN_TOKEN                  = 3;
int CODE2_TOKEN_TOO_BIG                 = 4;
int CODE2_PREV_TOKEN_INCORRECT          = 5;
int CODE2_STRING_IS_WRONG               = 6;

int CODE3_PREV_TOK_INCORRECT            = 0;

int CODE4_CMD_HASNT_FIRST_TOKEN         = 0;
int CODE4_CANT_ADD_TOKEN                = 1;
int CODE4_CANT_IDENT_CMD                = 2;
int CODE4_VARS_IDENT_USED               = 3;
int CODE4_TOK_TYPES_NOT_MATCH           = 4;

int CODE7_TYPES_NOT_MATCH_RIGHT_DATA    = 0;
int CODE7_TYPES_NOT_MATCH_LEFT_DATA     = 1;
int CODE7_TYPES_NOT_MATCH_BOTH_IDENT    = 2;
//int CODE7_IDENT_NOT_DEFINED = 0;
//int CODE7_LEFT_IDENT_DEFINED = 0;
//int CODE7_LEFT_IDENT_NOT_DEFINED = 1;
//int CODE7_RIGHT_IDENT_NOT_DEFINED = 2;

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
        "Näbelli ülňi ulanylşy tapyldy. Ülňiler ulanmazdan öň yglan edilmeli"},
    {0, 4, NO_FILE, 0, 0, 0, NO_CMD,
        "Ülňi üçin faýly tapyp bolmady"},
    {0, 5, NO_FILE, 0, 0, 0, NO_CMD,
        "C translýatorynda faýllary birikdirip bolmady"},
    // 1. FAÝL ******************************************************
    {1, 0, NO_FILE, 0, 0, 0, NO_CMD,
        "Kodly faýl gelmedi"},
    {1, 1, YES_FILE, 0, 0, 0, NO_CMD,
        "Kodly faýly açmak başartmady"},
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
    // 5. Algoritmler ******************************************************
	// 6. Global sanawlar ******************************************************
	// 7. Semantika ******************************************************
    {7, 0, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Komandanyň sag tarapky token tipi çep tarapdaky maglumata gabat gelenok"},   // 7.Semantika
    {7, 1, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Komandanyň çep tarapky token tipi sag tarapdaky maglumata gabat gelenok"},
    {7, 2, YES_FILE, YES_LINE, YES_CHAR, YES_CHAR, NO_CMD,
        "Komandanyň sag we çep tarapdaky birlikleriniň tipleri gabat gelenok"}
	//"Komandanyň çep identifikatory öň yglan edilipdi",
	 //"Komandanyň çep identifikatory ulanmazdan ozal yglan edilmeli",
	 //"Komandanyň sag identifikatory çagyrylmazdan ozal yglan edilmeli"
    // 8. C transleýtor
};

/*
 * TPL'iň işlän wagty ýalňyşlyk duşanda, peýdaly maglumatlar çap edilýär.
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
	printf("\n\nÝALŇYŞLYK #%d.%d\n", CUR_PART, num);								// Yalnyshlyk hakda title
	error_item *err = get_err_inf(num);
	if (err->file_num || err->line_num || err->char_num)
	//if (CUR_PART>=2 && CUR_PART<=7)
	{
        if (err->file_num)
            printf("Faýl: '%s'; ", (tok->inf_file_num!=-1)?
                   FILES[tok->inf_file_num].source : CUR_FILE_NAME);	// Source faylda yalnyshlygyn tapylan yeri,
		if (err->line_num)
        {
            printf("Setir: %d; ", tok->inf_line_num>-1?tok->inf_line_num:CUR_LINE);
        }
        if (err->char_num)
            printf("Harp: '%c'; Setirdäki orny: %d", (tok->inf_char_num>0)?tok->inf_char:CUR_CHAR,
                                                     (tok->inf_char_num)>0?tok->inf_char_num: CUR_CHAR_POS);
		printf("\n");													// yalnyshlygyn tapylan yeri
	}
	if (err->code_line)
    {
        printf("\t\n\n");
        printf("\t%s... şu ýerräkde\n");
        printf("\t\n\n");
    }
	printf("%s\n\n", err->msg);	// Yalnyshlyk sozi

	free_globs();

	sys_rmdir(C_SOURCE_FOLDER);

	exit(num);
}



/**
 * Ýüze çykan ýalňyşlyk kody boýunça, ýalňyşlyk üçin gerekli maglumatlary yzyna gaýtarýar
**/
error_item *get_err_inf(int num)
{
    int i, j, items;
    items = sizeof(err_items)/sizeof(err_items[0]);
    for (i=0; i<items; ++i)
    {
        //printf("HAZIR: %d, SHU WAGT: %d, BARLANYAR:%d, JEMI: %d\n", i, CUR_PART, err_items[i].tpl_part, items);
        if (err_items[i].tpl_part==CUR_PART && err_items[i].err_code==num)
            return &err_items[i];
    }
    return err_items;
}
