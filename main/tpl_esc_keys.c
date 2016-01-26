/**
 * TPL programma-da ulanylýan Speşl açarlar.
**/
#include <string.h>
#include "tpl_esc_keys.h"

// Sanawy
wchar_t TPL_ESC_KEYS_LIST[CONST_TPL_ESC_KEYS][2] = {
    {L't', L't'},         // Tab
    {L's', L'n'},         // Setir -> newline
    {L'\'', L'\''},
    {L'"', L'"'},
    {L'=', L'='}
};

#define CONST_TPL_ESC_KEY_MAX_LEN 2
short int TPL_ESC_KEY_MAX_LEN = 2;      // Harp we ESC key başlangyjy
wchar_t      TPL_ESC_KEY_OPENER = L'=';
wchar_t      TPL_ESC_KEY_C_OPENER = L'\\';

/** Açarmy ýa däldigini barlaýar
    @type - Dyrnaklaryň arasynda goşa dyrnak ulanyp bolýar. We Goşa dyrnaklaryň arasynda dyrnaklary ulanyp bolýar.
            0 - Goşa dyrnaklaryň içinde
            1 - Dyrnaklaryň içinde
**/
int is_tpl_ESC_key(wchar_t ESC_key, wchar_t type)
{
    /** Harplaryň arasynda dyrnagy we
        goşa dyrnagy harp ýaly ulansoň bolýar*/
    if( ( type==ESCAPE_IN_CHR && ESC_key==L'"' ) ||
        ( type==ESCAPE_IN_STR && ESC_key==L'\'' ) )
        return 0;

    int i;
    for (i=0; i<CONST_TPL_ESC_KEYS; ++i)
    {
        if(TPL_ESC_KEYS_LIST[i][0]==ESC_key)
        {
            return 1;
        }
    }
    return 0;
}


/**
 * Harplaryň içinde, TPL speşl belgini C ascii kodyna geçirýär.
 *
 * Şowly bolsa, söziň uzynlygyny yzyna gaýtarmaly
**/
int tpl_ESC_key_to_c_ESC_key(wchar_t *val)
{
    int i, j, z, len = wcslen(val), return_len = 0;
    for (i=0; i<len; ++i)
    {
        // len-1: soňky harp, ýagny sözi ýa harpy gutarýan belgi
        // len-2: sözüň ýa harpyň iň soňky belgisi
        if (val[i]==TPL_ESC_KEY_OPENER && i<=len-2)
        {
            for (j=0; j<CONST_TPL_ESC_KEYS; ++j)
            {
                if (val[i+1]==TPL_ESC_KEYS_LIST[j][0])
                {
                    // Deňdir ýasaljak kodda ESС belgi däl
                    if (val[i+1] == L'=')
                    {
                        // i+1 bu göýbermeli harp
                        // Ondan soňkulary öňe harplary öňe süýşirmeli
                        for (z=i+1; z<len; ++z)
                        {

                            if (z+1<len)
                            {
                                val[z] = val[z+1];
                            }
                        }
                        len--;
                        val[len] = L'\0';
                    }
                    else
                    {
                        val[i]=TPL_ESC_KEY_C_OPENER;
                        val[i+1]=TPL_ESC_KEYS_LIST[j][1];
                        return_len += 2;
                    }
                }
            }
        }
    }
    return return_len;
}
