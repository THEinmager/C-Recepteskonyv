#include "adatszerkezet.h"
#include "receptek.h"
#include "osszetevok.h"
#include "debugmalloc.h"
#include <ctype.h>

//konzolrol beolvas egy szamot, nincs bemenet, kimenete a beolvasott szam
int szam_beolvasas() {
    int szam = 0;

    while (scanf(" %d", &szam) != 1 || szam < 0) {
        //felhasznalo rosszat irt be, bekerül a pufferbe
        puffer_tisztitas();
        printf("Szamot adj meg!\n");
        fflush(stdout);
        //puffer kitisztítása (getchar kiveszi a puffer legelso elemet es addig csinalja amig nem new line karakter, es akkor c='\n' es leall a ciklus, üres a pufferÖ
    }
    puffer_tisztitas();
    //sikeres scanf- utan az ottmaradt new line-t kitakaritja, azert kell mert az osszetevo_beolvas new line ig olvas be de az mar ott van a pufferben ezert ki kell takaritani

    return szam;
}


//eloszor egy recepthez tartozo hozzavalok listabol kell felszabaditani mindent
//utana magat a hozzavalo listat
//ezt az osszes hozzavalokon
//utana a recept nevet es elkeszitest kell felszabaditani
//utana lehet a magat a recept "blokkot"
//ezt az osszes receptre

void receptek_felszabaditasa(ReceptCsomopont *receptek_eleje) {
    //inchworm módszerrel torles
    ReceptCsomopont *aktualis_recept = receptek_eleje;
    ReceptCsomopont *elozo_recept = aktualis_recept;

    while (aktualis_recept != NULL) {
        //ez osszes recepten vegigmenes
        //inchworm modszerrel torles
        ReceptHozzavalok *aktualis_hozzavalo = aktualis_recept->adat.eleje;
        ReceptHozzavalok *elozo_hozzavalo = aktualis_hozzavalo;

        while (aktualis_hozzavalo != NULL) {
            //az aktualis recept osszes hozzavalo listain vegigmenes

            aktualis_hozzavalo = aktualis_hozzavalo->kovetkezo;
            //hozzavalo lista "kitakaritasa" vvv
            free(elozo_hozzavalo->osszetevo_nev);
            free(elozo_hozzavalo->mertekegyseg);
            free(elozo_hozzavalo); //<< maga az aktualis hozzavalo lista felszabaditasa

            elozo_hozzavalo = aktualis_hozzavalo;
        }

        aktualis_recept = aktualis_recept->kovetkezo;
        //az aktualis lista "kitakaritasa" vvv
        free(elozo_recept->adat.etel_nev);
        free(elozo_recept->adat.elkeszites);
        free(elozo_recept); //<< maga az aktualis recept felszabaditasa

        elozo_recept = aktualis_recept;
    }
}

bool van_e_ilyen_recept(ReceptCsomopont *receptek_eleje, const char *keresett_recept_nev) {
    ReceptCsomopont *aktualis_recept = receptek_eleje;
    while (aktualis_recept != NULL) {
        if ((strcmp(aktualis_recept->adat.etel_nev, keresett_recept_nev)) == 0) {
            return true;
        }
        aktualis_recept = aktualis_recept->kovetkezo;
    }
    return false;
}

bool tartalmaz_betut(const char *szoveg) {
    if (szoveg == NULL) {
        return false;
    }

    for (int i = 0; szoveg[i] != '\0'; i++) {
        // Az isalpha megnézi, hogy az adott karakter betu e
        if (isalpha(szoveg[i])) {
            return true; 
        }
    }
    return false; //egy betu sem volt benne
}

ReceptCsomopont *recept_letrehozasa(ReceptCsomopont *receptek_eleje, char *beolvasott_recept_nev) {
    //név és elkeszites bekérése
    ReceptCsomopont *uj_recept = (ReceptCsomopont *) malloc(sizeof(ReceptCsomopont));
    uj_recept->adat.etel_nev = beolvasott_recept_nev;
    
    char *elkeszites_modja = NULL;
    
    while (1) {
        printf("Elkeszites modja:\n");
        elkeszites_modja = osszetevo_beolvasas();
        
        if (tartalmaz_betut(elkeszites_modja)) {
            break; // jol beirta a felhasznalo
        } else {
            printf("Hiba: Az elkeszites nem lehet ures vagy csak szam!\n");
            free(elkeszites_modja); // fel kell szabaditani a rosszat
        }
    }
    
    uj_recept->adat.elkeszites = elkeszites_modja;
    uj_recept->adat.eleje = NULL; //még nem mutat semmire
    uj_recept->kovetkezo=NULL;

    //befuzes vvv
    if (receptek_eleje == NULL) {
        //ures a receptek lista
        receptek_eleje = uj_recept;
    } else {
        //nem ures a receptek lista, elejere fuzes
        uj_recept->kovetkezo = receptek_eleje;
        receptek_eleje = uj_recept;
    }

    return receptek_eleje;
}

