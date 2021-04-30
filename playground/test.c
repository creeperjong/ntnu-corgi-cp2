#include <stdio.h>

int main(){
    char str[10] = "";

    while(!feof(stdin)){
        scanf("%s",str);
        printf("%s\n",str);
        for(int i = 0;i < 10;i++) printf("%d ",str[i]);
        printf("\n");
    }
    return 0;
}