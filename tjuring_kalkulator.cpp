#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <direct.h>

struct Operacija
{
    int prvi_broj;
    int drugi_broj;
    char OP; // '+', '-'
    int rezultat;
    struct Operacija* sledeci;
};

// Funkcija za promenu boje teksta u konzoli
void boja(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Dodavanje operacije u listu
void dodaj_operaciju(struct Operacija** glava, int a, int b, char op, int rez)
{
    struct Operacija* novi = (struct Operacija*)malloc(sizeof(struct Operacija));
    if (!novi) {
        printf("Greska u alokaciji memorije!\n");
        exit(1);
    }
    novi->prvi_broj = a;
    novi->drugi_broj = b;
    novi->OP = op;
    novi->rezultat = rez;
    novi->sledeci = NULL;

    if (*glava == NULL)
        *glava = novi;
    else
    {
        struct Operacija* trenutni = *glava;
        while (trenutni->sledeci != NULL)
            trenutni = trenutni->sledeci;
        trenutni->sledeci = novi;
    }
}

// Prikaz istorije operacija u boji
void prikazi_listu(struct Operacija* glava)
{
    struct Operacija* trenutni = glava;
    printf("\n--- ISTORIJA OPERACIJA ---\n");

    while (trenutni != NULL)
    {
        boja(14); // brojevi žuti
        printf("%d ", trenutni->prvi_broj);
        boja(12); // operator crveni
        printf("%c ", trenutni->OP);
        boja(14);
        printf("%d ", trenutni->drugi_broj);
        boja(10); // rezultat zeleni
        printf("= %d\n", trenutni->rezultat);
        trenutni = trenutni->sledeci;
    }

    boja(15); // reset boje
}

// Snimanje istorije u datoteku
void sacuvaj_u_datoteku(struct Operacija* glava)
{
    FILE* fajl = fopen("istorija.txt", "w");
    if (!fajl)
    {
        printf("Greska pri otvaranju datoteke.\n");
        return;
    }

    struct Operacija* trenutni = glava;

    while (trenutni != NULL)
    {
        fprintf(fajl, "%d %c %d %d\n",
            trenutni->prvi_broj,
            trenutni->OP,
            trenutni->drugi_broj,
            trenutni->rezultat);

        trenutni = trenutni->sledeci;
    }

    fclose(fajl);
    printf("Operacije su sacuvane u datoteku.\n");
}

void obrisi_istoriju(struct Operacija** glava)
{
    struct Operacija* trenutni;

    while (*glava != NULL)
    {
        trenutni = *glava;
        *glava = (*glava)->sledeci;
        free(trenutni);
    }
}


// Ucitavanje istorije iz datoteke
void ucitaj_iz_datoteke(struct Operacija** glava)
{
    FILE* fajl = fopen("istorija.txt", "r");
    if (!fajl)
    {
        printf("Datoteka ne postoji.\n");
        return;
    }

    obrisi_istoriju(glava);  // brisanje stare liste

    int a, b, r;
    char op;

    while (fscanf(fajl, "%d %c %d %d", &a, &op, &b, &r) != EOF)
        dodaj_operaciju(glava, a, b, op, r);
    fclose(fajl);

    char* putanja = _getcwd(NULL, 0);
    if (putanja != NULL)
    {
        printf("Operacije ucitane iz datoteke. Trenutni folder: %s\n", putanja);
        free(putanja);
    }
}

void ispisi_ascii_traku(char traka[])
{
    for (int i = 0; traka[i] != '\0'; i++)
    {
        boja(11);
        printf("|");

        if (traka[i] == '1')
        {
            boja(14); // zuta
            printf("1");
        }
        else if (traka[i] == '0')
        {
            boja(12); // crvena
            printf("0");
        }
        else if (traka[i] == '+')
        {
            boja(9); // plava
            printf("+");
        }
        else if (traka[i] == '-')
        {
            boja(13); // ljubicasta
            printf("-");
        }

        boja(11); // reset na svetlo plavu
    }

    printf("|\n");
}

void sledeci_korak()
{
    printf("Pritisnite ENTER za sledece stanje...");
    getchar();
}

void obrisi_datoteku()
{
    if (remove("istorija.txt") == 0)
        printf("Datoteka je uspesno obrisana.\n");
    else
        printf("Datoteka ne postoji ili nije moguce obrisati.\n");
}

void prirucnik()
{
    boja(11);
    printf("\n--- PRIRUCNIK ---\n");
    boja(15);

    printf("Ovaj program simulira rad osnovne Tjuringove masine.\n");
    printf("Brojevi se predstavljaju unarnim zapisom pomocu simbola '1'.\n");
    printf("Operacije sabiranja i oduzimanja izvode se manipulacijom trake.\n\n");

    printf("1 -> Sabiranje brojeva\n");
    printf("2 -> Oduzimanje brojeva\n");
    printf("...\n");

    printf("\nPritisnite ENTER za povratak u meni...");
    getchar();
}

void ispisi_ascii_traku_sa_glavom(char traka[], int pozicija) {
    ispisi_ascii_traku(traka); // crta traku
    if (pozicija >= 0) {
        for (int i = 0; i < pozicija; i++)
            printf("  ");
        boja(12);
        printf(" ^\n");
        boja(11);
    }
}

void prikazi_stanje(char traka[], int pozicija, int* stanje)
{
    boja(5);
    printf("\nq%d stanje:\n", (*stanje)++);
    ispisi_ascii_traku_sa_glavom(traka, pozicija);
    sledeci_korak();
}

void simuliraj_sabiranje(char traka[], int* rez, char nova_traka[])
{
    int stanje = 0;
    int i;

    int plus = -1;
    for (i = 0; traka[i] != '\0'; i++)
        if (traka[i] == '+') { plus = i; break; }

    int granica = plus + 1;
    while (traka[granica] != '0')
        granica++;

    int pozicija_glave = 0;

    prikazi_stanje(traka, pozicija_glave, &stanje);

    while (1)
    {
        int poslednja_jedinica = -1;

        for (i = plus + 1; traka[i] != '0'; i++)
            if (traka[i] == '1')
                poslednja_jedinica = i;

        if (poslednja_jedinica == -1)
            break;

        int kraj = plus + 1;
        while (traka[kraj] != '0')
            kraj++;

        while (pozicija_glave < kraj)
        {
            pozicija_glave++;
            prikazi_stanje(traka, pozicija_glave, &stanje);
        }

        pozicija_glave--;
        prikazi_stanje(traka, pozicija_glave, &stanje);

        traka[pozicija_glave] = '0';
        prikazi_stanje(traka, pozicija_glave, &stanje);

        memmove(&traka[plus + 1], &traka[plus], strlen(traka) - plus + 1);
        traka[plus] = '1';
        plus++;

        int j = 0;
        for (i = 0; traka[i] != '\0'; i++)
            if (traka[i] == '1')
                nova_traka[j++] = '1';

        nova_traka[j] = '\0';
        *rez = j;
    }
}

void simuliraj_oduzimanje(char traka[], int a, int b, int* rez, char nova_traka[])
{
    int i;

         if (b > a)
         {
             printf("GRESKA: Negativan rezultat nije dozvoljen!\n");
             printf("Pritisnite ENTER za povratak u meni...");
             getchar();
             return;
         }

         int stanje = 0;
         printf("\nSimulacija oduzimanja:\n");

         int minus = -1;
         for (i = 0; traka[i] != '\0'; i++)
             if (traka[i] == '-') { minus = i; break; }

         // pronalazi granicu (prva nula na kraju drugog broja)
         int granica = minus + 1;
         while (traka[granica] != '0')
             granica++;

         // Pozicija glave startuje na prvoj jedinici prvog broja
         int pozicija_glave = 0;
         while (traka[pozicija_glave] != '1') pozicija_glave++;

         prikazi_stanje(traka, pozicija_glave, &stanje);

         int prvi_prolaz = 1;

         while (1)
         {
             // 1. Pronalazi poslednju jedinicu drugog broja (s desna)
             int poslednja_jedinica_drugi = -1;
             for (i = granica - 1; i > minus; i--)
             {
                 if (traka[i] == '1')
                 {
                     poslednja_jedinica_drugi = i;
                     break;
                 }
             }

             if (poslednja_jedinica_drugi == -1)
                 break; // nema više jedinica u drugom broju

             // 2. Pomera glavu desno kroz ceo prvi broj, znak i drugi broj do te jedinice
             int cilj;

             if (prvi_prolaz)
             {
                 cilj = granica; // ide do nule
                 prvi_prolaz = 0;
             }
             else
             {
                 cilj = poslednja_jedinica_drugi; // kasnije samo do jedinice
             }

             while (pozicija_glave < cilj)
             {
                 pozicija_glave++;
                 prikazi_stanje(traka, pozicija_glave, &stanje);
             }

             // Ako smo prvi put, stojimo na granici pa idemo levo
             if (cilj == granica)
             {
                 pozicija_glave--; // na poslednju jedinicu

                 prikazi_stanje(traka, pozicija_glave, &stanje);
             }

             // 3. Pretvara tu jedinicu drugog broja u nulu
             traka[pozicija_glave] = '0';
             prikazi_stanje(traka, pozicija_glave, &stanje);

             // 4. Vraca glavu na sledecu jedinicu prvog broja s leva
             int sledeca_prva = 0;
             while (sledeca_prva < minus && traka[sledeca_prva] != '1') sledeca_prva++;
             if (sledeca_prva < minus)
             {
                 pozicija_glave = sledeca_prva;
                 traka[pozicija_glave] = '0'; // brise tu jedinicu prvog broja
                 prikazi_stanje(traka, pozicija_glave, &stanje);
             }

             // ide levo do pocetka trake
             pozicija_glave = sledeca_prva;
         }

         // Konacni rezultat (samo preostale jedinice prvog broja)
         char rezultat[256] = "";
         int j = 0;
         for (i = 0; i != minus; i++)
             if (traka[i] == '1')
                 rezultat[j++] = '1';
         rezultat[j] = '\0';

         printf("\nq%d stanje:\n", stanje++);
         ispisi_ascii_traku_sa_glavom(rezultat, -1);
         sledeci_korak();

         *rez = j;
         strcpy(nova_traka, rezultat);
         }

// Animacija Tjuring trake - sabiranje i oduzimanje
         void simuliraj_traku(int a, int b, char op, struct Operacija** glava)
         {
             int rez;
             char nova_traka[256];

             char traka[256] = "";
             int i;

             for (i = 0; i < a; i++)
                 strcat(traka, "1");

             strcat(traka, (op == '+') ? "+" : "-");

             for (i = 0; i < b; i++)
                 strcat(traka, "1");

             strcat(traka, "0");

             printf("\nPocetna traka: %s\n", traka);

             if (op == '+')
                 simuliraj_sabiranje(traka, &rez, nova_traka);
             else
                 simuliraj_oduzimanje(traka, a, b, &rez, nova_traka);


    boja(11);
    printf("\n--- KRAJ SIMULACIJE ---\n");
    boja(15);

    boja(10);
    printf("\nKrajnja traka:\n");
    boja(15);
    ispisi_ascii_traku(nova_traka);
    boja(3);
    printf("\nRezultat: %d\n", rez);
    boja(15);

    dodaj_operaciju(glava, a, b, op, rez);
}

void statistika(struct Operacija* glava)
{
    struct Operacija* trenutni = glava;

    int sabiranja = 0;
    int oduzimanja = 0;

    while (trenutni != NULL)
    {
        if (trenutni->OP == '+')
            sabiranja++;

        if (trenutni->OP == '-')
            oduzimanja++;

        trenutni = trenutni->sledeci;
    }

    boja(13);
    printf("\n--- STATISTIKA OPERACIJA ---\n");
    boja(15);

    printf("Broj sabiranja: %d\n", sabiranja);
    printf("Broj oduzimanja: %d\n", oduzimanja);
}

int ucitaj_broj_1_10(const char* poruka)
{
    char linija[100];
    int broj;
    char visak;

    while (1)
    {
        printf("%s", poruka);

        if (!fgets(linija, sizeof(linija), stdin))
            continue;

        if (sscanf(linija, "%d %c", &broj, &visak) != 1)
        {
            printf("Neispravan unos! Unesite ceo broj.\n");
            continue;
        }

        if (broj < 1 || broj > 10)
        {
            printf("Dozvoljeni brojevi su od 1 do 10.\n");
            continue;
        }

        return broj;
    }
}

int ucitaj_opciju()
{
    char linija[100];
    int broj;
    char visak;

    while (1)
    {
        if (!fgets(linija, sizeof(linija), stdin))
            continue;

        if (sscanf(linija, "%d %c", &broj, &visak) != 1)
        {
            printf("Pogresan unos! Pokusajte ponovo: ");
            continue;
        }

        return broj;
    }
}

// Glavni meni
void meni(struct Operacija** glava)
{
    int izbor;
    do
    {
        boja(13);
        printf("\n--- MENI ---\n");
        boja(15);
        printf("1. Sabiranje\n");
        printf("2. Oduzimanje\n");
        printf("3. Prikaz istorije\n");
        printf("4. Sacuvaj u datoteku\n");
        printf("5. Ucitaj iz datoteke\n");
        printf("6. Statistika operacija\n");
        printf("7. Obrisi istoriju\n");
        printf("8. Obrisi datoteku\n");
        printf("9. Prirucnik\n");
        printf("10. Izlaz\n");
        printf("Izaberite opciju: ");
        izbor = ucitaj_opciju();

        int a, b;
        switch (izbor)
        {
        case 1:
            a = ucitaj_broj_1_10("Unesite prvi broj (1-10): ");
            b = ucitaj_broj_1_10("Unesite drugi broj (1-10): ");
            simuliraj_traku(a, b, '+', glava);
            break;
        case 2:
            a = ucitaj_broj_1_10("Unesite prvi broj (1-10): ");
            b = ucitaj_broj_1_10("Unesite drugi broj (1-10): ");
            simuliraj_traku(a, b, '-', glava);
            break;
        case 3:
            prikazi_listu(*glava);
            break;
        case 4:
            sacuvaj_u_datoteku(*glava);
            break;
        case 5:
            ucitaj_iz_datoteke(glava);
            prikazi_listu(*glava);
            break;
        case 6:
            statistika(*glava);
            break;
        case 7:
            obrisi_istoriju(glava);
            printf("\nIstorija operacija je obrisana.\n");
            break;
        case 8:
            obrisi_datoteku();
            break;
        case 9:
            prirucnik();
            break;
        case 10:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nepoznata opcija!\n");
        }

    } while (izbor != 10);
}

// Oslobadjanje memorije liste
void oslobodi_listu(struct Operacija* glava)
{
    struct Operacija* trenutni;
    while (glava)
    {
        trenutni = glava;
        glava = glava->sledeci;
        free(trenutni);
    }
}

void kucaj_tekst(const char* tekst, int brzina)
{
    for (int i = 0; tekst[i] != '\0'; i++)
    {
        printf("%c", tekst[i]);
        Sleep(brzina);
    }
}

int main()
{
    struct Operacija* glava = NULL;
    boja(10);
    kucaj_tekst("Dobrodosli u Tjuring kalkulator!\n", 40);
    boja(15);

    boja(10);
    kucaj_tekst("\nDa li zelite da pocnete? (y/n): ", 40);
    boja(15);

    char c = getchar();

    int d;
    while ((d = getchar()) != '\n' && d != EOF);

    if (c != 'y' && c != 'Y')
    {
        printf("Izlaz iz programa.\n");
        return 0;
    }

    meni(&glava);

    oslobodi_listu(glava);
    return 0;
}