ReceptHozzavalok *recept_hozzavalok_letrehozasa(ReceptHozzavalok *recept_hozzavaloinak_eleje) {
    //egyet hoz létre
    ReceptHozzavalok *uj_hozzavalok = (ReceptHozzavalok *) malloc(sizeof(ReceptHozzavalok));
    uj_hozzavalok->osszetevo_nev = NULL;
    uj_hozzavalok->mennyiseg = 0;
    uj_hozzavalok->mertekegyseg = NULL;
    uj_hozzavalok->kovetkezo = NULL;

    if (recept_hozzavaloinak_eleje == NULL) {
        recept_hozzavaloinak_eleje = uj_hozzavalok;
    } else {
        uj_hozzavalok->kovetkezo = recept_hozzavaloinak_eleje;
        recept_hozzavaloinak_eleje = uj_hozzavalok;
    }

    return recept_hozzavaloinak_eleje;
}

char *osszetevo_megkeres(Osszetevok *osszetevok_eleje, int keresett_sorszam) {
    int jelenlegi_sorszam = 1;
    Osszetevok *aktualis_osszetevo = osszetevok_eleje;
    while (aktualis_osszetevo != NULL) {
        if (jelenlegi_sorszam == keresett_sorszam) {
            return aktualis_osszetevo->osszetevo_nev;
        }
        jelenlegi_sorszam += 1;
        aktualis_osszetevo = aktualis_osszetevo->kovetkezo;
    }
    return NULL; //nem talalta meg
}

int osszetevok_db(Osszetevok *osszetevok_eleje) {
    int darabszam = 0;
    Osszetevok *aktualis_osszetevo = osszetevok_eleje;
    while (aktualis_osszetevo != NULL) {
        darabszam += 1;
        aktualis_osszetevo = aktualis_osszetevo->kovetkezo;
    }
    return darabszam;
}

bool hozzavalo_szerepel_e(ReceptHozzavalok *recept_hozzavaloinak_eleje, const char *keresett_osszetevo) {
    ReceptHozzavalok *aktualis_hozzavalo = recept_hozzavaloinak_eleje;
    while (aktualis_hozzavalo != NULL) {
        // ha van neve és egyezik a keresettel
        if (aktualis_hozzavalo->osszetevo_nev != NULL && strcmp(aktualis_hozzavalo->osszetevo_nev, keresett_osszetevo) == 0) {
            return true; //mar letezik ilyen
        }
        aktualis_hozzavalo = aktualis_hozzavalo->kovetkezo;
    }
    return false; //nem letezik ilyen
}

ReceptCsomopont *recept_hozzavalok_beolvasasa(ReceptCsomopont *aktualis_recept, Osszetevok *osszetevok_eleje, ReceptHozzavalok *recept_hozzavaloinak_eleje) {
    while (1) {
        int elerheto_db = osszetevok_db(osszetevok_eleje);
        if (elerheto_db == 0) {
            printf("Nincsen egy osszetevo sem! Adj hozza uj osszetevot vagy lepj ki!");
        } else {
            printf("Az osszes felhasznalhato osszetevo:\n");
            osszetevo_listaz(osszetevok_eleje);

            printf("1. Meglevo osszetevokbol kivalasztas\n");
            printf("2. Uj osszetevo hozzaadasa\n");
            printf("3. Kilepes\n");

            int valasztas = szam_beolvasas();
            if (valasztas == 1) {
                //meglevokbol kivalasztas

                printf("Valassz egy osszetevot a sorszam beirasaval!\n");
                int valasztott_sorszam = szam_beolvasas();

                while (valasztott_sorszam > elerheto_db || valasztott_sorszam < 0) {
                    printf("Helyes sorszamot adj meg!\n");
                    valasztott_sorszam = szam_beolvasas();
                }
                
                recept_hozzavaloinak_eleje = recept_hozzavalok_letrehozasa(recept_hozzavaloinak_eleje);

                aktualis_recept->adat.eleje = recept_hozzavaloinak_eleje;

                char *kivalasztott_nev = osszetevo_megkeres(osszetevok_eleje, valasztott_sorszam);

                if (hozzavalo_szerepel_e(recept_hozzavaloinak_eleje, kivalasztott_nev)) {
                    printf("Ez az osszetevo mar szerepelt!\n");
                    continue; //visszaugrik a ciklus elejere, nem adja hozza mert mar benne van
                }

                printf("Mennyiseg:\n");
                int mennyiseg = szam_beolvasas();
                
                char *mertekegyseg = NULL;
                while (1) {
                    printf("Mertekegyseg: ");
                    mertekegyseg = osszetevo_beolvasas();
                    if (tartalmaz_betut(mertekegyseg)) {
                        break;
                    } else {
                        printf("Hiba: A mertekegyseg nem lehet ures vagy csak szam!\n");
                        free(mertekegyseg);
                    }
                }
                
                //mindig az uj-ra mutat a hozzavalok_eleje vvv
                recept_hozzavaloinak_eleje->osszetevo_nev = (char *) malloc((strlen(kivalasztott_nev) + 1) * sizeof(char));
                strcpy(recept_hozzavaloinak_eleje->osszetevo_nev, kivalasztott_nev);

                recept_hozzavaloinak_eleje->mennyiseg = mennyiseg;
                recept_hozzavaloinak_eleje->mertekegyseg = mertekegyseg;
                
            }
            else if (valasztas == 2) {
                
                char *beirt_osszetevo = NULL;
                while (1) {
                    printf("Uj hozzavalo neve: ");
                    beirt_osszetevo = osszetevo_beolvasas();
                    
                    if (tartalmaz_betut(beirt_osszetevo)) {
                        break; // Helyes név
                    } else {
                        printf("Hiba: A nev nem lehet ures vagy csak szam!\n");
                        free(beirt_osszetevo);
                    }
                }
                
                bool osszetevo_letezik = osszetevo_letezik_e(osszetevok_eleje, beirt_osszetevo);
                
                if (!osszetevo_letezik) {
                    osszetevok_eleje = interakcio_uj_osszetevo(osszetevok_eleje, beirt_osszetevo);
                    recept_hozzavaloinak_eleje = recept_hozzavalok_letrehozasa(recept_hozzavaloinak_eleje);

                    aktualis_recept->adat.eleje = recept_hozzavaloinak_eleje;
                    
                    printf("Mennyiseg:\n");
                    int mennyiseg = szam_beolvasas();
                    
                    char *mertekegyseg = NULL;
                    while (1) {
                        printf("Mertekegyseg: ");
                        mertekegyseg = osszetevo_beolvasas();
                        if (tartalmaz_betut(mertekegyseg)) {
                            break;
                        } else {
                            printf("Hiba: A mertekegyseg nem lehet ures vagy csak szam!\n");
                            free(mertekegyseg);
                        }
                    }

                    recept_hozzavaloinak_eleje->osszetevo_nev = (char *) malloc((strlen(beirt_osszetevo) + 1) * sizeof(char));
                    
                    strcpy(recept_hozzavaloinak_eleje->osszetevo_nev, beirt_osszetevo);
                    
                    recept_hozzavaloinak_eleje->mennyiseg = mennyiseg;
                    recept_hozzavaloinak_eleje->mertekegyseg = mertekegyseg;

                    free(beirt_osszetevo);
                    
                } else {
                    
                    //mar letezik ilyen hozzavalo
                    printf("letezik mar ilyen osszetevo!\n");
                    free(beirt_osszetevo);
                    break;
                }
                
            } else {
                
                break;
            }
        }
    }
    
    return aktualis_recept;
}

