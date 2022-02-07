
#include<stdio.h>

#include<stdlib.h>

#include<limits.h>

#include<string.h>

#include<math.h>


//După ce ați absolvit cursul de Programarea Calculatoarelor.ați primit de rezolvat următoarea problemă de monitorizare a
//tranzacțiilor financiare.Aceasta poate fi modelată folosind următoarele elemente :
//•	Sursă de bani
//o	Identificator sursă – număr întreg fără semn, maxim un byte
//o	Timestamp plată inițială – șir de 10 caractere in formatul “zz / ll / aaaa”
//o	Lungime lanț tranzacții – număr întreg fără semn
//o	Lanț de tranzacții – pointer către prima tranzacție efectuată din această sursă
//•	Tranzacție
//o	Nume persoană – șir de caractere	◦ Suma - număr double care va conține doar valori pozitive
//o	Următoarea tranzacție din lanțul curent, dacă este ultima tranzacție trebuie să fie setat cu NULL

//1.1.Să se definească tipurile de date TSursa și TTranzactie.TSursa va conține câmpurile : id, lung și lantTranzactii, 
//iar TTranzactie va conține câmpurile : nume, suma și urmTranzactie(3 puncte)

typedef struct TTranzactie {
	char* nume;
	double suma;
	struct TTranzactie* next;

	int nr_tranz;
} TTranzactie;


typedef struct TSursa {
	unsigned char id;
	char time[10];
	unsigned int lung;
	struct TTranzactie* lantTranzactii;
} TSursa;

//1.2.Să se scrie o funcție care alocă memorie pentru un element de tipul TSursa cu antetul :
TSursa* alocaSursa(unsigned int lungTranz) {

	TSursa* sursa = malloc(sizeof(TSursa));
	sursa->lung = lungTranz;
	sursa->lantTranzactii = (TTranzactie*)malloc((sursa->lung) * sizeof(TTranzactie));
	for (int i = 0; i < sursa->lung - 2; i++) {
		sursa->lantTranzactii[i].nr_tranz = 0;
		sursa->lantTranzactii[i].next = sursa->lantTranzactii[i + 1];
		sursa->lantTranzactii[i].nume = malloc(50);
	}
	sursa->lantTranzactii[sursa->lung - 1] = NULL;

	return sursa;
}
//1.3.Scrieți o funcție care alocă memorie pentru nrSurse, cu următoarea semnatură :
//unde nrSurse reprezintă numărul de surse de venit analizate.

TSursa** alocaSurse(unsigned int nrSurse, unsigned int* lungTranz) {

	TSursa** surse = (TSursa*)malloc(nrSurse * sizeof(TSursa*));
	for (int i = 0; i < nrSurse; i++) {
		surse[i] = alocaSursa(lungTranz[i]);
	}

	return surse;
}

void dezalocaSursa(TSursa* sursa) {

	for (int i = 0; i < sursa->lung; i++) {
		free(sursa->lantTranzactii[i].nume);
	}
	free(sursa->lantTranzactii);

	return;
}

//1.7.Definiți o funcție care eliberează toată memoria alocată :
void dezalocaSurse(TSursa** surse, unsigned int nrSurse) {
	for (int i = 0; i < nrSurse; i++) {
		dezalocaSursa(surse[i]);
	}
	free(surse);

	return;
}

//1.4.Definți o funcție care citește datele de intrare dintr - un fișier text deschis deja pentru citire.În fișier, 
//întâi este salvat un unsigned int care reprezintă numărul de surse de bani.După aceea, în fisier se găsesc toate informațiile necesare 
//completării unei surse de bani(alegeți voi ordinea în care acestea se gasesc în fișier). (6 puncte)

TSursa** citesc(FILE* in) {
	unsigned int nrSurse
	fscanf(in, "%d", &nrSurse);
	unsigned int* lungTranz = malloc(nrSurse * sizeof(int));
	for (int i = 0; i < nr_surse; i++) {
		fscanf(in, "%d", &(lungTranz[i]));
	}

	TSursa** surse = alocaSurse(nrSurse, lungTranz);
	for (int i = 0; i < nrSurse; i++) {
		fscanf(in, "%d", &(surse[i]->id));
		fscanf(in, "%s", surse[i]->time);
		fscanf(in, "%lu", surse[i]->lung);
		//acum citesc tranzactiile
		for (int j = 0; j < surse[i]->lung; j++) {
			fscanf(in, "%s", surse[i]->lantTranzactii->nume);
			surse[i]->lantTranzactii->nume[strlen(surse[i]->lantTranzactii->nume)] = '\0';
			fscanf(in, "%g", surse[i]->lantTranzactii->suma);
		}
	}

	return nrSurse;

}

//1.5.Implementați o funcție care detectează un ciclu constant(începe și se termină cu aceeași persoană, iar toate tranzacțiile implicate 
//au aceeași valoare numerică) în lanțul de tranzacții al unei surse.În caz că există un astfel de ciclu, eliminati - l din lanț :
//dc am timp trebuie refacut aici!!!!!!!!!!!!!
void detecteazaSiEliminaCiclu(TSursa* sursa) {

	int ies = 0;
	int lant = 0;
	for (int i = 0; i < sursa->lung - 1; i++) {
		
		for (int j = i + 1; i < sursa->lung - 1; i++) {
			//am cel putin un ciclu de doua tranzactii
			if (strcmp(sursa->lantTranzactii->nume, sursa->lantTranzactii->next->nume) == 0
				&& (sursa->lantTranzactii->suma == sursa->lantTranzactii->next->suma)) {
				
				//next sirug exista 
				free(sursa->lantTranzactii->next->nume); //>??
				sursa->lantTranzactii->next = sursa->lantTranzactii->next->next;
				lant = 1;
			}
			else {
				//trec la urmatorul i
				if (lant == 1) {
					//dc am avut un lant atunci ies de tot nu ami verific si alte lanturi (eu inteleg ca se vrea a fi eliminat numai un cilu)
					ies = 1;
				}
				break; //asta se intampal dc nu am avut lant trec sa vad cum e la urmatorul elem din lant
			}
		}
		if (ies == 1)
			break;
	}
	return;
}

//1.6.Realizați o funcție care să calculeze persoana cu cele mai multe tranzacții în toate lanțurile dintr - un an dat.
char* persoanaTranzactiiMax(TSursa** surse, unsigned int an) {

	char conv[4];
	for (int i = 0; i < surse->nrSurse; i++) {
		conv[0] = (surse[i]->time)[6];
		conv[1] = (surse[i]->time)[7];
		conv[2] = (surse[i]->time)[8];
		conv[3] = (surse[i]->time)[9];
		conv[4] = '\0';
		unsigned int an_Struct = atoi(conv);
		if (an_Struct == an) {
			//atuci are rost sa ma uit in tranzactii
			for (int j = 0; j < surse[i]->lung; j++) {
				surse[i]->lantTranzactii[j].nr_tranz++;
			}
		}
	}
	int max = -1;
	char* nume = malloc(50);
	for (int i = 0; i < surse->nrSurse; i++) {
		for (int j = 0; j < surse[i]->lung; j++) {
			if (max < surse[i]->lantTranzactii[i].nr_tranz) {
				max = surse[i]->lantTranzactii[i].nr_tranz;
				sprintf(nume, "%s", surse[i]->lantTranzactii[i].nume);
			}
		}
	}
	return nume;
}




int main() {



	return 0;
}
