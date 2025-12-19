
#include "fajlkezelo.h"
#include <string.h>
#include "osszetevok.h"
#include "stdlib.h"
#include "debugmalloc.h"
#include <stdlib.h>

void osszetevok_mentese_fajlba(Osszetevok *osszetevok_lista) {
    FILE *fajl_mutato;
    fajl_mutato = fopen("osszetevok.txt", "w"); //osszetevok.txt letrehozasa
    if (fajl_mutato == NULL) {
        //ellenorzes hogy sikerult e a letrehozas
        perror("osszetevok fajl megnyitasa sikertelen");
        return;
    }
    //osszetevok lista bejarasa
    for (Osszetevok *mozgo = osszetevok_lista; mozgo != NULL; mozgo = mozgo->kovetkezo) {
        fprintf(fajl_mutato, "%s\n", mozgo->osszetevo_nev);
    }
    fclose(fajl_mutato); //osszetevok fajl bezarasa
}

Osszetevok *osszetevok_beolvasasa_fajlbol(char *fajlnev) {
    
    FILE *osszetevok_fajl = fopen(fajlnev, "r");
    if (osszetevok_fajl == NULL) {
        printf("Nem sikerult az osszetevok fajl beolvasasa\n");
        return NULL;
    }
    
    Osszetevok *eleje = NULL;
    Osszetevok *utolso=NULL;
    
    char sor_puffer[68]; //ideiglenes tároló egy sornak, leghosszabb magyar szó 67 betűs ezért ebbe bármilyen szó elfár
    while (fgets(sor_puffer, sizeof(sor_puffer), osszetevok_fajl) != NULL) {
        
        sor_puffer[strcspn(sor_puffer, "\n")] = 0; //\n levágása
        
        Osszetevok *uj_osszetevo = (Osszetevok *) malloc(sizeof(Osszetevok));
        
        uj_osszetevo->osszetevo_nev = (char *) malloc((strlen(sor_puffer) + 1) * sizeof(char));
        strcpy(uj_osszetevo->osszetevo_nev, sor_puffer);

        uj_osszetevo->kovetkezo = NULL;
        
        if (eleje==NULL) {
            eleje=uj_osszetevo;
            utolso=uj_osszetevo;
        }
        else {
            utolso->kovetkezo=uj_osszetevo;
            utolso=uj_osszetevo;
        }
    }
    
    fclose(osszetevok_fajl);
    return eleje;
    
}

char *fajl_sor_beolvasas(FILE *fajlmutato) {
    if (fajlmutato == NULL) //ha nincs fajl akkor mibol olvasna be?
        return NULL;
    char *sor = (char *) malloc(sizeof(char)); //lezaro nullanak csinal csak helyet eloszor
    if (sor == NULL) //leellenorzes hogy sikerult e
        return NULL;

    int karakter_meret = 0;
    sor[0] = '\0';
    int beolvasott;

    while ((beolvasott = fgetc(fajlmutato)) != EOF) {
        //addig olvas be amig nem a fajl vege
        if (beolvasott == '\n')
            break;

        char *temp = (char *) realloc(sor, (karakter_meret+1+1)*sizeof(char)); //jelenlegi betuk szama + uj betunek hely + lezaro nullanak
        if (temp == NULL) {
            free(sor); //ha nemsikerult a realloc akkor nem tudta automatikusan felszabaditani az addig levo sort es ezert manualisan kell
            return NULL;
        }
        sor = temp;

        sor[karakter_meret] = (char) beolvasott; //kasztolás mert egy int es nekunk char kell
        sor[karakter_meret + 1] = '\0';
        karakter_meret += 1;
    }
    //ha nem olvasott be semmit (mert ures a fajl) akkor NULL-ot ad vissza
    if (karakter_meret == 0 && beolvasott == EOF) {
        free(sor); //fel kell szabaditani mert lefoglaltunk neki egy bajtot ('\0' karakter)
        return NULL;
    }

    return sor;
}

