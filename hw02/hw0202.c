#include <stdio.h>
#include <stdint.h>

typedef union _doubleBit{
    double dou;
    int64_t integer;
} doubleBit;

int main(){

    doubleBit input;
    double tmp = 0;
    int32_t fractionIsZero = 1;
    int32_t expo = 0;
    int32_t bi = 1;
    int32_t digit = 0;
    char sign = 0;
    char exp[12] = "";
    char fraction[53] = "";

    printf("Please enter a floating-point number (\e[91mdouble\e[m precision): ");
    scanf("%lf",&input.dou);

    tmp = input.dou;
    for(int32_t i = 51;i >= 0;i--){
        //printf("%ld\n",input.integer);
        fraction[i] = (input.integer & 1) + '0';
        input.integer >>= 1;
    }

    for(int32_t i = 10;i >= 0;i--){
        exp[i] = (input.integer & 1) + '0';
        input.integer >>= 1;
    }

    sign = (input.integer & 1) + '0';
    input.integer >>= 1;

    printf("Sign: %c\n",sign);
    printf("Exponent: %s\n",exp);
    printf("Fraction: %s\n",fraction);

    printf("%.10lf = (-1)^%c * ",tmp,sign);

    for(int32_t i = 0;i < 52;i++){
        if(fraction[i] != '0') fractionIsZero = 0;
    }

    if(!fractionIsZero){
        printf("(1");
        for(int32_t i = 0;i < 52;i++){
            if(fraction[i] == '1') printf(" + 2^-%d",i + 1);
        }
        printf(") * ");
    }

    for(int32_t i = 10;i >= 0;i--){
        if(exp[i] == '1') expo += bi;
        bi *= 2;
    }

    printf("2^(%d-1023)\n",expo);

    return 0;
}