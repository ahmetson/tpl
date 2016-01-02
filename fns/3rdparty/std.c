/** Ähli TPL Standard faňksiýeler bilen işleýän maglumatlar
**/
#include "std.h"
#include "std/chap_et.h"
#include "std/kabul_et.h"

int STD_FUNCS_NUM = 2;

// Şu funksiýalary programma biler ýaly
int (*std_funcs[])() = {
    add_std_chap_et,
    add_std_kabul_et
};

void add_std_funs()
{
    int i;
    for(i=0; i<STD_FUNCS_NUM; ++i)
    {
        std_funcs[i]();
    }
}
