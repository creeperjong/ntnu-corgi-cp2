#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "mixed.h"

int main(){

    int32_t a = 0;
    int32_t b = 0;
    int32_t c = 0;
    sMixedNumber input = {0};
    sMixedNumber* ptr = &input;
    sMixedNumber r1 = {1,2,3};
    sMixedNumber r2 = {4,5,6};

    mixed_add(ptr,r1,r2);
    printf("Add: ");
    printf("print: %d\n",mixed_print(input));

    mixed_sub(ptr,r1,r2);
    printf("Sub: ");
    printf("print: %d\n",mixed_print(input));

    return 0;
}