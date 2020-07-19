#include <iostream>
#define WIERZCHOLEK 0
#define MAX_WIERZCHOLKOW 30
#define UJEMNA (-1)
#define BRAK (-1)

typedef struct drzewo {
    struct drzewo* rodzic;
    struct drzewo* syn;
    struct drzewo* brat;
    int wartosc;
}drzewo_t;

enum wspolrzedne {
    wiersz,
    kolumna
};

enum dzialanie_w_tabllicy {
    brak = -2,
    przydziel_pozniej = -1,
    pomin = -3
};

int* Liscie(drzewo_t*& wskaznik, int ilosc_lisci) {
    int* liscie = (int*)malloc(ilosc_lisci * sizeof(int));
    int i = 0, numer_wierzcholka = WIERZCHOLEK;
    //static int numer_wierzcholka;
    while (true) {
        if (wskaznik->wartosc <= WIERZCHOLEK) {
            if (wskaznik->wartosc <= numer_wierzcholka) {
                --numer_wierzcholka;
            }
            else if (wskaznik->brat != NULL) {
                wskaznik = wskaznik->brat;
                continue;
            }
            else if (wskaznik->rodzic != NULL) {
                wskaznik = wskaznik->rodzic;
                continue;
            }
            else {
                break;
            }
        }
        else {
            liscie[i++] = wskaznik->wartosc;
        }

        if (wskaznik->syn != NULL) {
            wskaznik = wskaznik->syn;
        }
        else if (wskaznik->brat != NULL) {
            wskaznik = wskaznik->brat;
        }
        else {
            wskaznik = wskaznik->rodzic;
        }
    }
    return liscie;
}
void SkanowanieTablicy(int& rozmiar, int& suma_usunietych, int**& tablica_wegierska, short*& usuniete_kolumny, short*& usuniete_wiersze) {
    int ilosc_zer;
    int kolumna;
    int wiersz;
    for (int k = 0; k < rozmiar; ++k) {
        usuniete_kolumny[k] = usuniete_wiersze[k] = brak;
    }
    for (int w = 0; w < rozmiar; ++w) {         //SPRAWDZ KOLUMNY
        ilosc_zer = 0;
        kolumna = BRAK;
        for (int k = 0; k < rozmiar; ++k) {
            if (usuniete_kolumny[k] > brak) {
                continue;
            }
            if (tablica_wegierska[w][k] == 0) {
                kolumna = k;
                ilosc_zer++;
            }
            if (ilosc_zer > 1) {
                break;
            }
        }
        if (ilosc_zer == 1) {
            usuniete_kolumny[kolumna] = w;
            suma_usunietych++;
        }
    }

    if (suma_usunietych < rozmiar) {
        for (int k = 0; k < rozmiar; ++k) {         //SPRAWDZ WIERSZE
            if (usuniete_kolumny[k] > brak) {
                continue;
            }
            ilosc_zer = 0;
            wiersz = BRAK;
            for (int w = 0; w < rozmiar; ++w) {
                if (usuniete_wiersze[w] > brak) {
                    continue;
                }
                if (tablica_wegierska[w][k] == 0) {
                    wiersz = w;
                    ilosc_zer++;
                }
                if (ilosc_zer > 1) {
                    break;
                }
            }
            if (ilosc_zer == 1) {
                usuniete_wiersze[wiersz] = k;
                suma_usunietych++;
            }
        }
    }
}

void UtozsamianieZWezlem(drzewo_t*& wskaznik, drzewo_t*& wskaznik2, int**& tablica, int& ilosc_lisci, int& wynik, bool pierwszy_wierzcholek) {
    int wartosc2, wartosc1 = wskaznik->wartosc * UJEMNA + ilosc_lisci + 1;
    while (true) {
        if (wskaznik2->wartosc <= WIERZCHOLEK) {
            wartosc2 = wskaznik2->wartosc * UJEMNA + ilosc_lisci + 1;
            if (pierwszy_wierzcholek) {
                if (tablica[wartosc1][wartosc2] > wynik) {
                    wynik = tablica[wartosc1][wartosc2];
                }
            }
            else {
                if (tablica[wartosc2][wartosc1] > wynik) {
                    wynik = tablica[wartosc2][wartosc1];
                }
            }
        }

        if (wskaznik2->brat != NULL) {
            wskaznik2 = wskaznik2->brat;
        }
        else {
            break;
        }
    }
}

