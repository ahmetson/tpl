/**
 * TPL programma-da ulanylýan Speşl açarlar.
**/
#include <string.h>
#include "tpl_esc_keys.h"

// Sanawy
char TPL_ESC_KEYS_LIST[CONST_TPL_ESC_KEYS][2] = {
    {'t', 't'},         // Tab
    {'s', 'n'},         // Setir -> newline
    {'\'', '\''},
    {'"', '"'},
    {'=', '='}
};

#define CONST_TPL_ESC_KEY_MAX_LEN 2
short int TPL_ESC_KEY_MAX_LEN = 2;      // Harp we ESC key başlangyjy
char      TPL_ESC_KEY_OPENER = '=';
char      TPL_ESC_KEY_C_OPENER = '\\';

/**
 * Açarmy ýa däldigini barlaýar
**/
int is_tpl_ESC_key(char ESC_key, char type)
{
    // Harplaryň arasynda dyrnagy we
    // goşa dyrnagy harp ýaly ulansoň bolýar
    if (type==1 && ESC_key=='"' ||
        type==0 && ESC_key=='\'')
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
int tpl_ESC_key_to_c_ESC_key(char *val)
{
    int i, j, z, len = strlen(val), return_len = 0;
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
                    if (val[i+1] == '=')
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
                        val[len] = '\0';
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