ReceptCsomopont *interakcio_uj_recept(ReceptCsomopont *receptek_eleje, Osszetevok *osszetevok_eleje) {
    printf(" UJ RECEPT LETREHOZASA\n");
    
    char *uj_recept_neve = NULL;
    while (1) {
        printf("Mi legyen az etel neve? (Legalabb egy betut tartalmaznia kell!)\n");
        uj_recept_neve = osszetevo_beolvasas();

        if (tartalmaz_betut(uj_recept_neve)) {
            break; //jo nevet irt be
        } else {
            printf("Hiba: A nev nem lehet csak szam vagy ures!\n");
            free(uj_recept_neve); //felszabaditjuk a rossz nevet
        }
    }
    
    bool letezik_recept = van_e_ilyen_recept(receptek_eleje, uj_recept_neve);

    if (!letezik_recept) {
        //nem létezik ilyen recept még
        receptek_eleje = recept_letrehozasa(receptek_eleje, uj_recept_neve);
        printf("Hozzavalok kovetkeznek!\n");
        ReceptHozzavalok *recept_hozzavaloinak_elejee = NULL;
        receptek_eleje = recept_hozzavalok_beolvasasa(receptek_eleje, osszetevok_eleje, recept_hozzavaloinak_elejee);
        return receptek_eleje;
    } else {
        //létezik mar ilyen recept
        printf("Letezik mar ilyen recept! Nem lehet letrehozni\n");
        free(uj_recept_neve);
        return receptek_eleje;
    }
}

