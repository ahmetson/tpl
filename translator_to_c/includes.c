/** Ýasaljak kodda #inçlude preprosessor komandasy bilen işleýän ähli funksiýalar we maglumatlar
 *
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes.h"
#include "files.h"
#include "../fns.h"

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
void includes_file_add_include( file_incs *fi, wchar_t *h_source )
{
    int i;
    /** 1) Eger eýýäm goşulmaly faýl inklud etmeli faýllaryň sanawyna goşulan bolsa,
    //      a) onda faýl goşulanok.
    //    Ýogsa
    //      b) faýl goşulýar*/

    /** 1)*/
    if ( fi->num )
    {
        for( i=0; i<fi->num; ++i )
        {
            if ( is_wcseq( h_source, fi->inc[ i ] ) )
                /// 1.a)
                return;
        }
    }
    else
    {
        fi->num = 0;
        fi->inc = NULL;
    }

    /** 1.b)*/
    fi->num++;

    fi->inc = realloc( fi->inc, sizeof( wchar_t[ MAX_FILE_LEN ] )*fi->num );

    wcscpys( fi->inc[ fi->num-1 ], h_source );
}



/** Ýasalan kodlara, #include preprosessory goşulýar*/
void translator_to_c_add_includes()
{
    int i;
    for(i=0; i<INCLUDES_NUM; ++i)
    {
        /** Faýla inklud etmeli. */
        if (INCLUDES[i].num)
        {
            /** Diýmek inkludlar ýazylýar */
            FILE *s = _wfopen( FILES[i].c_source, L"r+, ccs=UTF-8" );
            add_includes_to_source(s, INCLUDES[i].inc, INCLUDES[i].num);
            fclose(s);
        }
    }
}
