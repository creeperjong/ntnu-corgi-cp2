#include <stdio.h>
#include <stdint.h>

void max(int32_t a,int32_t b);

void max(int32_t a,int32_t b){
    printf("%d\n",(a & (-(a>b))) + (b & (-(b>a))));
}

int main(){

    int32_t a = 0;
    int32_t b = 0;

    printf("Please enter a and b: ");
    scanf("%d %d",&a,&b);

    printf("Max: ");
    max(a,b);

    return 0;
}