//kimenet NULL ha nincs benne a receptek ben találat, ha van akkor vissza adja a kiválasztott recept pointerét, bemenet: receptek elejére mutató pointer
ReceptCsomopont *recept_kereses_nev_alapjan(ReceptCsomopont *receptek_eleje) {
    printf("Ird be a nevet amire keresnel.\n");
    char *keresett_nev = osszetevo_beolvasas(); //string beolvasása hogy mi alapjan keressen
    ReceptCsomopont *aktualis_recept = receptek_eleje;
    int talalatok_szama = 1;
    while (aktualis_recept != NULL) {
        if (strstr(aktualis_recept->adat.etel_nev, keresett_nev) != NULL) {
            //ha benne van az aktualis recept nevében a keresett string akkor nem NULL-t ad vissza (benne van)
            printf("%d. recept neve: %s\n", talalatok_szama, aktualis_recept->adat.etel_nev);
            talalatok_szama += 1;
        }
        aktualis_recept = aktualis_recept->kovetkezo;
    }

    if (talalatok_szama == 1) {
        //nem volt benne a listában egy olyan sem amire a felhasználó rá keresett
        printf("Nincs talalat!\n");
        free(keresett_nev); //mivel dinamikusan foglalt stringet ad vissza az osszetevo_beolvasas
        return NULL;
    }

    printf("Valassz sorszamot: ");
    int valasztott_sorszam = szam_beolvasas();
    while (valasztott_sorszam < 1 || valasztott_sorszam >= talalatok_szama) {
        //rosszat írt be
        printf("Hibas sorszam! Add meg ujra: ");
        valasztott_sorszam = szam_beolvasas();
    }

    int ujra_szamlalo = 0;
    ReceptCsomopont *aktualis_kereso_recept = receptek_eleje;
    while (aktualis_kereso_recept != NULL) {
        if (strstr(aktualis_kereso_recept->adat.etel_nev, keresett_nev) != NULL) {
            //ha az aktualis recept neve ben benne van a keresett string
            ujra_szamlalo += 1;
        }
        if (ujra_szamlalo == valasztott_sorszam) {
            //ez az ami kell
            free(keresett_nev); //mivel dinamikusan foglalt stringet ad vissza az osszetevo_beolvasas
            return aktualis_kereso_recept;
        }
        aktualis_kereso_recept = aktualis_kereso_recept->kovetkezo;
    }

    free(keresett_nev);
    return NULL;
}

ReceptCsomopont *recept_kereses_random(ReceptCsomopont *receptek_eleje) {
    int recept_darabszam = 0;
    ReceptCsomopont *aktualis_recept = receptek_eleje;
    while (aktualis_recept != NULL) {
        recept_darabszam += 1;
        aktualis_recept = aktualis_recept->kovetkezo;
    }

    //mennyi random receptet adjon. legalább 1 et ad max az összes receptet
    int sorsolando_darab = (rand() % recept_darabszam) + 1;
    //pointereket tároló tömb foglalás
    ReceptCsomopont **kisorsolt_receptek = (ReceptCsomopont **) malloc(sorsolando_darab * sizeof(ReceptCsomopont*));

    if (kisorsolt_receptek == NULL) {
        printf("Memoriahiba!\n");
        return NULL;
    }

    printf("Sorsoltam neked %d receptet:\n", sorsolando_darab);
    
    int sorszam_szamlalo = 1; //hanyadikat irjuk ki eppen
    int jelenlegi_lepes = 0; //mennyit leptunk idaig a listaban
    ReceptCsomopont *veletlen_receptt = receptek_eleje;

    while (sorszam_szamlalo <= sorsolando_darab) {
        int cel_index = (rand() % recept_darabszam); //ide akarunk a lepkedessel eljutni

        while (jelenlegi_lepes != cel_index) {
            veletlen_receptt = veletlen_receptt->kovetkezo;
            jelenlegi_lepes += 1;
        }

        bool mar_szerepelt = false;
        for (int i = 0; i < sorszam_szamlalo - 1; i++) {
            if (kisorsolt_receptek[i] == veletlen_receptt) {
                mar_szerepelt = true;
                break;
            }
        }
        //ha mar szerepelt akkor ujra az egeszet
        if (mar_szerepelt) {
            veletlen_receptt = receptek_eleje;
            jelenlegi_lepes = 0;
            continue;
        }

        jelenlegi_lepes = 0;
        printf("%d. recept neve: %s\n", sorszam_szamlalo, veletlen_receptt->adat.etel_nev);

        //itt kell megjegyezni a pointert ami a kiírt receptre mutat
        kisorsolt_receptek[sorszam_szamlalo - 1] = veletlen_receptt;

        veletlen_receptt = receptek_eleje;
        sorszam_szamlalo += 1;
    }

    //megvan hogy random recepteket ír ki sorszámozva ^^^^
    //felhasználó melyiket szeretné vvvv
    printf("Valassz sorszamot: ");
    int valasztott_sorszam = szam_beolvasas();
    ReceptCsomopont *valasztott_recept = NULL; //ez a vegeredmeny amit visszaadunk
    while (valasztott_sorszam < 1 || valasztott_sorszam >= sorszam_szamlalo) {
        //rosszat írt be
        printf("Hibas sorszam! Add meg ujra: ");
        valasztott_sorszam = szam_beolvasas();
    }

    valasztott_recept = kisorsolt_receptek[valasztott_sorszam - 1]; //ez az amit kiválasztott a felhasználó
    free(kisorsolt_receptek); //fel kell szabadítani mert dinamikusan lett foglalva
    return valasztott_recept;
}

