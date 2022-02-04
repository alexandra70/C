#include <stdio.h>
#include <stdlib.h>

/*1.1.	Definiți tipurile de date TAutor și TVolum cu care veți salva în memorie datele.
 TAutor conține câmpurile numeAutor, codTara, nrVolume, volume (de tip TVolum*). 
 Tipul TVolum are câmpurile numeVolum, anPublicare, stare și idPersoana. 
 Dacă adăugați câmpuri adiționale, explicați în comentarii rolul lor. (3 puncte)

 o	Starea volumului salvată pe un octet: bitul cel mai puțin semnificativ înseamnă carte
  donată (0) sau cumpărată (1), al doilea bit dacă cartea este împrumutată (1) sau nu (0), 
 iar următorii biți conțin alte informații despre starea ei fizică (gradul de deteriorare, etc.)*/

typedef struct TVolum { 
    char* numeVolum;
    short int anPublicare;
    char stare; //ma olosesc de bitii din octetul asta pt codficare
    int idPersoana; //ultima pers care a mprumutat acest volum
} TVolum;

typedef struct TAutor {
    char* numeAutor;
    char codTara[2];
    int nrVolume;
    TVolum * volume; //vector de volume?
} TAutor;

/*1.2.	Scrieți o functie care alocă memorie pentru un autor cu nrVolume cărți, cu semnătura
TAutor* alocaAutor(int nrVolume)*/

TAutor* alocaAutor(int nrVolume) {
    TAutor* autor = (TAutor*)malloc(sizeof(TAutor));
    if(autor == NULL) {
        printf("erore la alocare autor");
        exit(-1); //nu stiu bine ce face asta, cred ca doar iese cu ceva erori, nu?
    }

    autor->numeAutor = malloc(50);
    autor->nrVolume = nrVolume;
    autor->volume = (TVolum*)malloc(autor->nrVolume * sizeof(TVolum));
    for(int i = 0; i < autor->nrVolume; i ++) {
        //nu prea vad cum pot sa folosesc aici alta functie 
        //de alocare pt tvolum* deci aloc drect aici, am nevoie de pareri in legatura cu chestia asta
        //cum se paote asta adica eu aici nu pot aloca volume, astea efectiv sunt TVOLUM TOATE
        //adica efectiv declarate, nu prea inteleg dc vrea vector de chestii asa mari
        //deci vectorul meu ocupa nr * 8 octeti pt fiecare strucutra, sau mai mult
        (autor->volume[i]).numeVolum = malloc(50);
        (autor->volume[i]).anPublicare = 0;
        (autor->volume[i]).idPersoana = 0;
        (autor->volume[i]).stare = 0;
    }

    return autor;
}