int main() {
    int ilosc_drzew;
    int ilosc_lisci;
    bool pierwszy_syn;
    char wejscie;
    int liczba;
    int numer = 1;
    scanf_s("%d", &ilosc_drzew);
    drzewo_t** drzewo = (drzewo_t**)malloc(ilosc_drzew * sizeof(drzewo_t*));
    drzewo_t*** tablica_wierzcholkow = (drzewo_t***)malloc(ilosc_drzew * sizeof(drzewo_t**));
    drzewo_t* wskaznik = (drzewo_t*)malloc(sizeof(drzewo_t));
    drzewo_t* wskaznik2 = (drzewo_t*)malloc(sizeof(drzewo_t));
    int* numer_wierzcholka = (int*)malloc(ilosc_drzew * sizeof(int));
    int** ilosc_synow_wierzcholka = (int**)malloc(ilosc_drzew * sizeof(int*));
    int** kolejnosc_lisci = (int**)malloc(ilosc_drzew * sizeof(int*));

    for (int i = 0; i < ilosc_drzew; ++i) {
        getchar();
        getchar();
        ilosc_lisci = 1;
        numer_wierzcholka[i] = WIERZCHOLEK;
        kolejnosc_lisci[i] = (int*)malloc(MAX_WIERZCHOLKOW * sizeof(int));
        ilosc_synow_wierzcholka[i] = (int*)malloc(MAX_WIERZCHOLKOW * sizeof(int));
        for (int j = 0; j < MAX_WIERZCHOLKOW; ++j) {
            ilosc_synow_wierzcholka[i][j] = 0;
        }

        tablica_wierzcholkow[i] = (drzewo_t**)malloc(MAX_WIERZCHOLKOW * sizeof(drzewo_t*));
        drzewo[i] = (drzewo_t*)malloc(sizeof(drzewo_t));
        wskaznik = drzewo[i];
        wskaznik->rodzic = NULL;
        wskaznik->syn = NULL;
        wskaznik->brat = NULL;
        tablica_wierzcholkow[i][numer_wierzcholka[i] * UJEMNA] = wskaznik;
        wskaznik->wartosc = numer_wierzcholka[i]--;
        pierwszy_syn = true;

        while ((wejscie = getchar()) != ';') {
            if (isdigit(wejscie)) {
                liczba = 0;
                char kolejna = getchar();
                if (isdigit(kolejna)) {
                    liczba = (wejscie - '0') * 10 + (kolejna - '0');
                }
                else {
                    ungetc(kolejna, stdin);
                    liczba = (wejscie - '0');
                }
                drzewo_t* zmienna = (drzewo_t*)malloc(sizeof(drzewo_t));
                zmienna->syn = NULL;
                zmienna->brat = NULL;
                zmienna->wartosc = liczba;
                if (pierwszy_syn) {
                    pierwszy_syn = 0;
                    zmienna->rodzic = wskaznik;
                    wskaznik->syn = zmienna;
                    wskaznik = wskaznik->syn;
                }
                else {
                    zmienna->rodzic = wskaznik->rodzic;
                    wskaznik->brat = zmienna;
                    wskaznik = wskaznik->brat;
                }
                kolejnosc_lisci[i][zmienna->wartosc] = ilosc_lisci;
                ilosc_synow_wierzcholka[i][wskaznik->rodzic->wartosc * UJEMNA]++;
                //printf("Liczba %d\n", liczba);
            }
            else if (wejscie == ',') {
                ++ilosc_lisci;
                continue;
            }
            else if (wejscie == '(') {
                drzewo_t* zmienna = (drzewo_t*)malloc(sizeof(drzewo_t));
                zmienna->syn = NULL;
                zmienna->brat = NULL;
                zmienna->wartosc = numer_wierzcholka[i];
                if (pierwszy_syn) {
                    zmienna->rodzic = wskaznik;
                    wskaznik->syn = zmienna;
                    wskaznik = wskaznik->syn;
                }
                else {
                    zmienna->rodzic = wskaznik->rodzic;
                    wskaznik->brat = zmienna;
                    wskaznik = wskaznik->brat;
                }
                ilosc_synow_wierzcholka[i][wskaznik->rodzic->wartosc * UJEMNA]++;
                tablica_wierzcholkow[i][(numer_wierzcholka[i]--) * UJEMNA] = wskaznik;
                pierwszy_syn = true;
                //puts("Stworz galaz\n");
            }
            else if (wejscie == ')') {
                wskaznik = wskaznik->rodzic;
                //puts("Przejdz do rodzica\n");
            }
        }

        numer_wierzcholka[i] = numer_wierzcholka[i] * UJEMNA;
        //for (int j = 0; j < numer_wierzcholka[i]; ++j) {
        //    printf("%d ", ilosc_synow_wierzcholka[i][j]);
        //}
        tablica_wierzcholkow[i] = (drzewo_t**)realloc(tablica_wierzcholkow[i], numer_wierzcholka[i] * sizeof(drzewo_t*));
        ilosc_synow_wierzcholka[i] = (int*)realloc(ilosc_synow_wierzcholka[i], numer_wierzcholka[i] * sizeof(int));
        kolejnosc_lisci[i] = (int*)realloc(kolejnosc_lisci[i], (ilosc_lisci + 1) * sizeof(int));
    }

    for (int x = 0; x < ilosc_drzew - 1; ++x) {
        for (int y = x + 1; y < ilosc_drzew; ++y) {
            int** tablica = (int**)malloc((ilosc_lisci + numer_wierzcholka[y] + 1) * sizeof(int*));
            //int* tymczasowa = (int*)malloc(ilosc_lisci * sizeof(int));
            int max_wartosc = 0;

            tablica[0] = (int*)malloc((ilosc_lisci + numer_wierzcholka[x] + 1) * sizeof(int));
            tablica[0][0] = NULL;

            //wskaznik = drzewo[x];
            //tymczasowa = Liscie(wskaznik, ilosc_lisci);      
            for (int i = 1; i <= ilosc_lisci; ++i) {
                //tablica[0][i] = tymczasowa[i - 1];
                tablica[i] = (int*)malloc((ilosc_lisci + numer_wierzcholka[x] + 1) * sizeof(int));
                tablica[0][kolejnosc_lisci[x][i]] = i;
            }

            //wskaznik = drzewo[y];
            //tymczasowa = Liscie(wskaznik, ilosc_lisci);
            for (int i = 1; i <= ilosc_lisci; ++i) {
                tablica[kolejnosc_lisci[y][i]][0] = i;
            }
            for (int i = 1; i <= ilosc_lisci; ++i) {
                //tablica[i] = (int*)malloc((ilosc_lisci + numer_wierzcholka[x] + 1) * sizeof(int));
                //tablica[i][0] = tymczasowa[i - 1];
                //tablica[kolejnosc_lisci[y][i]][0] = i;
                for (int j = 1; j <= ilosc_lisci; ++j) {
                    if (tablica[i][0] == tablica[0][j]) {
                        tablica[i][j] = 1;
                    }
                    else {
                        tablica[i][j] = 0;
                    }
                }
            }

            for (int i = ilosc_lisci + 1, j = 0; i <= ilosc_lisci + numer_wierzcholka[x]; ++i, ++j) {
                tablica[0][i] = tablica_wierzcholkow[x][j]->wartosc;
            }
            for (int i = ilosc_lisci + 1, j = 0; i <= ilosc_lisci + numer_wierzcholka[y]; ++i, ++j) {
                tablica[i] = (int*)malloc((ilosc_lisci + numer_wierzcholka[x] + 1) * sizeof(int));
                tablica[i][0] = tablica_wierzcholkow[y][j]->wartosc;
            }


            for (int i = ilosc_lisci + numer_wierzcholka[x]; i > ilosc_lisci; --i) {
                for (int j = 1; j <= ilosc_lisci; ++j) {
                    tablica[j][i] = 0;
                    wskaznik = tablica_wierzcholkow[x][i - ilosc_lisci - 1]->syn;
                    while (true) {
                        if (wskaznik->wartosc == tablica[j][0]) {
                            tablica[j][i] = 1;
                            break;
                        }
                        if (wskaznik->brat != NULL) {
                            wskaznik = wskaznik->brat;
                        }
                        else {
                            break;
                        }
                    }
                }

                wskaznik = tablica_wierzcholkow[x][i - ilosc_lisci - 1]->syn;
                while (true) {
                    if (wskaznik->wartosc <= WIERZCHOLEK) {
                        for (int k = 1; k <= ilosc_lisci; ++k) {
                            if (tablica[k][ilosc_lisci + (wskaznik->wartosc * UJEMNA) + 1] == 1) {
                                tablica[k][i] = 1;
                            }
                        }
                    }
                    if (wskaznik->brat != NULL) {
                        wskaznik = wskaznik->brat;
                    }
                    else {
                        break;
                    }
                }
            }

            for (int i = ilosc_lisci + numer_wierzcholka[y]; i > ilosc_lisci; --i) {
                for (int j = 1; j <= ilosc_lisci; ++j) {
                    tablica[i][j] = 0;

                    wskaznik = tablica_wierzcholkow[y][i - ilosc_lisci - 1]->syn;
                    while (true) {
                        if (wskaznik->wartosc == tablica[0][j]) {
                            tablica[i][j] = 1;
                            break;
                        }
                        if (wskaznik->brat != NULL) {
                            wskaznik = wskaznik->brat;
                        }
                        else {
                            break;
                        }
                    }
                }
                wskaznik = tablica_wierzcholkow[y][i - ilosc_lisci - 1]->syn;
                while (true) {
                    if (wskaznik->wartosc <= WIERZCHOLEK) {
                        for (int k = 1; k <= ilosc_lisci; ++k) {
                            if (tablica[ilosc_lisci + (wskaznik->wartosc * UJEMNA) + 1][k] == 1) {
                                tablica[i][k] = 1;
                            }
                        }
                    }
                    if (wskaznik->brat != NULL) {
                        wskaznik = wskaznik->brat;
                    }
                    else {
                        break;
                    }
                }
            }

            for (int i = numer_wierzcholka[y] - 1; i >= 0; --i) {
                for (int j = numer_wierzcholka[x] - 1; j >= 0; --j) {
                    int wynik = 0;                                                        //powi¹zaæ synów wêz³a AX z synami wêz³a BX metoda wêgierska
                    int rozmiar;
                    if (ilosc_synow_wierzcholka[y][i] >= ilosc_synow_wierzcholka[x][j]) {
                        rozmiar = ilosc_synow_wierzcholka[y][i];
                    }
                    else {
                        rozmiar = ilosc_synow_wierzcholka[x][j];
                    }
                    int** powiazania_synow = (int**)malloc(rozmiar * sizeof(int*));

                    int w = 0, wartosc1, wartosc2;
                    wskaznik = tablica_wierzcholkow[y][i]->syn;
                    while (true) {
                        powiazania_synow[w] = (int*)malloc(rozmiar * sizeof(int));
                        if (wskaznik->wartosc > WIERZCHOLEK) {
                            wartosc1 = kolejnosc_lisci[y][wskaznik->wartosc];
                        }
                        else {
                            wartosc1 = wskaznik->wartosc * UJEMNA + ilosc_lisci + 1;
                        }

                        int k = 0;
                        wskaznik2 = tablica_wierzcholkow[x][j]->syn;
                        while (true) {
                            if (wskaznik2->wartosc > WIERZCHOLEK) {
                                wartosc2 = kolejnosc_lisci[x][wskaznik2->wartosc];
                            }
                            else {
                                wartosc2 = wskaznik2->wartosc * UJEMNA + ilosc_lisci + 1;
                            }
                            powiazania_synow[w][k] = tablica[wartosc1][wartosc2];
                            if (max_wartosc < powiazania_synow[w][k]) {
                                max_wartosc = powiazania_synow[w][k];
                            }
                            ++k;
                            if (wskaznik2->brat != NULL) {
                                wskaznik2 = wskaznik2->brat;
                            }
                            else {
                                break;
                            }
                        }
                        while (k < rozmiar) {
                            powiazania_synow[w][k++] = 0;
                        }
                        ++w;
                        if (wskaznik->brat != NULL) {
                            wskaznik = wskaznik->brat;
                        }
                        else {
                            break;
                        }
                    }
                    while (w < rozmiar) {
                        powiazania_synow[w] = (int*)malloc(rozmiar * sizeof(int));
                        for (int k = 0; k < rozmiar; ++k) {
                            powiazania_synow[w][k] = 0;
                        }
                        ++w;
                    }

                    puts("\n");
                    for (int w = 0; w < rozmiar; ++w) {
                        for (int k = 0; k < rozmiar; ++k) {
                            printf("%d", powiazania_synow[w][k]);
                        }
                        printf("\n");
                    }

                    if (rozmiar < 3) {
                        wynik = powiazania_synow[0][0] + powiazania_synow[1][1];
                        if (wynik < powiazania_synow[0][1] + powiazania_synow[1][0]) {
                            wynik = powiazania_synow[0][1] + powiazania_synow[1][0];
                        }
                    }

                    else {
                        int** tablica_wegierska = (int**)malloc(rozmiar * sizeof(int*));
                        int* min_w_kolumnie = (int*)malloc(rozmiar * sizeof(int));
                        int* min_w_wierszu = (int*)malloc(rozmiar * sizeof(int));
                        for (int w = 0; w < rozmiar; ++w) {
                            tablica_wegierska[w] = (int*)malloc(rozmiar * sizeof(int));
                            min_w_kolumnie[w] = min_w_wierszu[w] = max_wartosc;
                            for (int k = 0; k < rozmiar; ++k) {
                                tablica_wegierska[w][k] = max_wartosc - powiazania_synow[w][k];
                                if (tablica_wegierska[w][k] < min_w_wierszu[w]) {
                                    min_w_wierszu[w] = tablica_wegierska[w][k];
                                }
                            }
                            if (min_w_wierszu[w] == 0) {
                                continue;
                            }
                            for (int k = 0; k < rozmiar; ++k) {
                                tablica_wegierska[w][k] -= min_w_wierszu[w];
                            }
                        }
                        for (int k = 0; k < rozmiar; ++k) {
                            for (int w = 0; w < rozmiar; ++w) {
                                if (tablica_wegierska[w][k] < min_w_kolumnie[k]) {
                                    min_w_kolumnie[k] = tablica_wegierska[w][k];
                                }
                            }
                            if (min_w_kolumnie[k] == 0) {
                                continue;
                            }
                            for (int w = 0; w < rozmiar; ++w) {
                                tablica_wegierska[w][k] -= min_w_kolumnie[k];
                            }
                        }

                        puts("\n");
                        for (int w = 0; w < rozmiar; ++w) {
                            for (int k = 0; k < rozmiar; ++k) {
                                printf("%d", tablica_wegierska[w][k]);
                            }
                            printf("\n");
                        }

                        short* usuniete_kolumny = (short*)malloc(rozmiar * sizeof(short));
                        short* usuniete_wiersze = (short*)malloc(rozmiar * sizeof(short));
                        int suma_usunietych = 0;
                        SkanowanieTablicy(rozmiar, suma_usunietych, tablica_wegierska, usuniete_kolumny, usuniete_wiersze);

                        while (suma_usunietych < rozmiar) {
                            bool wspolrzedna_usuniecia;
                            bool flaga = false;
                            int najmniejsze_z_nieusunietych_wartosc = max_wartosc + 1;
                            int najmniejszy_k = BRAK;
                            int najmniejszy_w = BRAK;
                            int ilosc_najmnijszych = rozmiar + 1;
                            int min_wartosc;
                            int min_k = BRAK;
                            int min_w = BRAK;
                            int ilosc_min;
                            int ile_usunieto;
                            for (int w = 0; w < rozmiar; ++w) {
                                if (usuniete_wiersze[w] > brak) {
                                    continue;
                                }
                                ilosc_min = 0;
                                ile_usunieto = 0;
                                min_wartosc = max_wartosc + 1;
                                for (int k = 0; k < rozmiar; ++k) {
                                    if (usuniete_kolumny[k] > brak) {
                                        if (tablica_wegierska[w][k] == 0) {
                                            ile_usunieto++;
                                        }
                                        continue;
                                    }
                                    if (tablica_wegierska[w][k] == 0 && (usuniete_kolumny[k] == pomin || usuniete_wiersze[w] == pomin)) {
                                        if (min_wartosc != 0) {
                                            ilosc_min = 1;
                                        }
                                        else {
                                            ++ilosc_min;
                                        }
                                    }
                                    else if (tablica_wegierska[w][k] < min_wartosc) {
                                        min_wartosc = tablica_wegierska[w][k];
                                        min_k = k;
                                        min_w = w;
                                        ilosc_min = 1;
                                    }
                                    else if (tablica_wegierska[w][k] == min_wartosc) {
                                        min_k = k;
                                        min_w = w;
                                        ++ilosc_min;
                                    }
                                }
                                if (ilosc_min + ile_usunieto == rozmiar && min_wartosc == 0) {
                                    usuniete_wiersze[w] = przydziel_pozniej;
                                    suma_usunietych++;
                                    flaga = true;
                                    break;
                                }
                                if ((min_wartosc < najmniejsze_z_nieusunietych_wartosc) ||
                                    (min_wartosc == najmniejsze_z_nieusunietych_wartosc && ilosc_min < ilosc_najmnijszych)) {
                                    najmniejsze_z_nieusunietych_wartosc = min_wartosc;
                                    najmniejszy_k = min_k;
                                    najmniejszy_w = min_w;
                                    ilosc_najmnijszych = ilosc_min;
                                    wspolrzedna_usuniecia = kolumna;
                                }
                            }
                            if (flaga) {
                                continue;
                            }

                            for (int k = 0; k < rozmiar; ++k) {
                                if (usuniete_kolumny[k] > brak) {
                                    continue;
                                }
                                ile_usunieto = 0;
                                ilosc_min = 0;
                                for (int w = 0; w < rozmiar; ++w) {
                                    if (usuniete_wiersze[w] > brak) {
                                        if (tablica_wegierska[w][k] == 0) {
                                            ile_usunieto++;
                                        }
                                        continue;
                                    }
                                    if (tablica_wegierska[w][k] == 0 && (usuniete_kolumny[k] == pomin || usuniete_wiersze[w] == pomin)) {
                                        if (min_wartosc != 0) {
                                            ilosc_min = 1;
                                        }
                                        else {
                                            ++ilosc_min;
                                        }
                                    }
                                    else if (tablica_wegierska[w][k] == najmniejsze_z_nieusunietych_wartosc) {
                                        min_k = k;
                                        min_w = w;
                                        ++ilosc_min;
                                    }
                                }
                                if (ilosc_min + ile_usunieto == rozmiar && min_wartosc == 0) {
                                    usuniete_kolumny[k] = przydziel_pozniej;
                                    suma_usunietych++;
                                    flaga = true;
                                    break;
                                }
                                if (ilosc_min > 0 && ilosc_min < ilosc_najmnijszych) {
                                    najmniejszy_k = min_k;
                                    najmniejszy_w = min_w;
                                    ilosc_najmnijszych = ilosc_min;
                                    wspolrzedna_usuniecia = wiersz;
                                }
                            }
                            if (flaga) {
                                continue;
                            }

                            if (najmniejsze_z_nieusunietych_wartosc == 0) {
                                if (wspolrzedna_usuniecia == wiersz) {
                                    usuniete_wiersze[najmniejszy_w] = najmniejszy_k;
                                    usuniete_kolumny[najmniejszy_k] = pomin;
                                }
                                else {
                                    usuniete_kolumny[najmniejszy_k] = najmniejszy_w;
                                    usuniete_wiersze[najmniejszy_w] = pomin;
                                }
                                suma_usunietych++;
                            }
                            else {
                                for (int w = 0; w < rozmiar; ++w) {
                                    for (int k = 0; k < rozmiar; ++k) {
                                        if (usuniete_wiersze[w] > brak && usuniete_kolumny[k] > brak) {
                                            tablica_wegierska[w][k] += najmniejsze_z_nieusunietych_wartosc;
                                        }
                                        else if (usuniete_wiersze[w] <= brak && usuniete_kolumny[k] <= brak) {
                                            tablica_wegierska[w][k] -= najmniejsze_z_nieusunietych_wartosc;
                                        }
                                    }
                                }
                                puts("\n");
                                for (int w = 0; w < rozmiar; ++w) {
                                    for (int k = 0; k < rozmiar; ++k) {
                                        printf("%d", tablica_wegierska[w][k]);
                                    }
                                    printf("\n");
                                }
                                suma_usunietych = 0;
                                SkanowanieTablicy(rozmiar, suma_usunietych, tablica_wegierska, usuniete_kolumny, usuniete_wiersze);
                            }
                        }
                        for (int w = 0; w < rozmiar; ++w) {
                            if (usuniete_wiersze[w] == przydziel_pozniej) {
                                for (int k = 0; k < rozmiar; ++k) {
                                    if (usuniete_kolumny[k] == brak) {
                                        usuniete_wiersze[w] = k;
                                        usuniete_kolumny[k] = pomin;
                                        break;
                                    }
                                }
                            }                            
                        }

                        for (int k = 0; k < rozmiar; ++k) {
                            if (usuniete_kolumny[k] == przydziel_pozniej) {
                                for (int w = 0; w < rozmiar; ++w) {
                                    if (usuniete_wiersze[w] == brak) {
                                        usuniete_kolumny[k] = w;
                                        usuniete_wiersze[w] = pomin;
                                        break;
                                    }
                                }
                            }
                            if (usuniete_wiersze[k] > brak) {
                                wynik += powiazania_synow[k][usuniete_wiersze[k]];
                            }
                            if (usuniete_kolumny[k] > brak) {
                                wynik += powiazania_synow[usuniete_kolumny[k]][k];
                            }
                        }

                        for (int k = 0; k < rozmiar; ++k) {
                            free(tablica_wegierska[k]);
                        }
                        free(tablica_wegierska);
                        free(min_w_kolumnie);
                        free(min_w_wierszu);
                        free(usuniete_kolumny);
                        free(usuniete_wiersze);
                    }
                    printf("%d\n", wynik);
                    //uto¿samianie z wêz³ami
                    wskaznik = tablica_wierzcholkow[y][i];
                    wskaznik2 = tablica_wierzcholkow[x][j]->syn;
                    UtozsamianieZWezlem(wskaznik, wskaznik2, tablica, ilosc_lisci, wynik, true);

                    wskaznik = tablica_wierzcholkow[x][j];
                    wskaznik2 = tablica_wierzcholkow[y][i]->syn;
                    UtozsamianieZWezlem(wskaznik, wskaznik2, tablica, ilosc_lisci, wynik, false);

                    tablica[i + ilosc_lisci + 1][j + ilosc_lisci + 1] = wynik;
                    printf("%d  na [%d][%d]\n", wynik, i + ilosc_lisci + 1, j + ilosc_lisci + 1);

                    for (int k = 0; k < rozmiar; ++k) {
                        free(powiazania_synow[k]);
                    }
                }
            }

            //printf("\n numer_wierzcholka[x]=%d\n", numer_wierzcholka[x]);
            /*
            for (int i = 0; i <= ilosc_lisci + numer_wierzcholka[y]; ++i) {
                for (int j = 0; j <= ilosc_lisci + numer_wierzcholka[x]; ++j) {
                    if (i <= ilosc_lisci || j <= ilosc_lisci) {
                        printf("%d  ", tablica[i][j]);
                    }
                }
                printf("\n");
            }
            */
            int max_wynik = 0;
            for (int i = ilosc_lisci + 1; i <= ilosc_lisci + numer_wierzcholka[y]; ++i) {
                for (int j = ilosc_lisci + 1; j <= ilosc_lisci + numer_wierzcholka[x]; ++j) {
                    //printf("%d  na [%d][%d]\n", tablica[i][j], i, j);
                    if (tablica[i][j] > max_wynik) {
                        max_wynik = tablica[i][j];
                    }
                }
            }
            printf("%d, %d\n", numer++, ilosc_lisci - max_wynik);
            if (numer == 404) {
                puts("\n");
            }
            for (int i = 0; i < ilosc_lisci + numer_wierzcholka[y] + 1; ++i) {
                free(tablica[i]);
            }
            //free(tymczasowa);
        }
    }
    for (int i = 0; i < ilosc_drzew; ++i) {
        for (int j = 0; j < numer_wierzcholka[i]; ++j) {
            free(tablica_wierzcholkow[i][j]);
        }
        free(kolejnosc_lisci[i]);
        free(ilosc_synow_wierzcholka[i]);
        free(tablica_wierzcholkow[i]);
    }
    free(drzewo);
    free(kolejnosc_lisci);
    free(ilosc_synow_wierzcholka);
    free(tablica_wierzcholkow);
    //free(wskaznik2);
    free(numer_wierzcholka);
}