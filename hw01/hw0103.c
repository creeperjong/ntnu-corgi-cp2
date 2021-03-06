#include <stdio.h>
#include "hw0103.h"

int main(){

    char* tmp = NULL;

    tmp = pStr01;
    pStr01 = pStr02;
    pStr02 = tmp;

    print_answers();

    return 0;
}