ReceptCsomopont *recept_keres_egy_osszetevo(ReceptCsomopont *receptek_eleje, Osszetevok *osszetevok_eleje) {
    osszetevo_listaz(osszetevok_eleje);
    printf("Ird be a keresendo osszetevo nevet!\n");
    char *keresett_nev = osszetevo_beolvasas();

    ReceptCsomopont *aktualis_recept = receptek_eleje;
    int kovetkezo_sorszam = 1; //kiirasnal hasznalt szam
    while (aktualis_recept != NULL) {
        ReceptHozzavalok *aktualis_hozzavalo = aktualis_recept->adat.eleje;
        while (aktualis_hozzavalo != NULL) {
            //recepthez tartozo hozzavalokon vegigmenes
            if (strcmp(aktualis_hozzavalo->osszetevo_nev, keresett_nev) == 0) {
                //ha ez a hozzavalo amit keresünk akkor kiirja a receptet
                printf("%d. recept neve: %s\n", kovetkezo_sorszam, aktualis_recept->adat.etel_nev);
                kovetkezo_sorszam += 1;
                break; //ha pl az elso volt amit kerestünk akkor ne kelljen vegigmenni a többin foloslegesen
            }
            aktualis_hozzavalo = aktualis_hozzavalo->kovetkezo;
        }
        aktualis_recept = aktualis_recept->kovetkezo;
    }

    if (kovetkezo_sorszam == 1) {
        //nem volt benne a listában egy olyan sem amire a felhasználó rá keresett
        printf("Nincs talalat!\n");
        free(keresett_nev); //mivel dinamikusan foglalt stringet ad vissza az osszetevo_beolvasas
        return NULL;
    }

    printf("Valassz sorszamot: ");
    int kivant_sorszam = szam_beolvasas();
    while (kivant_sorszam < 1 || kivant_sorszam >= kovetkezo_sorszam) {
        //rosszat írt be
        printf("Hibas sorszam! Add meg ujra: ");
        kivant_sorszam = szam_beolvasas();
    }

    //most azt a receptet keressuk amit beirt a felhasznalo 
    ReceptCsomopont *aktualis_kereso_recept = receptek_eleje;
    int megtalalt_db = 0; //szémolja hol tartunk
    while (aktualis_kereso_recept != NULL) {
        ReceptHozzavalok *aktualis_kereso_hozzavalo = aktualis_kereso_recept->adat.eleje;

        while (aktualis_kereso_hozzavalo != NULL) {
            if (strcmp(aktualis_kereso_hozzavalo->osszetevo_nev, keresett_nev) == 0) {
                //ha a receptben a hozzavalo megegyezik a keresettel

                megtalalt_db += 1;

                if (megtalalt_db == kivant_sorszam) {
                    //ez a recept amit beirt a felhasznalo
                    free(keresett_nev); //mivel dinamikusan lett foglalva
                    return aktualis_kereso_recept; //receptre pointer
                }
                break; //ne kelljen a tobbi hozzavalon is vegigmenni ha mar volt olyan ami kell
            }

            aktualis_kereso_hozzavalo = aktualis_kereso_hozzavalo->kovetkezo;
        }
        aktualis_kereso_recept = aktualis_kereso_recept->kovetkezo;
    }

    //ide nem kellene eljutnia a kódnak de biztonságbol ide irtam
    free(keresett_nev);
    return NULL;
}

Osszetevok *osszetevo_kivalasztasa(Osszetevok *osszetevok_eleje, int keresett_sorszam) {
    int jelenlegi_sorszam = 1;
    Osszetevok *aktualis_osszetevo = osszetevok_eleje;

    while (aktualis_osszetevo != NULL) {
        if (jelenlegi_sorszam == keresett_sorszam) {
            return aktualis_osszetevo;
        }
        jelenlegi_sorszam += 1;
        aktualis_osszetevo = aktualis_osszetevo->kovetkezo;
    }

    return NULL; //nem talalta meg
}

