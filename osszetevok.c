

#include "osszetevok.h"
#include "debugmalloc.h"
#include "receptek.h"

//visszatérési érték a beolvasott dinamikusan lefoglalt string memóriacíme, ha nem sikerült akkor NULL
//osszetevo beolvasasnak neveztem el de ezt használom mindig bármilyen string beolvasására, nem neveztem át.

char *osszetevo_beolvasas() {
    int db = 0;
    char *beolvasott_szoveg = malloc((db + 1) * sizeof(char)); //+1 a lezáró nullának
    beolvasott_szoveg[0] = '\0';
    char betu;
    while (scanf("%c", &betu) == 1 && betu != '\n') {
        //amig be tudott olvasni és nem enter
        char *ujstring = (char *) malloc((db + 1 + 1) * sizeof(char));
        if (ujstring == NULL) {
            printf("Hiba tortent a memoriafoglalasnal\n");
            return NULL;
        }
        strcpy(ujstring, beolvasott_szoveg);
        ujstring[db] = betu;
        ujstring[db + 1] = '\0';
        free(beolvasott_szoveg);
        beolvasott_szoveg = ujstring;
        db += 1;
    }

    return beolvasott_szoveg;
}

//simán végigmegy az egyszeresen lancolt listan es kiirja az osszetevoket, bemenet: a lista amin vegigmenjen, nincs kimenet
void osszetevo_listaz(Osszetevok *osszetevok_eleje) {
    if (osszetevok_eleje == NULL) {
        printf("   (Nincsen felhasznalhato osszetevo)\n");
        return;
    }
    printf("\n   === FELHASZNALHATO OSSZETEVOK ===\n");
    
    int i=1;
    for (Osszetevok *aktualis_osszetevo = osszetevok_eleje; aktualis_osszetevo != NULL; aktualis_osszetevo = aktualis_osszetevo->kovetkezo) {
        //%2d: két karakter széles helyet hagy a szamnak
        printf("   [%2d] %s\n", i, aktualis_osszetevo->osszetevo_nev);
        i+=1;
    }
    printf("   ======================\n");
}

void osszetevok_felszabaditas(Osszetevok *osszetevok_eleje) {
    Osszetevok *aktualis_osszetevo = osszetevok_eleje;
    if (aktualis_osszetevo != NULL) {
        Osszetevok *lemarado = aktualis_osszetevo;
        while (aktualis_osszetevo != NULL) {
            aktualis_osszetevo = aktualis_osszetevo->kovetkezo;
            free(lemarado->osszetevo_nev);
            free(lemarado);
            lemarado = aktualis_osszetevo;
        }
    }
}

Osszetevok *osszetevo_letrehozasa(Osszetevok *osszetevok_eleje) {
    Osszetevok *uj_osszetevo = (Osszetevok *) malloc(sizeof(Osszetevok));
    uj_osszetevo->osszetevo_nev = NULL;
    uj_osszetevo->kovetkezo = NULL;
    if (osszetevok_eleje == NULL) {
        //ha üres az eredeti lista akkor az uj az elso eleme
        osszetevok_eleje = uj_osszetevo;
    } else {
        Osszetevok *mozgo = osszetevok_eleje; //ha nem akkor vegig megy a listan es megall az utolso elemen
        while (mozgo->kovetkezo != NULL) {
            mozgo = mozgo->kovetkezo;
        }
        mozgo->kovetkezo = uj_osszetevo; //az uj elemre mutat, igy a vegen van
    }
    return osszetevok_eleje;
}

void osszetevo_feltoltes(Osszetevok *osszetevok_eleje, const char *beolvasott_osszetevo_nev) {
    if (osszetevok_eleje == NULL) {
        //ha ures akkor nincs mit átállítani
        return;
    }
    Osszetevok *aktualis_osszetevo = osszetevok_eleje;
    while (aktualis_osszetevo->kovetkezo != NULL) {
        aktualis_osszetevo = aktualis_osszetevo->kovetkezo;
    }

    if (aktualis_osszetevo->osszetevo_nev != NULL) {
        free(aktualis_osszetevo->osszetevo_nev);
    }

    aktualis_osszetevo->osszetevo_nev = (char *) malloc((strlen(beolvasott_osszetevo_nev)+1)*sizeof(char));
    strcpy(aktualis_osszetevo->osszetevo_nev, beolvasott_osszetevo_nev);
}

bool osszetevo_letezik_e(Osszetevok *osszetevok_eleje, const char *beolvasott_osszetevo) {
    Osszetevok *aktualis_osszetevo = osszetevok_eleje;
    while (aktualis_osszetevo != NULL) {
        if (strcmp(aktualis_osszetevo->osszetevo_nev, beolvasott_osszetevo) == 0) {
            return true;
        }
        aktualis_osszetevo = aktualis_osszetevo->kovetkezo;
    }
    return false;
}

