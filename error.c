/*
 * Yalnyshlyklar bilen ishleyan funksiyalaryn jemi
**/
#include <stdio.h>																// Make_algol.h fayllar bilen ishleyar
#include <stdlib.h>

#include "tpl.h"
#include "translator_to_c.h"															// Yalnyshlykly faylyn we setirin adresleri
#include "error.h"

/* Kompilyatoryn bolekleri*/
char *parts_names[] = {
"0. ulgam",
"1. inisalizasiya we kodly fayllary saygarma",
"2. harplar",
"3. tokenler",
"4. komandalar",
"5. algoritm",
"6. global sanawlar",
"7. semantika",
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
int CODE4_TOO_MANY_TOKENS = 1;
int CODE4_CANT_IDENT_CMD = 2;

/* Yalnyshlyklaryn tekstleri */
char *err_texts[][10] = {
	{"Kompilyatora niyetlenen komandanyn achyp bolmady",				// 0.Ulgam
	 "Eyyam bash fayl oturdildi",
	 "Programma-da bash fayl bellenmandir.\n \
Ony '#b1' pragmasy bilen kodyn ichinde bellemeli."},				
	{"Kodly fayllar tapylmady",											// 1.File we inisializasiya
	 "Kodly fayly achmak bashartmady"},	
	{"Bu harpdan token bashlanok",										// 2.Harplar (parsing)
	 "Pragma dolylygyna yazylmady",
	 "Pragma tanalmady",
	 "Sonky tokenden son, komandany gutaryan token yetenok",
	 "Token gaty uzyn bolyar. Ony gysgaldyn"},										
	{"Mundan onki token gutarylmadyk"},									// 3.Tokenler
	{"Hich bir komanda beyle tokenden bashlanok",						// 4.Komandalar
	 "Komanda maksimum 3 tokenden durmaly",
	 "Komandany saygaryp bolmady"},
	{},																	// 5.Algoritmler
	{},																	// 6.Global sanawlar
	{},																	// 7.Sanawlar
	{}																	// 8.Kompilyator we C translyator
};

/*
 * Kompilyator ishlande yalnyshlyk cykan wagty,
 * Haty chap edyar
 *
 * @part - Kompilyatoryn bolumi
 * @num  - yalnyshlygyn nomeri
 * @file - Yalnyshlygyn chykan fayly
 * @line - Yalnyshlygyn chykan setiri
 * @ch   - Yalnyshlyk chykanda, parserin saklanan harpy
**/
void print_err(int num)
{
	printf("\n\nYalnyshlyk yuze chykdy!\n");												// Yalnyshlyk hakda title
	if (CUR_PART>=2 && CUR_PART<=7)
	{	
	    printf("Fayl: '%s', Setir: '%d' ", cur_parse_file_name, cur_parse_line_num);	// Source faylda yalnyshlygyn tapylan yeri, 
		if (cur_parse_char!=-1)
			printf(", Bashlayan harpy: '%c'\n", cur_parse_char);						// Bolup biljek Source faylda, 
		printf("\n");																	// yalnyshlygyn tapylan yeri
	}																					
	
	printf("(Yalnyshlygyn nomeri: %d.%d) %s! \n\n", CUR_PART, num, err_texts[CUR_PART][num]);					// Yalnyshlyk sozi

	free_globs();

	sys_rmdir(C_SOURCE_FOLDER);

	exit(num);
}

