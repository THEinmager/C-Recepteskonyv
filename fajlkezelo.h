
#ifndef RECEPTESKONYV_FAJLKEZELO_H
#define RECEPTESKONYV_FAJLKEZELO_H

#include "adatszerkezet.h"
#include <stdio.h>


void osszetevok_mentese_fajlba(Osszetevok *osszetevok_lista);

Osszetevok *osszetevok_beolvasasa_fajlbol(char *fajlnev);

ReceptCsomopont *receptek_beolvasasa_fajlbol(char *fajlnev);

char *fajl_sor_beolvasas(FILE *fajlmutato);

void receptek_mentese_fajlba(ReceptCsomopont *eleje);

#endif //RECEPTESKONYV_FAJLKEZELO_H
