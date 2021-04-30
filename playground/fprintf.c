#include <stdio.h>
#include <stdint.h>

int main(){

    FILE* pFile = NULL;

    if( (pFile = fopen("list.txt","w")) == NULL){
        perror("LINE 8");
        return 0;
    }

    while(1){
        
        char account[20] = "";
        char password[20] = "";
        printf("Please enter the account: ");
        fgets(account,20,stdin);
        printf("Please enter the password: ");
        fgets(password,20,stdin);

        fprintf(pFile,"Account:%sPassword:%s\n",account,password);
    }

    fclose(pFile);
    return 0;
}