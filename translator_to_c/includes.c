/** Ýasaljak kodda #inçlude preprosessor komandasy bilen işleýän ähli funksiýalar we maglumatlar
 *
**/
#include <stdlib.h>
#include <string.h>
#include "includes.h"
#include "files.h"

// Içine inklud edýän preprosessor komandasyny ulanmaly faýllaryň sanyny ýene birine köpledýär
file_incs *includes_add_new()
{
    ++INCLUDES_NUM;
    INCLUDES = realloc(INCLUDES, sizeof(*INCLUDES)*INCLUDES_NUM);

    file_incs fi;
    fi.inc = NULL;
    fi.num = 0;
    INCLUDES[INCLUDES_NUM-1] = fi;

    return &INCLUDES[INCLUDES_NUM-1];
}


// Içinde inklud edýän preprosessor komandasyny ulanmaly faýla, inklud etmeli faýly goşýar
void includes_file_add_include(file_incs *fi, char *h_source)
{
    int i, len;
    // 1) Eger eýýäm goşulmaly faýl inklud etmeli faýllaryň sanawyna goşulan bolsa,
    //      a) onda faýl goşulanok.
    //    Ýogsa
    //      b) faýl goşulýar

    // 1)
    if (fi->num)
    {
        for(i=0; i<fi->num; ++i)
        {
            len = (strlen(h_source)>strlen(fi->inc[i]))?strlen(h_source):strlen(fi->inc[i]);
            if (strncmp(h_source, fi->inc[i], len)==0)
                // 1.a)
                return;
        }
    }
    else
    {
        fi->num = 0;
        fi->inc = NULL;
    }

    // 1.b)
    fi->num++;

    fi->inc = realloc(fi->inc, sizeof(char[MAX_FILE_LEN])*fi->num);

    strncpy(fi->inc[fi->num-1], h_source, strlen(h_source)+1);
}



/// Ýasalan kodlara, #include preprosessory goşulýar
void translator_to_c_add_includes()
{
    // ÜLŇILER
    //   Içine komandany goşmaly faýlyň adresi (1-nji ülňi)
    // Içine inklud komandany goşmaly her faýl boýunça
    //   1) Eger içine inklud etmeli faýllaryň sany boş bolmasa
    //      a) Içine inklud komandany goşmaly faýl açylýar. (1-nji ülňi <-)
    //      b) add_includes_to_source diýen funksiýa çagyrylyp, ähli goşmaly inkludlar, ýasalan koda salynýar.
    //      ç) açylan papka ýapylýar.

    int i;
    for(i=0; i<INCLUDES_NUM; ++i)
    {
        if (INCLUDES[i].num)
        {
            FILE *s = fopen(FILES[i].c_source, "r+");
            add_includes_to_source(s, INCLUDES[i].inc, INCLUDES[i].num);

            fclose(s);
        }
    }
}
