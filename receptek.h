

#ifndef RECEPTESKONYV_RECEPTEK_H
#define RECEPTESKONYV_RECEPTEK_H

#include "adatszerkezet.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

int szam_beolvasas();

void receptek_felszabaditasa(ReceptCsomopont *receptek_eleje);

bool van_e_ilyen_recept(ReceptCsomopont *receptek_eleje,const char *keresett_recept_nev);

bool tartalmaz_betut(const char *szoveg);

ReceptCsomopont *recept_letrehozasa(ReceptCsomopont *receptek_eleje,char *beolvasott_recept_nev);

ReceptHozzavalok *recept_hozzavalok_letrehozasa(ReceptHozzavalok *recept_hozzavaloinak_eleje);

bool hozzavalo_szerepel_e(ReceptHozzavalok *recept_hozzavaloinak_eleje,const char *keresett_nev);

ReceptCsomopont *recept_hozzavalok_beolvasasa(ReceptCsomopont *aktualis_recept,Osszetevok *osszetevok_eleje,ReceptHozzavalok *recept_hozzavaloinak_eleje);

char *osszetevo_megkeres(Osszetevok *osszetevok_eleje,int keresett_sorszam);

int osszetevok_db(Osszetevok *osszetevok_eleje);

ReceptCsomopont *interakcio_uj_recept(ReceptCsomopont *receptek_eleje,Osszetevok *osszetevok_eleje);

ReceptCsomopont *recept_kereses_nev_alapjan(ReceptCsomopont *receptek_eleje);

ReceptCsomopont *recept_kereses_random(ReceptCsomopont *receptek_eleje);

ReceptCsomopont *recept_keres_egy_osszetevo(ReceptCsomopont *receptek_eleje, Osszetevok *osszetevok_eleje);

ReceptCsomopont *recept_keres_tobb_osszetevo(ReceptCsomopont *receptek_eleje, Osszetevok *osszetevok_eleje);

Osszetevok *osszetevo_kivalasztasa(Osszetevok *osszetevok_eleje,int keresett_sorszam);

void recept_megjelenit(const ReceptCsomopont *megjelenitendo_recept);

void recept_torlese_segedfuggveny(ReceptCsomopont *torlendo_recept);

ReceptCsomopont *recept_torlese(ReceptCsomopont *torlendo_recept,ReceptCsomopont *receptek_eleje);

void recept_hozzavalok_kiiras(ReceptHozzavalok *hozzavalok_eleje);

void kereses_menu_kiiras();

ReceptCsomopont *kereses_seged_kiiras(ReceptCsomopont *kereses_eredmenye,ReceptCsomopont *receptek_eleje);

ReceptHozzavalok *hozzavalo_megkeres(ReceptHozzavalok *hozzavalok_eleje,int keresett_sorszam);

ReceptCsomopont *recept_modositas(ReceptCsomopont *modositando_recept, ReceptCsomopont *receptek_eleje);

ReceptCsomopont *interakcio_kereses(ReceptCsomopont *receptek_eleje, Osszetevok *osszetevok_eleje);

#endif //RECEPTESKONYV_RECEPTEK_H
