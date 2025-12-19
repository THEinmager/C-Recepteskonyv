

#ifndef RECEPTESKONYV_OSSZETEVOK_H
#define RECEPTESKONYV_OSSZETEVOK_H

#include  "adatszerkezet.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *osszetevo_beolvasas();

void osszetevo_listaz(Osszetevok *osszetevok_eleje);

void osszetevok_felszabaditas(Osszetevok *osszetevok_eleje);

Osszetevok *osszetevo_letrehozasa(Osszetevok *osszetevok_eleje);

void osszetevo_feltoltes(Osszetevok *osszetevok_eleje, const char *beolvasott_osszetevo_nev);

bool osszetevo_letezik_e(Osszetevok *osszetevok_eleje, const char *beolvasott_osszetevo);

Osszetevok *interakcio_uj_osszetevo(Osszetevok *osszetevok_eleje, char *beolvasott_osszetevo_nev); //menuben meg at kell irni, kell majd ott free elni mert kivettem ebbol a fuggvenybol

void puffer_tisztitas();

bool osszetevo_hasznalatban_e(const char *keresett_osszetevo_nev, ReceptCsomopont *receptek_eleje);

Osszetevok *osszetevo_torlese(Osszetevok *osszetevok_eleje, const char *torlendo_osszetevo_nev);

Osszetevok *interakcio_osszetevo_torles(Osszetevok *osszetevok_eleje, ReceptCsomopont *receptek_eleje);


#endif //RECEPTESKONYV_OSSZETEVOK_H
