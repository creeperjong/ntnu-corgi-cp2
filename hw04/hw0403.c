#include <stdio.h>
#include "myprintf.h"

int main(){
    char* str = "Hello World!";
    uint32_t X = 655;
    uint32_t x = 45;
    int32_t d = -130;
    int32_t i = -62;

    myprintf("test: %s %X %x %d %i\n",str,X,x,d,i);
    return 0;
}