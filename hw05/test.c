#include <stdio.h>
#include <stdint.h>

int main(){

    char c = 0;
    FILE* test = NULL;

    test = fopen("test.txt","rb");

    while((c = fgetc(test)) != -1) printf("%c",c);
    printf("\n");

    fclose(test);

    return 0;
}