ReceptCsomopont *recept_keres_tobb_osszetevo(ReceptCsomopont *receptek_eleje, Osszetevok *osszetevok_eleje) {
    printf("Melyik osszetevoket kell elhasznalni? Tobbet is megadhatsz, 0 beirasaval kilepsz az osszetevok beolvasasabol!\n");

    Osszetevok **keresett_osszetevok = NULL;
    osszetevo_listaz(osszetevok_eleje);

    int elerheto_osszetevok_db = osszetevok_db(osszetevok_eleje);
    int keresett_db = 0; //mennyi osszetevot kell keresni

    Osszetevok *kivalasztott_osszetevo;

    while (1) {
        //addig olvasunk be amig 0-t nem ad a felhasznalo
        
        int beolvasott_sorszam = szam_beolvasas();

        if (beolvasott_sorszam == 0) {
            break;
        }

        while (beolvasott_sorszam < 0 || beolvasott_sorszam > elerheto_osszetevok_db) {
            printf("A lathato sorszamokbol irj be egyet!\n");
            beolvasott_sorszam = szam_beolvasas();
        }

        kivalasztott_osszetevo = osszetevo_kivalasztasa(osszetevok_eleje, beolvasott_sorszam);
        keresett_db += 1;

        Osszetevok **osszetevo_temp = (Osszetevok **) realloc(keresett_osszetevok, keresett_db * sizeof(Osszetevok*)); //pointer tomb, átmeneti
        if (osszetevo_temp == NULL) {
            free(keresett_osszetevok);
            return NULL; // Memóriahiba
        }

        keresett_osszetevok = osszetevo_temp;
        keresett_osszetevok[keresett_db - 1] = kivalasztott_osszetevo;

        printf("Hozzaadva: %s\n", kivalasztott_osszetevo->osszetevo_nev);
    }
    //megvan a keresett osszetevok beolvasasa^^^^

    if (keresett_db == 0) {
        printf("Nem adtal meg egyet sem.\n");
        return NULL;
    }

    ReceptCsomopont **talalatok_tomb = NULL;

    ReceptCsomopont *aktualis_recept = receptek_eleje;
    int recept_talalatok_db = 0;
    int kiirasi_sorszam = 1;

    printf("Talalatok:\n");

    while (aktualis_recept != NULL) {
        ReceptHozzavalok *vizsgalt_hozzavalo = aktualis_recept->adat.eleje;
        int egyezesek_szama_recepten_belul = 0;

        while (vizsgalt_hozzavalo != NULL) {
            for (int i = 0; i < keresett_db; i++) {
                if (strcmp(vizsgalt_hozzavalo->osszetevo_nev, keresett_osszetevok[i]->osszetevo_nev) == 0) {
                    egyezesek_szama_recepten_belul += 1;
                    break; //ha megtalaltuk akkor nem kell vegig nezni a tobbivel
                }
            }

            vizsgalt_hozzavalo = vizsgalt_hozzavalo->kovetkezo;
        }

        if (egyezesek_szama_recepten_belul == keresett_db) {
            printf("%d. recept neve: %s\n", kiirasi_sorszam, aktualis_recept->adat.etel_nev);
            recept_talalatok_db += 1;
            kiirasi_sorszam += 1;

            ReceptCsomopont **recept_temp = (ReceptCsomopont **) realloc(talalatok_tomb, recept_talalatok_db*sizeof(ReceptCsomopont*));

            if (recept_temp == NULL) {
                free(keresett_osszetevok);
                free(talalatok_tomb);
                return NULL; //hiba tortent
            }

            //kiirt receptek elmentése
            talalatok_tomb = recept_temp;
            talalatok_tomb[recept_talalatok_db - 1] = aktualis_recept;
        }

        aktualis_recept = aktualis_recept->kovetkezo;
    }

    if (recept_talalatok_db == 0) {
        //egyet sem talalt
        free(keresett_osszetevok);
        return NULL;
    }

    printf("Valassz egy receptet!\n");
    int valaszott_sorszam = szam_beolvasas();

    while (valaszott_sorszam < 1 || valaszott_sorszam > recept_talalatok_db) {
        //rosszat írt be
        printf("Hibas sorszam! Add meg ujra: ");
        valaszott_sorszam = szam_beolvasas();
    }

    ReceptCsomopont *kivalasztott_recept = talalatok_tomb[valaszott_sorszam - 1];

    free(talalatok_tomb);
    free(keresett_osszetevok);

    return kivalasztott_recept;
}

void recept_megjelenit(const ReceptCsomopont *megjelenitendo_recept) {
    if (megjelenitendo_recept == NULL) {
        printf("Hiba tortent a recept megjelenitesenel!\n");
        return;
    }

    printf("\n");
    printf("  ___________________________________________________\n");
    printf(" /                                                   \\\n");
    printf("|                  RECEPT ADATLAP                     |\n");
    printf(" \\___________________________________________________/\n");

    printf("\n  ELNEVEZES: %s\n", megjelenitendo_recept->adat.etel_nev);
    printf("  ---------------------------------------------------\n");
    
    printf("  HOZZAVALOK:\n");

    ReceptHozzavalok *aktualis_hozzavalo = megjelenitendo_recept->adat.eleje;

    if (aktualis_hozzavalo == NULL) {
        printf("Nincsenek hozzavaloi ennek a receptnek!\n");
    }

    while (aktualis_hozzavalo != NULL) {
        printf("* %-20s  | %4d %-10s\n", aktualis_hozzavalo->osszetevo_nev, aktualis_hozzavalo->mennyiseg, aktualis_hozzavalo->mertekegyseg);
        aktualis_hozzavalo = aktualis_hozzavalo->kovetkezo;
    }

    printf("  ---------------------------------------------------\n");
    printf("  ELKESZITES:\n");
    
    if (megjelenitendo_recept->adat.elkeszites != NULL) {
        printf("%s\n", megjelenitendo_recept->adat.elkeszites);
    } else {
        printf("Nincs elkeszites modja!\n");
    }
    printf("  ---------------------------------------------------\n");
}

//receptet torli
void recept_torlese_segedfuggveny(ReceptCsomopont *torlendo_recept) {
    ReceptHozzavalok *aktualis_hozzavalo = torlendo_recept->adat.eleje;
    ReceptHozzavalok *elozo_hozzavalo = aktualis_hozzavalo;

    while (aktualis_hozzavalo != NULL) {
        aktualis_hozzavalo = aktualis_hozzavalo->kovetkezo;

        free(elozo_hozzavalo->osszetevo_nev);
        free(elozo_hozzavalo->mertekegyseg);
        free(elozo_hozzavalo);

        elozo_hozzavalo = aktualis_hozzavalo;
    }

    //most a receptben lővő dolgokat kell felszabaditanom
    free(torlendo_recept->adat.etel_nev);
    free(torlendo_recept->adat.elkeszites);
    free(torlendo_recept);
}

