#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Osoba {
    char ime[50];
    char prezime[50];
    int godina_rodjenja;
    struct Osoba* sljedeci;
} Osoba;

void unosOsobe(Osoba* nova) {
    printf("Unesite ime: ");
    scanf("%49s", nova->ime);

    printf("Unesite prezime: ");
    scanf("%49s", nova->prezime);

    printf("Unesite godinu rodenja: ");
    scanf("%d", &nova->godina_rodjenja);
}

Osoba* dodajNaPocetak(Osoba* glava) {
    Osoba* nova = malloc(sizeof(Osoba));

    if (nova == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return glava;
    }

    unosOsobe(nova);
    nova->sljedeci = glava;

    return nova;
}

void ispisiListu(Osoba* glava) {
    if (glava == NULL) {
        printf("Lista je prazna.\n");
        return;
    }

    Osoba* tekuci = glava;

    while (tekuci != NULL) {
        printf("Ime: %s, Prezime: %s, Godina rodenja: %d\n",
            tekuci->ime,
            tekuci->prezime,
            tekuci->godina_rodjenja);

        tekuci = tekuci->sljedeci;
    }
}

Osoba* dodajNaKraj(Osoba* glava) {
    Osoba* nova = malloc(sizeof(Osoba));

    if (nova == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return glava;
    }

    unosOsobe(nova);
    nova->sljedeci = NULL;

    if (glava == NULL)
        return nova;

    Osoba* tekuci = glava;

    while (tekuci->sljedeci != NULL)
        tekuci = tekuci->sljedeci;

    tekuci->sljedeci = nova;

    return glava;
}

Osoba* pronadiOsobu(Osoba* glava, const char* prezime) {
    Osoba* tekuci = glava;

    while (tekuci != NULL) {
        if (strcmp(tekuci->prezime, prezime) == 0)
            return tekuci;

        tekuci = tekuci->sljedeci;
    }

    return NULL;
}

Osoba* obrisiOsobu(Osoba* glava, const char* prezime) {
    Osoba* tekuci = glava;
    Osoba* prethodni = NULL;

    while (tekuci != NULL &&
        strcmp(tekuci->prezime, prezime) != 0) {
        prethodni = tekuci;
        tekuci = tekuci->sljedeci;
    }

    if (tekuci == NULL) {
        printf("Osoba nije pronadena.\n");
        return glava;
    }

    if (prethodni == NULL)
        glava = tekuci->sljedeci;
    else
        prethodni->sljedeci = tekuci->sljedeci;

    free(tekuci);
    printf("Osoba je obrisana.\n");

    return glava;
}

Osoba* dodajIza(Osoba* glava, const char* prezime) {
    Osoba* trazena = pronadiOsobu(glava, prezime);

    if (trazena == NULL) {
        printf("Osoba nije pronadena.\n");
        return glava;
    }

    Osoba* nova = malloc(sizeof(Osoba));

    if (nova == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return glava;
    }

    unosOsobe(nova);

    nova->sljedeci = trazena->sljedeci;
    trazena->sljedeci = nova;

    return glava;
}

Osoba* dodajIspred(Osoba* glava, const char* prezime) {
    Osoba* tekuci = glava;
    Osoba* prethodni = NULL;

    while (tekuci != NULL &&
        strcmp(tekuci->prezime, prezime) != 0) {
        prethodni = tekuci;
        tekuci = tekuci->sljedeci;
    }

    if (tekuci == NULL) {
        printf("Osoba nije pronadena.\n");
        return glava;
    }

    Osoba* nova = malloc(sizeof(Osoba));

    if (nova == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return glava;
    }

    unosOsobe(nova);
    nova->sljedeci = tekuci;

    if (prethodni == NULL)
        return nova;

    prethodni->sljedeci = nova;

    return glava;
}

Osoba* sortirajListu(Osoba* glava) {
    Osoba pomocna;
    Osoba* j, * prethodni, * temp, * kraj;

    pomocna.sljedeci = glava;
    kraj = NULL;

    while (pomocna.sljedeci != kraj) {
        prethodni = &pomocna;
        j = prethodni->sljedeci;

        while (j->sljedeci != kraj) {
            if (strcmp(j->prezime, j->sljedeci->prezime) > 0) {
                temp = j->sljedeci;
                prethodni->sljedeci = temp;
                j->sljedeci = temp->sljedeci;
                temp->sljedeci = j;

                j = temp;
            }

            prethodni = j;
            j = j->sljedeci;
        }

        kraj = j;
    }

    printf("Lista je sortirana po prezimenima.\n");

    return pomocna.sljedeci;
}

