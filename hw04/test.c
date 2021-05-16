#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(){

    char str[] = "--33";
    char* end = NULL;
    int64_t num = 0;

    num = strtol(str,&end,10);

    printf("%ld %ld\n",num,end - str);
    
    
    return 0;
}