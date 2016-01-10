#include <wchar.h>
#include "englification.h"

wchar_t tk_analog[][2] = {
    {L'ä', L'a'},    // ä
    {L'ň', L'n'},    // ň
    {L'ö', L'o'},    // ö
    {L'ş', L's'},    // ş
    {L'ü', L'u'},    // ü
    {L'ç', L'c'},    // ç
    {L'ý', L'y'},    // ý
    {L'ž', L'z'},    // ž
    {L'Ä', L'A'},    // Ä
    {L'Ň', L'N'},    // Ň
    {L'Ö', L'O'},    // Ö
    {L'Ş', L'S'},    // Ş
    {L'Ü', L'U'},    // Ü
    {L'Ç', L'C'},    // Ç
    {L'Ý', L'Y'},    // Ý
    {L'Ž', L'Z'}     // Ž
};

int is_tk_char(wchar_t c)
{
   int i;
   for (i=0; i<TK_LETTERS_NUM; ++i)
   {
       if (c==tk_analog[i][0])
          return 1;
   }
   return 0;
}


int is_tk_analog_char(wchar_t c)
{
    int i;
   for (i=0; i<TK_LETTERS_NUM; ++i)
   {
       if (c==tk_analog[i][1])
          return 1;
   }
   return 0;
}
