#include <stdio.h>

int main(){

    char c = 0;
    char cb = 0;
    FILE* pFile = fopen("test.txt","rb");

    fseek(pFile,0,SEEK_SET);
    fread(&c,1,1,pFile);
    printf("%c\n",c);
    printf("%p\n",pFile);

    pFile++;
    printf("%p\n",pFile);

    fread(&cb,1,1,pFile);
    printf("%c\n",c);

    fclose(pFile);

    return 0;
}