#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(){

    int32_t ascii = 0;
    int32_t ans_idx = 0;
    char hex[3] = "";
    char ans[1024] = "";
    char** garbage = NULL;

    do{
        fgets(hex,3,stdin);

        if(strncmp(hex,"00",2) != 0){
            ascii = (int32_t)strtol(hex,garbage,16);
            if(ascii < 32 || ascii > 126){
                printf("Warning: The string includes unprintable characters.\n");
                return 0;
            }else ans[ans_idx++] = ascii;
        }

    }while(strncmp(hex,"00",2) != 0);

    printf("%s\n",ans);

    return 0;
}