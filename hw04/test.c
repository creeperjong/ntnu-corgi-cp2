#include <stdio.h>

int main(){

    char input = 0;
    FILE* pFile = NULL;

    if((pFile = fopen("test.txt","wb")) == NULL){
        perror("Error");
        return 0;
    }


    fseek(pFile,5000,SEEK_SET);
    scanf("%c",&input);
    fwrite(&input,1,1,pFile);

    rewind(pFile);

    return 0;
}