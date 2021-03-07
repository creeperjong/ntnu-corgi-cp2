#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void myperror(const char* str);

void myperror(const char* str){

    char* ans = (char*)malloc(sizeof(str));
    strncpy(ans,str,strlen(str));

    ans = strncat(ans,": ",3);
    ans = strncat(ans,strerror(errno),strlen(strerror(errno)));
    printf("%s\n",ans);

    free(ans);
}

int main(){

    FILE* file = NULL;

    file = fopen("IDontExist.txt","r");
    
    if(file == NULL){
        printf("Error(strerror): %s\n",strerror(errno));
        myperror("Error(myperror)");
    }
     
    return 0;
}