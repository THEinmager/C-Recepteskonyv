
#include <stdio.h>
#include "adatszerkezet.h"
#include <stdlib.h>
#include "debugmalloc.h"
#include "receptek.h"
#include "osszetevok.h"
#include "fajlkezelo.h"
#include "menu.h"

int main(void) {

    srand(time(NULL));
    
    Osszetevok *osszetevok = NULL;
    osszetevok = osszetevok_beolvasasa_fajlbol("osszetevok.txt");
    
    ReceptCsomopont *receptek = NULL;
    receptek = receptek_beolvasasa_fajlbol("receptek.txt");
    
    menu(&osszetevok,&receptek);

    osszetevok_mentese_fajlba(osszetevok);
    osszetevok_felszabaditas(osszetevok);
    
    receptek_mentese_fajlba(receptek);
    receptek_felszabaditasa(receptek);

    return 0;
}