ReceptCsomopont *recept_torlese(ReceptCsomopont *torlendo_recept, ReceptCsomopont *receptek_eleje) {
    if (receptek_eleje == NULL || torlendo_recept == NULL) {
        return NULL;
    }
    //ha a torlendo recept az elso recept
    if (torlendo_recept == receptek_eleje) {
        receptek_eleje = receptek_eleje->kovetkezo;

        recept_torlese_segedfuggveny(torlendo_recept);
    }
    //nem az elso receptet kell törölni
    else {
        ReceptCsomopont *aktualis_recept = receptek_eleje;
        //eloszor meg kell keresni mit kell torolni
        ReceptCsomopont *elozo_recept = aktualis_recept;

        while (aktualis_recept != NULL) {
            if (aktualis_recept == torlendo_recept) {
                //megvan amit kell torolni
                //at kotjuk hogy ne legyen szakadas
                aktualis_recept = aktualis_recept->kovetkezo;
                elozo_recept->kovetkezo = aktualis_recept;


                recept_torlese_segedfuggveny(torlendo_recept);

                return receptek_eleje;
            } else {
                elozo_recept = aktualis_recept;

                aktualis_recept = aktualis_recept->kovetkezo;
            }
        }
    }

    return receptek_eleje;
}

void recept_hozzavalok_kiiras(ReceptHozzavalok *hozzavalok_eleje) {
    ReceptHozzavalok *aktualis_hozzavalo = hozzavalok_eleje;
    int hozzavalo_sorszam = 1;
    while (aktualis_hozzavalo != NULL) {
        printf("%d %s\n", hozzavalo_sorszam, aktualis_hozzavalo->osszetevo_nev);
        hozzavalo_sorszam += 1;
        aktualis_hozzavalo = aktualis_hozzavalo->kovetkezo;
    }
}

//visszaadja a memóriacímét a keresett hozzávalónak
ReceptHozzavalok *hozzavalo_megkeres(ReceptHozzavalok *hozzavalok_eleje, int keresett_sorszam) {
    int jelenlegi_sorszam = 1;
    ReceptHozzavalok *aktualis_hozzavalo = hozzavalok_eleje;
    while (aktualis_hozzavalo != NULL) {
        if (jelenlegi_sorszam == keresett_sorszam) {
            return aktualis_hozzavalo;
        }
        jelenlegi_sorszam += 1;
        aktualis_hozzavalo = aktualis_hozzavalo->kovetkezo;
    }
    return NULL;
}


ReceptCsomopont *recept_modositas(ReceptCsomopont *modositando_recept, ReceptCsomopont *receptek_eleje) {
    
    int valasztas;

    do {
        
        printf("\n========================================\n");
        printf("   RECEPT MODOSITASA: %s\n", modositando_recept->adat.etel_nev);
        printf("========================================\n");
        printf(" 1. Etel neve\n");
        printf(" 2. Elkeszites modja\n");
        printf(" 3. Hozzavalok kezelese\n");
        printf(" 0. Kilepes\n");
        printf("----------------------------------------\n");
        
        valasztas = szam_beolvasas();

        switch (valasztas) {
            case 0:
                break;

            case 1:

                printf("Az etel uj neve: ");
                char *uj_recept_nev = osszetevo_beolvasas();
                free(modositando_recept->adat.etel_nev);
                modositando_recept->adat.etel_nev = uj_recept_nev;
                break;

            case 2:

                printf("Az etel uj elkeszitesi modja: (enterrel leokezod)");
                char *uj_elkeszitesi_mod = osszetevo_beolvasas();
                free(modositando_recept->adat.elkeszites);
                modositando_recept->adat.elkeszites = uj_elkeszitesi_mod;
                break;

            case 3: {
                int hozzavalok_valasztas;
                printf("Hozzavalok modositasa.\n");
                while (1) {
                    
                    recept_hozzavalok_kiiras(modositando_recept->adat.eleje);

                    printf("Melyik hozzavalot szeretned modositani? 0 a kilepeshez, kivalasztashoz a sorszamot ird be.\n");
                    hozzavalok_valasztas = szam_beolvasas();

                    if (hozzavalok_valasztas == 0) {
                        break;
                    }

                    ReceptHozzavalok *kivalasztott_hozzavalo = hozzavalo_megkeres(modositando_recept->adat.eleje, hozzavalok_valasztas);
                    //ezt a hozzavalot akarja modositani a felhasznalo ^^^^

                    if (kivalasztott_hozzavalo == NULL) {
                        printf("Nincs ilyen sorszamu hozzavalo!\n");
                        continue; //ujra a sorszamot
                    }
                    
                    int hozzavalo_modositas_valasztas; 
                    do {
                        printf("Mit szeretnel modositani? 0 a kilepeshez, 1 a hozzavalo neve modositasahoz, 2 a mennyiseg modositasahoz, 3 a mertekegyseg modositasahoz: ");
                        hozzavalo_modositas_valasztas = szam_beolvasas();

                        switch (hozzavalo_modositas_valasztas) {
                            case 0:
                                break;

                            case 1:

                                printf("\n [ A HOZZAVALO UJ NEVE ]: ");
                                char *uj_osszetevo_nev = osszetevo_beolvasas();
                                free(kivalasztott_hozzavalo->osszetevo_nev);
                                kivalasztott_hozzavalo->osszetevo_nev = uj_osszetevo_nev;
                                break;

                            case 2:

                                printf("\n[ AZ UJ MENNYISEG ]: ");
                                int uj_osszetevo_mennyiseg = szam_beolvasas();
                                kivalasztott_hozzavalo->mennyiseg = uj_osszetevo_mennyiseg;
                                break;

                            case 3:

                                printf("\n[ A HOZZAVALO UJ MERTEKEGYSEGE ]: ");
                                char *uj_osszetevo_mertekegyseg = osszetevo_beolvasas();
                                free(kivalasztott_hozzavalo->mertekegyseg);
                                kivalasztott_hozzavalo->mertekegyseg = uj_osszetevo_mertekegyseg;
                                break;

                            default:
                                printf("Hibas sorszam!\n");
                                break;
                        }
                    } while (hozzavalo_modositas_valasztas != 0);
                }
                break;
            }
            default:
                printf("Hibas sorszam!\n");
                break;
        }
    } while (valasztas != 0);

    return receptek_eleje;
}