ReceptCsomopont *receptek_beolvasasa_fajlbol(char *fajlnev) {
    
    FILE *file_mutato = fopen(fajlnev, "r");
    if (file_mutato == NULL) {
        return NULL;
    }

    ReceptCsomopont *eleje = NULL;
    ReceptCsomopont *utolso_recept = NULL;

    char *sor;
    
    while ((sor = fajl_sor_beolvasas(file_mutato)) != NULL) {
        
        //ures sorok
        if (strlen(sor) == 0) {
            free(sor); //fel kell szabaditani
            continue;
        }

        //uj recept
        ReceptCsomopont *uj_recept = (ReceptCsomopont *)malloc(sizeof(ReceptCsomopont));
        uj_recept->adat.eleje = NULL;
        uj_recept->kovetkezo = NULL;

        //feldarabolas

        //etel neve
        char *etel_nev_mezo = strtok(sor, ";");
        if (etel_nev_mezo == NULL) {
            free(uj_recept); 
            free(sor); 
            continue; 
        }
        
        uj_recept->adat.etel_nev = (char *)malloc((strlen(etel_nev_mezo) + 1) * sizeof(char));
        strcpy(uj_recept->adat.etel_nev, etel_nev_mezo);

        //elkeszites modja
        char *elkeszites_mezo = strtok(NULL, ";");
        
        if (elkeszites_mezo != NULL) {
            uj_recept->adat.elkeszites = (char *)malloc((strlen(elkeszites_mezo) + 1) * sizeof(char));
            strcpy(uj_recept->adat.elkeszites, elkeszites_mezo);
            
        } else {
            uj_recept->adat.elkeszites = (char *)malloc(sizeof(char));
            *uj_recept->adat.elkeszites = '\0';
        }

        //hozzavalok
        ReceptHozzavalok *utolso_hozzavalo = NULL; 

        while (1) {
            
            char *hozzavalo_nev = strtok(NULL, ";");
            if (hozzavalo_nev == NULL) break; 

            char *mennyiseg_str = strtok(NULL, ";");
            char *mertekegyseg = strtok(NULL, ";");

            if (mennyiseg_str != NULL && mertekegyseg != NULL) {
                ReceptHozzavalok *uj_hozzavalo = (ReceptHozzavalok *)malloc(sizeof(ReceptHozzavalok));
                
                uj_hozzavalo->osszetevo_nev = (char *)malloc((strlen(hozzavalo_nev) + 1) * sizeof(char));
                strcpy(uj_hozzavalo->osszetevo_nev, hozzavalo_nev);

                uj_hozzavalo->mennyiseg = atoi(mennyiseg_str);

                uj_hozzavalo->mertekegyseg = (char *)malloc((strlen(mertekegyseg) + 1) * sizeof(char));
                strcpy(uj_hozzavalo->mertekegyseg, mertekegyseg);

                uj_hozzavalo->kovetkezo = NULL;

                //vegere fuzes
                if (uj_recept->adat.eleje == NULL) {
                    //elso hozzavalo
                    uj_recept->adat.eleje = uj_hozzavalo;
                    utolso_hozzavalo = uj_hozzavalo;
                } else {
                    //nem az elso hozzavalo
                    utolso_hozzavalo->kovetkezo = uj_hozzavalo;
                    utolso_hozzavalo = uj_hozzavalo;
                }
            }
        }

        //recept befuzese a vegere a listanak
        if (eleje == NULL) {
            eleje = uj_recept;
            utolso_recept = uj_recept;
        } else {
            utolso_recept->kovetkezo = uj_recept;
            utolso_recept = uj_recept;
        }
        
        free(sor); 
    }

    fclose(file_mutato);
    return eleje;
}

void receptek_mentese_fajlba(ReceptCsomopont *eleje) {
    
    FILE *file_mutato = fopen("receptek.csv", "w"); 
    if (file_mutato == NULL) {
        perror("Receptek fajl megnyitasa sikertelen");
        return;
    }

    ReceptCsomopont *aktualis_recept = eleje;
    while (aktualis_recept != NULL) {
        //nev, elkeszites
        fprintf(file_mutato, "%s;%s", aktualis_recept->adat.etel_nev, aktualis_recept->adat.elkeszites);

        //hozzavalok: nev, mennyiseg, mertekegyseg formaban
        ReceptHozzavalok *hozzavalo = aktualis_recept->adat.eleje;
        while (hozzavalo != NULL) {
            fprintf(file_mutato, ";%s;%d;%s", hozzavalo->osszetevo_nev, hozzavalo->mennyiseg, hozzavalo->mertekegyseg);
            hozzavalo = hozzavalo->kovetkezo;
        }
        
        fprintf(file_mutato, "\n"); //sornak vege van
        aktualis_recept = aktualis_recept->kovetkezo;
    }
    fclose(file_mutato);
}

