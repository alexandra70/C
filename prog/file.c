#include <stdio.h>
#include <string.h>
#include<stdlib.h>


void f(char* s) { 
    printf("|%c|", *s);

    if(strlen(s) >= 1) { 
        f(s + 1);
    }

   // printf("%s\n", (int*)s);
}

int main() {


///execrcitiul 2
    char* x = (char*)calloc(10, sizeof(int));

    for(int i = 0; i < 10; i ++ ) { 
        x[i] = i % 4 == 1; 
    }

     for(int i = 0; i < 10; i ++ ) { 
         //castul de mai jos nu e ok, pt ca eu vreau sa afisez un int, plus ca fac cast la un pointer
        printf("%d | %d\n", i , (int*)x[i]); 
    }
    


    //exercitiul 3
    char str[30] = "Trec cu nota mare la PC!";

    char bla[10] = "aaabbbccc";
    char *bla2 = bla+3;
    //bla = "bababababababa";
    memcpy(bla, bla2, 6);
    puts(bla);
    memcpy(bla + 17, str + 12, 13);
    puts(bla);

    memmove(str + 17, str + 12, 13);
    puts(str);
    memcpy(str + 17, str + 12, 13);
    puts(str);

    return 0;
}