void dezalocAutor(TAutor* autor) {

    free(autor->numeAutor);
     //am un vector de volume deci doar il dezaloc pe el
    for(int i = 0; i < autor->nrVolume; i ++) {
        free((autor->volume[i]).numeVolum);
    }
    free((autor->volume);
    free(autor);
    autor = NULL; 

    return;
}

/*1.3.	Scrieți o funcție care alocă memorie pentru nrAutori autori cu următoarea semnătură,
 unde nrVolumeAutor este un vector de dimensiune nrAutori :	
 TAutori** alocaAutori(int nrAutori, int* nrVolumeAutor) */


TAutor** alocaAutori(int nrAutori, int* nrVolumeAutor) {
    //deci vector de pointeri la structuri de tipul autor
    TAutor** autori = (TAutor**)malloc(nrAutori * sizeof(TAutor*));
    for(int i = 0 ; i < nrAutori; i ++) {
        autori[i] = alocaAutor(nrVolumeAutor[i]);
        //dc nu e (*autori)[i] poate pt ca e 
        //deci aloc in da un pointer si autori de [i] - ma duce la structura aia
        //deci  (*autori)[i] = de ce e efectiv la pointer
    }

    return autori;
}


/*1.7.	Definiți o funcție care eliberează toată memoria alocată: 
dezalocaScoli(TScoala** autori, int nrAutori) */
 
 void dezalocAutori(TAutor** autori, int nrAutori) {

    for(int i = 0; i < nrAutori; i ++) {
        dezalocAutor(&(autori[i])); //sperca e adresa lui autori de i
    }

    free(autori);

    return;
 }

/*1.4.	Definiți o funcție care citește datele de intrare dintr-un fișier binar deschis deja
 pentru citire. in fișier sunt salvate întâi un int care reprezintă numărul de autori, apoi
  nrAutori numere întregi care reprezintă numărul de volume al fiecărui autor, iar apoi 
  datele pentru fiecare autor (și volumele asociate) în parte. Funcția trebuie să întoarcă
   atât numărul de autori din fișier (variabilă int), cât și toți autorii citiți din fișier
    (variabilă de tip TAutor**)*/
typedef struct TVolum { 
    char* numeVolum;
    short int anPublicare;
    char stare; //ma olosesc de bitii din octetul asta pt codficare
    int idPersoana; //ultima pers care a mprumutat acest volum
} TVolum;

typedef struct TAutor {
    char* numeAutor;
    char codTara[2];
    int nrVolume;
    TVolum * volume; //vector de volume?
} TAutor;

TAutor** citeste_fisier(FILE* fin) {

    int nrAutori;
    int* nrVolumeAutor = malloc(nrAutori * sizeof(int));
    fread(&nrAutori, sizeof(int), 1, fin);
    for(int i = 0 ; i < nrAutori; i ++) {
        fread(&(nrVolumeAutor[i]));
    }

    int len = 0;//pp ca pt stringuri se da lungimea inainte de citire inainte
    TAutor** autori = alocaAutori(nrAutori, nrVolumeAutor);
    for(int i = 0 ; i < nrAutori; i ++) {
        fread(&len, sizeof(int), 1, fin);
        fread(autori[i]->numeAutor, sizeof(char), len, fin);
        fread(autori[i]->codTara, sizeof(char), 2, fin);
        //deja am citit nr de volume pt fiecare scriitor

        for(int j = 0; j < nrVolumeAutor[i]; j ++) {
            fread(&((autori[i]->volume).anPublicare), sizeof(int), 1, fin);
            fread(&((autori[i]->volume).stare), sizeof(char), 1, fin);
            fread(&((autori[i]->volume).idPersoana), sizeof(int), 1, fin);
        }
    }

    return autori;

}

/*1.5.	Implementați o funcție care să calculeze câte volume sunt în bibiotecă pentru autori
 dintr-o anumită țară:*/

void nrVolumeTara(TAutor** autori, int nrAutori, char codTari[]) {
    int count = 0;
    for(int i = 0; i < nrAutori; i ++) {

        if(strncmp( (autori[i]->volume).codTara), codTari, 2) == 0) {
            count++;
        }
    }
    printf("Din %s sunt %d volume\n", codTari, count);
    return;
}

/* 1.6 Definiți o funcție care calculează cine este persoana care a împrumutat cele mai multe
 cărți și nu le-a returnat (flag-ul de împrumutat trebuie să fie 1). 
 Semnătura funcției este: int maxVolume(TAutor** autori, int nrAutori) */

int maxVolume(TAutor** autori, int nrAutori) {
    int max = 0;

    //vad care are bitul setat
   
    char mask = 1 << 1; // octet = 0000 0010
    int* impr = calloc(1000 * sizeof(int));

    //trec prin toti utorii
    for(int i = 0; i < nrAutori; i ++) {
        //trec prin toate volumele lor
        for(int j = 0; j < autori[i]->nrVolume; j++) {
            //pt autorul j vad ce volum a fost imprumutat
            if(((autori[i]->volume[j]).stare) & mask) == 0) {
                //nu e imprumutat
                continue;
            }
            impr[(autor->volume[i]).idPersoana] ++;
        }
    }

    int id = -1;
    for(int i = 0; i < 1000; i ++) {
        if (max > impr[i]) {
            max = impr[i];
            id = i;
        }
    }

    return id;
}





int main() {


    FILE* fin;
    fin = fopen("binar.txt", "r");


    return 0;
}