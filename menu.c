

#include "menu.h"
#include "receptek.h"
#include "debugmalloc.h"

void menu(Osszetevok **osszetevok_ptre,ReceptCsomopont **receptek_ptr) {
    int menupont;
    do {
        
        printf("\n");
        printf("========================================\n");
        printf("       RECEPTKONYV KEZELOFELULET        \n");
        printf("========================================\n");
        printf(" [1] Uj osszetevo felvetele\n");
        printf(" [2] Osszetevok listazasa\n");
        printf(" [3] Osszetevo torlese\n");
        printf("----------------------------------------\n");
        printf(" [4] Uj recept letrehozasa\n");
        printf(" [5] KERESES a receptek kozott\n");
        printf("----------------------------------------\n");
        printf(" [0] KILEPES es MENTES\n");
        printf("========================================\n");
        
        menupont = szam_beolvasas();

        switch (menupont) {
            
            case 0:
                break;
                
            case 1:
            {
                char *uj_nev = NULL;
                
                while (1) {
                    printf("Uj hozzavalo neve: ");
                    uj_nev = osszetevo_beolvasas();
                    //van benne legalabb egy betu?
                    if (tartalmaz_betut(uj_nev)) {
                        break; 
                    } else {
                        printf("Hiba: A nev nem lehet ures vagy csak szam!\n");
                        free(uj_nev); //a rosszat fel kell szabaditani
                    }
                }

                *osszetevok_ptre = interakcio_uj_osszetevo(*osszetevok_ptre, uj_nev);
                free(uj_nev);
                break;
            }
                
            case 2:
                
                osszetevo_listaz(*osszetevok_ptre);
                break;
                
            case 3:
                
                *osszetevok_ptre=interakcio_osszetevo_torles(*osszetevok_ptre,*receptek_ptr);
                break;
                
            case 4:
                
                *receptek_ptr=interakcio_uj_recept(*receptek_ptr,*osszetevok_ptre);
                break;

            case 5:
                
                *receptek_ptr=interakcio_kereses(*receptek_ptr,*osszetevok_ptre);
                break;
                
            default:
                
                printf("Hibas sorszam!\n");
                break;
        }
        printf("\n");
    } while (menupont != 0);
}