void kereses_menu_kiiras() {

    printf("----------------------------------------\n");
    printf("\n   --- KERESESI SZUROK ---\n");
    printf("   [1] Recept neve alapjan\n");
    printf("   [2] Veletlenszeru ajanlat (Nincs otletem)\n");
    printf("   [3] Egy osszetevo alapjan (Mihez van...)\n");
    printf("   [4] Tobb osszetevo alapjan (El kell hasznalni)\n");
    printf("   -----------------------\n");
    printf("   [0] Vissza a fomenube\n");
    printf("----------------------------------------\n");
    printf("   Valasztas: ");
    
    
}

ReceptCsomopont *kereses_seged_kiiras(ReceptCsomopont *kereses_eredmenye,ReceptCsomopont *receptek_eleje) {
    if (kereses_eredmenye==NULL) {
        return receptek_eleje;
    }
    
    recept_megjelenit(kereses_eredmenye);

    int kereses_sorszam;
    do {
        printf("----------------------------------------\n");
        printf("0. Kilepes\n");
        printf("1. Recept torlese\n");
        printf("2. Recept modositasa\n");
        printf("----------------------------------------\n");
                    
        kereses_sorszam=szam_beolvasas();

        switch (kereses_sorszam) {
            case 0:
                break;

            case 1:
                receptek_eleje=recept_torlese(kereses_eredmenye,receptek_eleje);
                printf("Sikeresen kitorlodott ez a recept!\n");
                return receptek_eleje;

            case 2:
                receptek_eleje=recept_modositas(kereses_eredmenye,receptek_eleje);
                printf("Sikeresen modositottad ezt a receptet!\n");
                printf("A modosult recept: \n");
                recept_megjelenit(kereses_eredmenye);
                break;

            default:
                printf("Hibas sorszam!\n");
                break;
        }
    }while (kereses_sorszam!=0);
    
    return receptek_eleje;
}

ReceptCsomopont *interakcio_kereses(ReceptCsomopont *receptek_eleje, Osszetevok *osszetevok_eleje) {
    
    int valasztott_kereses;
    do {
        
        kereses_menu_kiiras();
        
        valasztott_kereses=szam_beolvasas();

        ReceptCsomopont *aktualis_talalat = NULL;

        switch (valasztott_kereses) {
            
            case 0:
                break;

            case 1:
                
                
                aktualis_talalat=recept_kereses_nev_alapjan(receptek_eleje);
                
                receptek_eleje=kereses_seged_kiiras(aktualis_talalat,receptek_eleje);
                break;
                
            case 2:
                
                aktualis_talalat=recept_kereses_random(receptek_eleje);

                receptek_eleje=kereses_seged_kiiras(aktualis_talalat,receptek_eleje);
                break;

            case 3:
                
                aktualis_talalat=recept_keres_egy_osszetevo(receptek_eleje,osszetevok_eleje);
                
                receptek_eleje=kereses_seged_kiiras(aktualis_talalat,receptek_eleje);
                break;

            case 4:
                
                aktualis_talalat=recept_keres_tobb_osszetevo(receptek_eleje,osszetevok_eleje);

                receptek_eleje=kereses_seged_kiiras(aktualis_talalat,receptek_eleje);
                break;

            default:
                printf("Hibas sorszam!\n");
                break;
        }
        
    }while (valasztott_kereses!=0);
    return receptek_eleje;
}

//1000. sor :D