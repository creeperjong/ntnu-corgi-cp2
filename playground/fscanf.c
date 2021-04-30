#include <stdio.h>
#include <stdint.h>

int main(){

    FILE* pFileR = NULL;
    FILE* pFileW = NULL;

    if((pFileR = fopen("data.txt","r")) == NULL){
        perror("Read:");
        return 0;
    }

    if((pFileW = fopen("list.txt","w")) == NULL){
        perror("Write:");
        return 0;
    }

    char account[20] = "";
    char password[20] = "";
    while(1){
        
        if(fgets(account,20,pFileR) == NULL) break;
        if(fgets(password,20,pFileR) == NULL) break;
        // scanf(pFileR,"%s %s",account,password);

        for(int32_t i = 0;i < 15;i++) printf("%d ",account[i]);
        printf("\n");
        for(int32_t i = 0;i < 15;i++) printf("%d ",password[i]);
        printf("\n");

        fprintf(pFileW,"Account:%sPassword:%s\n",account,password);
    }

    rewind(pFileR);

    while(1){
        
        if(fgets(account,20,pFileR) == NULL) break;
        if(fgets(password,20,pFileR) == NULL) break;
        // scanf(pFileR,"%s %s",account,password);

        for(int32_t i = 0;i < 15;i++) printf("%d ",account[i]);
        printf("\n");
        for(int32_t i = 0;i < 15;i++) printf("%d ",password[i]);
        printf("\n");

        fprintf(pFileW,"Account:%sPassword:%s\n",account,password);
    }

    fclose(pFileR);
    fclose(pFileW);

    return 0;
}