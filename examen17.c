#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>
#include<math.h>


/*
•	Un șofer va avea informațiile :
o	Nume, reprezentat ca șir de caractere care va conține maximum 20 caractere utile
o	Număr mașină, șir de caractere format din 8 caractere utile(de forma JJNNNLLL – J = Județ, N = Număr, L = Literă,
de ex.“B 103PPP”, "IF12 CCC", "B 1	ACS", "CJ123ABC")
o	Locația curentă pe hartă, salvată ca două numere reale cu precizie dublă
o	Numărul de curse efectuate, întreg fără semn
o	Informații despre toate cursele efectuate, folosind alocare dinamică și structura de mai jos
•	O cursă ca conține următoarele detalii :
o	Locație început cursă, salvată ca două numere reale cu precizie dublă
o	Starea cursei reprezentată pe un byte(folosind bitfields, măști de biți sau orice altă variantă, explicați 
soluția voastră) în care rețineți : cursa anulată sau nu, cursă în desfășurare, nr stele(între 0 - 5)
1.1.Definiți tipurile de date TSofer și TCursa.Tipul TSofer conține câmpurile nume, nrMasina, locatieX, locatieY,
nrCurse și curse(de tip TCursa**).Tipul TCursa are câmpurile startX, startY și stare.Dacă adăugați câmpuri adiționale,
explicați în comentarii rolul lor. (4 puncte)
*/


typedef struct TSofer {
	char nume[20];
	char nrMasina[8]; // JJ  NNN   LLL
	double locatieX;
	double locatieY;
	unsigned int nrCurse;
	TCursa** curse;
	double dist;

} TSofer;


typedef struct TCursa {
	double startX;
	double startY; //locatia de start
	//iau asa 0000 xabc - x-bit anulata sau nu, toate incep ca anulate = 0
	//urm abc suficient sa reprezint 5 (max de stele) toti pe 0 => cursa are 0 stele
	//101 => 4 + 1 = 5; cu trei bti pot rep toate nr care ma intereseaza
	char stare; //cursa anulată sau nu, cursă în desfășurare, nr stele(între 0 - 5)

} TCursa;

//1.2.Scrieți o functie care alocă memorie pentru un șofer cu nrCurse curse efectuate, cu semnătura

TCursa* alocaCursa() {
	TCursa* cursa = (TCursa*)malloc(sizeof(TCursa));
	cursa->startX = 0.0;
	cursa->stareY = 0.0;
	cursa->stare = 0;

	return cursa;
}

TSofer* alocaSofer(int nrCurse) {

	TSofer* sofer = (TSofer*)malloc(sizeof(TSofer));
	sofer->nrCurse = nrCurse;

	//aloc pointer la un vector de pointeri la structuri de tipul TSofer
	sofer->curse = (TCursa**)malloc(sofer->nrCurse * sizeof(TCursa*));
	for (int i = 0; i < sofer->nrCurse; i++) {
		//acum acol pt fiecare cursa 
		sofer->curse[i] = alocaCursa();
	}

	return sofer;
}



//1.3.Scrieți o funcție care alocă memorie pentru nrSoferi șoferi cu următoarea semnătură, unde 
//nrCurseSoferi este un vector de dimensiune nrSoferi : TSofer * *alocaSoferi(int nrSoferi, int* nrCurseSoferi)


TSofer** alocaSoferi(int nrSoferi, int* nrCurseSoferi) {

	TSofer** soferi = (TSofer**)malloc(nrSoferi * (TSofer*));
	for (int i = 0; i < nrSoferi; i++) {
		soferi[i] = alocaSofer(nrCurseSoferi[i]);
	}

	return soferi;
}



//1.4.Definiți o funcție care citește datele de intrare dintr - un fișier binar deschis deja pentru citire.
//În fișier sunt salvate întâi un int care reprezintă numărul de șoferi, apoi nrȘoferi numere întregi care 
//reprezintă numărul de curse efectuate de fiecare șofer, iar apoi datele pentru fiecare șofer(și cursele asociate) 
//în parte.Funcția trebuie să întoarcă atât numărul de șoferi citiți din fișier(variabilă int), cât și datele despre 
//șoferi(variabilă de tip TSofer**).