void upisiUDatoteku(Osoba* glava) {
    FILE* dat = fopen("osoba.txt", "w");

    if (dat == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return;
    }

    Osoba* tekuci = glava;

    while (tekuci != NULL) {
        fprintf(dat, "%s %s %d\n",
            tekuci->ime,
            tekuci->prezime,
            tekuci->godina_rodjenja);

        tekuci = tekuci->sljedeci;
    }

    fclose(dat);
    printf("Lista je spremljena u datoteku osoba.txt.\n");
}

void oslobodiListu(Osoba* glava) {
    while (glava != NULL) {
        Osoba* zaBrisanje = glava;
        glava = glava->sljedeci;
        free(zaBrisanje);
    }
}

Osoba* ucitajIzDatoteke(Osoba* glava) {
    FILE* dat = fopen("osoba.txt", "r");

    if (dat == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return glava;
    }

    oslobodiListu(glava);
    glava = NULL;

    Osoba* zadnji = NULL;

    while (1) {
        Osoba* nova = malloc(sizeof(Osoba));

        if (nova == NULL) {
            printf("Greska pri alokaciji memorije!\n");
            break;
        }

        if (fscanf(dat, "%49s %49s %d",
            nova->ime,
            nova->prezime,
            &nova->godina_rodjenja) != 3) {
            free(nova);
            break;
        }

        nova->sljedeci = NULL;

        if (glava == NULL)
            glava = nova;
        else
            zadnji->sljedeci = nova;

        zadnji = nova;
    }

    fclose(dat);

    printf("Lista je ucitana iz datoteke osoba.txt.\n");

    return glava;
}

int main(void) {
    Osoba* glava = NULL;
    Osoba* pronadena;
    char prezime[50];
    int izbor;

    do {
        printf("\n--- IZBORNIK ---\n");
        printf("1. Dodaj osobu na pocetak\n");
        printf("2. Ispisi listu\n");
        printf("3. Dodaj osobu na kraj\n");
        printf("4. Pronadi osobu po prezimenu\n");
        printf("5. Obrisi osobu po prezimenu\n");
        printf("6. Dodaj osobu iza odredene osobe\n");
        printf("7. Dodaj osobu ispred odredene osobe\n");
        printf("8. Sortiraj listu po prezimenu\n");
        printf("9. Upisi listu u datoteku\n");
        printf("10. Ucitaj listu iz datoteke\n");
        printf("0. Izlaz\n");
        printf("Odabir: ");

        if (scanf("%d", &izbor) != 1) {
            printf("Neispravan unos.\n");
            break;
        }

        switch (izbor) {
        case 1:
            glava = dodajNaPocetak(glava);
            break;

        case 2:
            ispisiListu(glava);
            break;

        case 3:
            glava = dodajNaKraj(glava);
            break;

        case 4:
            printf("Unesite prezime za pretragu: ");
            scanf("%49s", prezime);

            pronadena = pronadiOsobu(glava, prezime);

            if (pronadena != NULL) {
                printf("Ime: %s, Prezime: %s, Godina rodenja: %d\n",
                    pronadena->ime,
                    pronadena->prezime,
                    pronadena->godina_rodjenja);
            }
            else {
                printf("Osoba nije pronadena.\n");
            }
            break;

        case 5:
            printf("Unesite prezime za brisanje: ");
            scanf("%49s", prezime);
            glava = obrisiOsobu(glava, prezime);
            break;

        case 6:
            printf("Iza koje osobe dodati novu osobu? Prezime: ");
            scanf("%49s", prezime);
            glava = dodajIza(glava, prezime);
            break;

        case 7:
            printf("Ispred koje osobe dodati novu osobu? Prezime: ");
            scanf("%49s", prezime);
            glava = dodajIspred(glava, prezime);
            break;

        case 8:
            glava = sortirajListu(glava);
            break;

        case 9:
            upisiUDatoteku(glava);
            break;

        case 10:
            glava = ucitajIzDatoteke(glava);
            break;

        case 0:
            printf("Kraj programa.\n");
            break;

        default:
            printf("Nepostojeca opcija.\n");
        }

    } while (izbor != 0);

    oslobodiListu(glava);

    return 0;
}