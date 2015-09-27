/*
 * Yalnyshlyklar bilen ishleyan funksiyalaryn jemi
**/
#include <stdio.h>																// Make_algol.h fayllar bilen ishleyar
#include <stdlib.h>

#include "tpl.h"
#include "translator_to_c.h"															// Yalnyshlykly faylyn we setirin adresleri
#include "error.h"

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
int CODE0_PRAGMA_ACT_NOT_IDENTIFIED = 0;
int CODE0_MAIN_FILE_ALREADY_CHOSED  = 1;
int CODE0_NOT_FOUND_MAIN_FILE       = 2;

int CODE1_FILE_NOT_FOUND = 0;
int CODE1_FILE_CANT_OPEN = 1;

int CODE2_UNKNOWN_TOKENS_CHAR = 0;
int CODE2_PRAGMA_NOT_END = 1;
int CODE2_PRAGMA_NOT_IDENT = 2;
int CODE2_REMAIN_TOKEN = 3;
int CODE2_TOKEN_TOO_BIG = 4;

int CODE3_PREV_TOK_INCORRECT = 0;

int CODE4_CMD_HASNT_FIRST_TOKEN = 0;
int CODE4_CANT_ADD_TOKEN = 1;
int CODE4_CANT_IDENT_CMD = 2;
int CODE4_VARS_IDENT_USED= 3;

int CODE7_LEFT_IDENT_DEFINED = 0;
int CODE7_LEFT_IDENT_NOT_DEFINED = 1;
int CODE7_RIGHT_IDENT_NOT_DEFINED = 2;

/* Ýalňyşlyklaryň tekstleri */
char *err_texts[][10] = {
	{"Kompilyatora niyetlenen komandanyn achyp bolmady",				// 0.Ulgam
	 "Eyyam bash fayl oturdildi",
	 "Ýazyljak programmanyň baş faýly tanalmady.\n \
Baş faýlyň kodly faýlynda ýörite pragmany ýazyň"},
	{"Kodly faýl gelmedi",											// 1.File we inisializasiya
	 "Kodly faýly açmak başartmady"},
	{"Şeýle symboldan harp başlanok",										// 2.Harplar (parsing)
	 "Pragma dolylygyna ýazylmady",
	 "Pragma tanalmady",
	 "Komandany gutarýan token ýetenok",
	 "Token gaty uzyn"},
	{"Mundan öňki token tanalmandy (ýa gutarylmandy)"},									// 3.Tokenler
	{"Hiç komandanyň birinji tokeniniň görnüşi şeýle token bolup bilenok",						// 4.Komandalar
	 "Kompýuteriň ýadynda ýer ýetmedi",
	 "Komanda tanalmady",
	 "Ulni uchin identifikator eyyam ulanyldy"},
	{},																	// 5.Algoritmler
	{},																	// 6.Global sanawlar
	{"Komandanyň çep identifikatory öň yglan edilipdi",		        	// 7.Semantika
	 "Komandanyň çep identifikatory ulanmazdan ozal yglan edilmeli",
	 "Komandanyň sag identifikatory çagyrylmazdan ozal yglan edilmeli"},
	{}																	// 8.Kompilyator we C translyator
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
void print_err(int num)
{
	printf("\n\nÝalňyşlyk ýüze çykdy!\n");								// Yalnyshlyk hakda title
	if (CUR_PART>=2 && CUR_PART<=7)
	{
	    printf("Faýl: '%s', Setir: '%d' ", CUR_FILE_NAME, CUR_LINE);	// Source faylda yalnyshlygyn tapylan yeri,
		if (CUR_CHAR!=-1)
			printf(", Harp: '%c'\n", CUR_CHAR);						    // Bolup biljek Source faylda,
		printf("\n");													// yalnyshlygyn tapylan yeri
	}

	printf("#%d.%d %s! \n\n", CUR_PART, num, err_texts[CUR_PART][num]);	// Yalnyshlyk sozi

	free_globs();

	sys_rmdir(C_SOURCE_FOLDER);

	exit(num);
}

