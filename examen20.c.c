


typedef struct{
	unsigned char[3] param;
	unsigned int num;
	unsigned long int* termeni;
	unsigned long int* (*generator)(unsigned char[3], unsigned int);
}TSerie;

typedef struct{
	char* desc;
	unsigned int numSerii;
	TSerie** serii;
}TColectie;



unsigned long int* aritmetica(unsigned char[3] params, unsigned int cate){
    unsigned long int* ret = malloc(cate * sizeof(unsigned long int));
    ret[0] = (unsigned long int)params[2];
    for (int i = 1; i < cate; i++)
        ret[i] = ret[i-1] + params[1];
    return ret;
}

double calc_medie(unsigned int num, unsigned long int* termeni) {

    double sum = 0;
    for(int i = 0; i < num; i ++ ) {
        sum += termeni[i];
    }

    if(sum == 0)
        return (double)0;
    return (sum/(double)num);
}

unsigned long int* geometrica(unsigned char[3] params, unsigned int cate) { 
    //generez cate numere din progresia geometrica->
    // long int replace %d with %ld.
    unsigned long int* ret = malloc(cate * sizeof(unsigned long int));
    ret[0] = (unsigned long int)params[2];
    for(int i = 1; i < cate; i ++ ) { 
        ret[i] = ret[i - 1] * params[1];
    }
    
    return ret;
}

unsigned long int* fibo(unsigned char[3] params, unsigned int cate) { 
    
    unsigned long int* ret = malloc(cate * sizeof(unsigned long int));
    ret[0] = 0;
    ret[1] = 1; 

    for(int i = 2; i < cate; i ++ ) { 
        ret[i] = ret[i - 2] + ret[i - 1];
    }
    
    return ret;
}

unsigned long int* primes(unsigned char[3] params, unsigned int cate) { 
    //generez cate numere din progresia geometrica->
    // long int replace %d with %ld.
    unsigned long int* ret = malloc(cate * sizeof(unsigned long int));
    ret[0] = 2; //primu prim

    int repe = cate - 1;
    int next_nr = ret[0] + 1;
    int k = 1;
    int ok = 1;
    while(repe) {

        ok = 1; //orice nr poate in regula de adauat in list.

        for(int i = 2; i < next_nr/2; j ++) {
            if(next_nr%j == 0) { 
                ok = 0; //nu mai e ok de aduagt in lista, pt c anu e prim
                break;
            }
        }

        if(ok == 1) {
            //adaug nr in sir
            ret[k++] = next_nr;
            repe--;
        }
    
        next_nr++; //oricum trebuie sa cresaca, ca la un moment dat trebe sa verifie coditia de a fi prim.
    }
    
    return ret;
}


TSerie* alocaSerie(char[3] params, int num, unsigned long int* (*generator)(unsigned char[3], unsigned int)){
    TSerie* serie = malloc(sizeof(TSerie));
    serie->param = params;
    serie->num = num;
    serie->generator = generator;
    serie->termeni = (*(serie->generator))(serie->params,serie->num);
    return serie;
}

//numTerms e cate am de fiecre - nr de eem din fiecare serie.
TColectie* alocaColectie(unsigned int numSerii, unsigned int *numTerms) {
    TColectie* cole = malloc(sizeo(TColectie));
    cole->numSerii = numSerii;
    cole->serii = (TSerie**)malloc(cole->numSerii * (sizeof(TSerie*)));
    //aloc pt s aritmetca
    char params[3];
    params[0] = 2;
    params[1] = 1;
    params[2] = 1;
    for(int i = 0; i < cole->numSerii; i ++) {
        cole->serii[i] = alocaSerie(params, &numTerms, aritmetica);
    }

    return cole;
}

void dezaloc(TColectie** colectie) {
    //in sfarsit are sens dublu pointer, nu?
    //dezaloc serii
    for( int i = 0; i < (*colectie)->numSerii; i ++) {
        free(((*colectie)->serii[i])->termeni);
        //dau free si la elementul de tipul TSerie din vector
        free((*colectie)->serii[i]);
    }
    //free(((*colectie)->serii[i]));
    //(*colectie).num = 0;
    free((*colectie)->desc);
    free(*colectie); //??? dc e pointer

    return;
}

void completeazaSiScrieDescrierea(TColectie** colectie) { 
    double media;
    (*colectie)->desc = calloc(40 * (*colectie)->numSerii , sizeof(char));
    memset((*colectie)->desc, 0, 40 * (*colectie)->numSerii);

    for(int i = 0; i < (*colectie)->numSerii; i ++) {
        // am colectie->serii[i]
        int type = ((*colectie)->serii[i]).param[0].;
        media = calc_medie((*colectie)->serii[i].num, (*colectie)->serii[i].termeni);
        if(type == 0) {
            //prime
            sprintf((*colectie)->desc + strlen((*colectie)->desc), "Seria %d de tip prime cu media %.1f.\n", i, media);
        }
        else if(type == 1) {
            //fibonacii
            sprintf((*colectie)->desc + strlen((*colectie)->desc), "Seria %d de tip fibonacii cu media %.1f\n", i, media);
        }
        else if (type == 2) {
            //aritmetice
            sprintf((*colectie)->desc + strlen((*colectie)->desc), "Seria %d de tip aritmetice cu media %.1f\n", i, media);
        }
        else if (type == 3) {
            //geometrice
            sprintf((*colectie)->desc + strlen((*colectie)->desc), "Seria %d de tip geometrice cu media %.1f\n", i, media);
        }
    }
    
    FILE* fout = fopen("out.txt", "wt");
    fprintf(fout, "%s", (*colectie)->desc);
    fclose(fout);
}

unsigned long long termenComun(TColectie** colectie) {

    long long comun = -1;
    if((*colectie)->numSerii < 2)
        return -1; //am cel mult o serie

    unsigned long int comune[(*colectie)->serii[0]->num];

    //iau o copie a seriei 1 in comune si apoi incep sa verifica de la indexul 1 din coletie->serii

    int o = 0;
    for(int j = 0; j < (*colectie)->serii[0]->num; j++) {
        for(int k = 0; k < (*colectie)->serii[1]->num; k++) {
            //vad cs doua serii consecutive au un termen in comun, dc nu atunci nu mai caut deloc
            if(((*colectie)->serii[0]).termeni[j] == ((*colectie)->serii[1]).termeni[k]) {
                comune[o++] = ((*colectie)->serii[1]).termeni[k];
            }
        }
    }

    int ok = 0;
    for(int k = 2; k <  (*colectie)->numSerii; k ++) {
        for(int i = 0; i < o; i++) {
            ok = 0;
            for(int j = 0; j < (*colectie)->serii[k]->num; j++) { 
                if(comune[i] == ((*colectie)->serii[k]).termeni[j]) {
                    ok = 1;
                    break;
                }
            }
            if(ok == 0) {
                //scot elementul de poz i
                comune[i] = -1; //il invalidez
            }
        }
    }

    for(int i = 0; i < o; i ++) {
        if (comune[i] > comun) {
            comun = comune[i];
        }
    }
    
    return comun;
}

int main(){
    TSerie* serieAritmetica = alocaSerie(params, num, aritmetica);
    int nr_terment = 10;
    TColectie* cole = alocaColectie(cate_vreu, &nr_termeni);
    completeazaSiScrieDescrierea(&cole);
    printf("%lld", termenComun(&cole));
    dezaloc(&cole);


}