Osszetevok *interakcio_uj_osszetevo(Osszetevok *osszetevok_eleje, char *beolvasott_osszetevo_nev) {
    bool letezik_e = osszetevo_letezik_e(osszetevok_eleje, beolvasott_osszetevo_nev);
    if (!letezik_e) {
        osszetevok_eleje = osszetevo_letrehozasa(osszetevok_eleje);
        osszetevo_feltoltes(osszetevok_eleje, beolvasott_osszetevo_nev);
    } else {
        printf("mar letezik ilyen osszetevo\n");
    }
    return osszetevok_eleje;
}

void puffer_tisztitas() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

bool osszetevo_hasznalatban_e(const char *keresett_osszetevo_nev, ReceptCsomopont *receptek_eleje) {
    ReceptCsomopont *aktualis_recept = receptek_eleje;
    while (aktualis_recept != NULL) {
        ReceptHozzavalok *aktualis_hozzavalo = aktualis_recept->adat.eleje;
        while (aktualis_hozzavalo != NULL) {
            if (strcmp(aktualis_hozzavalo->osszetevo_nev, keresett_osszetevo_nev) == 0) {
                return true;
            }
            aktualis_hozzavalo = aktualis_hozzavalo->kovetkezo;
        }
        aktualis_recept = aktualis_recept->kovetkezo;
    }
    return false;
}

Osszetevok *osszetevo_torlese(Osszetevok *osszetevok_eleje, const char *torlendo_osszetevo_nev) {
    Osszetevok *elozo_osszetevo=NULL;
    Osszetevok *aktualis_osszetevo=osszetevok_eleje;
    
    while (aktualis_osszetevo!=NULL && (strcmp(aktualis_osszetevo->osszetevo_nev,torlendo_osszetevo_nev)!=0)) {  //amig nem egyenlo a torlendo_osszetevovel az aktualis osszetevo vagy vegig ment a listan
        elozo_osszetevo=aktualis_osszetevo;
        aktualis_osszetevo=aktualis_osszetevo->kovetkezo;
    }
    
    if (aktualis_osszetevo==NULL) { //vegig ment a listan es nem volt benne 
        return osszetevok_eleje;
    }
    
    if (elozo_osszetevo==NULL) { // ha az elso elem a torlendo_osszetevo
        osszetevok_eleje=aktualis_osszetevo->kovetkezo;
    }
    else { //ha nem az elso elem a torlendo_osszetevo
        elozo_osszetevo->kovetkezo=aktualis_osszetevo->kovetkezo;
    }
    //felszabaditas vvv
    free(aktualis_osszetevo->osszetevo_nev);
    free(aktualis_osszetevo);
    
    return osszetevok_eleje;
}

Osszetevok *interakcio_osszetevo_torles(Osszetevok *osszetevok_eleje, ReceptCsomopont *receptek_eleje) {
    osszetevo_listaz(osszetevok_eleje); //ki listaz
    printf("\nMit szeretnel torolni? Ne a sorszamot ird be hanem a nevet az osszetevonek!");
    
    char *torlendo_osszetev_nev = NULL;
    while (1) {
        torlendo_osszetev_nev = osszetevo_beolvasas();
        
        // van e benne betu
        if (tartalmaz_betut(torlendo_osszetev_nev)) {
            break; //ha jo akkor kilepunk
        } else {
            printf("Hiba: A torlendo nev nem lehet csak szam vagy ures!\n");
            printf("Add meg ujra a nevet: ");
            free(torlendo_osszetev_nev); //fel kell szabaditani a rossz nevet
        }
    }

    bool letezik_osszetevo;
    letezik_osszetevo = osszetevo_letezik_e(osszetevok_eleje, torlendo_osszetev_nev);
    if (letezik_osszetevo) { //ha létezik egyáltalan
        bool hasznalatban_osszetevo = osszetevo_hasznalatban_e(torlendo_osszetev_nev, receptek_eleje);
        if (!hasznalatban_osszetevo) { //létezik és nincs hasznalatban
                Osszetevok *torles_eredmenye=osszetevo_torlese(osszetevok_eleje,torlendo_osszetev_nev);
                free(torlendo_osszetev_nev);
                return torles_eredmenye;
        } else { //létezik de használatban van
            printf("\nHasznalatban van a torlendo osszetevo! Nem lehet torolni!\n");
            free(torlendo_osszetev_nev);
            return osszetevok_eleje;
        }
    } else { // nem létezik ilyen osszetevo
        printf("\nNem letezik ilyen osszetevo!\n");
        free(torlendo_osszetev_nev);
        return osszetevok_eleje;
    }
}
