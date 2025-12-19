
#ifndef RECEPTESKONYV_ADATSZERKEZET_H
#define RECEPTESKONYV_ADATSZERKEZET_H

//Ez tárolja az összes választható összetevőt (az osszetevok.txt alapján)
typedef struct Osszetevok {
    char *osszetevo_nev;
    struct Osszetevok *kovetkezo;
} Osszetevok;

//Egy recepthez tartozó hozzávalókat tárolja
typedef struct ReceptHozzavalok {
    char *osszetevo_nev;
    int mennyiseg;
    char *mertekegyseg;
    struct ReceptHozzavalok *kovetkezo;
} ReceptHozzavalok;

//Tartalmazza egy étel adatait és a fenti hozzávalók lista elejére mutató pointert
typedef struct Recept {
    char *etel_nev;
    ReceptHozzavalok *eleje;
    char *elkeszites;
} Recept;

typedef struct ReceptCsomopont {
    struct Recept adat;
    struct ReceptCsomopont *kovetkezo;
} ReceptCsomopont;



#endif //RECEPTESKONYV_ADATSZERKEZET_H
