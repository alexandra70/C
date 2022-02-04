#include <stdio.h>
#include <string.h>

int main() {

    char *s1 = "abcd"; //rodata
    char s2[30] = "abcd"; //aloca pe stack
    //30 de octeti, in mod normal ar trebui sa puna
    // \0 dupa sir (pune el nul terminator);


    //if(true) executa - o sa zica ca sirurile sunt la fel
    if(strcmp(s1, s2) != 0) {
        printf("siruri la fel\n");
    }
    //pt ca strcmp pe doua siruri care dif e != 0
    else printf("siruri diferite\n");

    printf("%d\n", strlen(s2));


    strcpy(s2, s1);
    printf("%s\n", s1);

    strcpy(s1, s2);
    printf("%s\n", s2);
    
    return 0;
}