typedef struct TCursa {
	double startX;
	double startY;
	char stare;
} TCursa;
DateSoferi* citesteDate(FILE* in) {

	DateSoferi* data = malloc(sizeof(DateSoferi));

	fread(&(data->nrSoferi), sizeof(int), 1, in);
	int* nrCurseSoferi = malloc(data->nrSoferi * sizeof(int));
	/*for (int i = 0; i < data->nrSoferi; i++) {
		fread(&(nrCurseSoferi[i]), sizeof(int), 1, in);
	}*/
	fread(nrCurseSoferi, sizeof(int), data->nrSoferi, in);

	data->soferi = alocaSoferi(data->nrSoferi, nrCurseSoferi);
	for (int i = 0; i < data->nrSoferi; i++) {
		fread(data->soferi[i]->nume, sizeof(char), 20, in);
		fread(data->soferi[i]->nrMasina, sizeof(char), 8, in);
		fread(data->soferi[i]->locatieX, sizeof(double), 1, in);
		fread(data->soferi[i]->locatieY, sizeof(double), 1, in);
		data->soferi[i]->nrCurse = nrCurseSoferi[i]; //pp ca nu mai citesc ce deja am citit in vectorul de mai sus

		//acum pt fiecare cursa
		for (int j = 0; j < data->soferi[i]->nrCurse; j++) {
			fread(&(data->soferi[i]->curse[j]->startX), sizeof(double), 1, in);
			fread(&(data->soferi[i]->curse[j]->startY), sizeof(double), 1, in);
			fread(&(data->soferi[i]->curse[j]->stare), sizeof(char), 1, in);
		}
	}
	
	return data;
}

/*1.5.	Implementați o funcție care să calculeze care este șoferul cu cel mai mare rating mediu 
(rating mediu = sumă număr stele / (număr curse total - anulate - în desfășurare) să fie maximă) și întoarceți numele acestuia:*/

char* numeSoferRatingMaxim(TSofer** soferi, int nrSoferi) {
	int max = -1;
	int rating = 0;
	int stars = 0;
	char mask = 7; // 1 << 2 + 1 << 1 + 1 << 0; //sper ca e 7, sau scriu ca e gal 7 .
	char* nume = malloc(20);
	for (int i = 0; i < nrSoferi; i++) {
		//calculez aici ratingul pt fiecare
		rating = 0;
		stars = 0;
		mask = 7;
		for (int j = 0; j < soferi[i]->nrCurse; j++) {
			char val = (soferi[i]->curse[j]->stare);
			stars += (int)(mask & val);
		}
		rating = stars / (soferi[i]->nrCurse);
		if (rating > max) {
			max = rating;
			strcpy(nume, soferi[i]->nume);
		}
	}
	nume[strlen(nume)] = '\0';
	return nume;
}

/*1.6.Definiți o funcție care să întoarcă primii nr șoferi cei mai apropiați(folosind distanța euclidiană) de un punct de pornire(x, y).
Semnătura funcției este : */
TSofer** maxVolume(TSofer** soferi, int nrSoferi, int x, int y, int nr) {
	
	//actualizez distanta pt fiecare sofer(am def un camp in struct de soferi pt asta)
	for (int i = 0; i < nrSoferi; i++) {
		double val = sqrt(soferi[i]->locatieX * soferi[i]->locatieX - (double)(x * x))
			- (soferi[i]->locatieY * soferi[i]->locatieY - (double)(y * y));
		soferi[i]->dist = val;
	}

	//parcurg vectorul de pointeri la struct soferi si il sortez crescator

	for (int i = 0; i < nrSoferi; i++) {
		for (int j = i + 1; j < nrSoferi - 1; j++) {
			if (soferi[i]->dist > soferi[j]->dist) {
				TSofer* tmp = soferi[i];
				soferi[i] = soferi[j];
				soferi[j] = tmp;
			}
		}
	}

	//copiez prinii nr soferi din soferi acum sortatit dupa distanta

	if (nr > nrSoferi) {
		return NULL;
	}

	if (nr == nrSoferi) {
		return soferi;
	}

	TSofer** soferiD = malloc(nr * sizeof(TSofer*));

	for (int i = 0; i < nr; i++) {
		soferiD[i] = soferi[i];
	}

	return soferiD
}




typedef struct TCursa {
	double startX;
	double startY; //locatia de start
	//iau asa 0000 xabc - x-bit anulata sau nu, toate incep ca anulate = 0
	//urm abc suficient sa reprezint 5 (max de stele) toti pe 0 => cursa are 0 stele
	//101 => 4 + 1 = 5; cu trei bti pot rep toate nr care ma intereseaza
	char stare; //cursa anulată sau nu, cursă în desfășurare, nr stele(între 0 - 5)

} TCursa;
typedef struct TSofer {
	char nume[20];
	char nrMasina[8]; // JJ  NNN   LLL
	double locatieX;
	double locatieY;
	unsigned int nrCurse;
	TCursa** curse;
} TSofer;


void dezalocSofer(TSofer* sofer) {
	for (int i = 0; i < sofer->nrCurse; i++) {
		free(sofer->curse[i]);
	}
	free(sofer->curse);

	return;
}

/*1.7.Definiți o funcție care eliberează toată memoria alocată :*/

void dezalocaSoferi(TSofer** soferi, int nrSoferi) {
	for (int i = 0; i < nrSoferi; i++) {
		dezalocSofer(soferi[i]);
	}
	free(soferi);

	return;
}



int main() {



	return 0;
}