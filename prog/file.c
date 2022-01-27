#include <stdio.h>

int main() {

    int x[2] = {5, 6};
    const int* p = &x;

   // printf("|%d|\n", *(++p)); ----- ce afiseaza linia asta

    /* ideea e ca *p = adresa lui x
        x exista undeva in memorie si are o adresa, acest x e de fapt
        incpeutul vectorului meu. Acum pointerul meu p pointeaza spre adresa lui x
        adica unde gasesc adresa adresa vectorului. Acum dc ma uit sa vad ce e la acel pointer observ ca
        este de fapt evident x. adica p si x referea ac zona de memorie, dc ma uit sa vad ce gasesc 
        la p[1] vad ca e lafel ca in cazul lui v[1].
        deci p = ce gaseste la adresa lui x, iar la adresa lui x se afla evident adresa de niceput
        a vectorului meu. 
    */

     printf("|%d|\n", p);
     printf("|%d|\n", x);

    printf("|%d|\n", *p);
    printf("|%d|\n", &x);
    printf("eu mai sunt aici");

    return 0;
}