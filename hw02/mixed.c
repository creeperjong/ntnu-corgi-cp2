#include "mixed.h"

int gcd(int32_t x, int32_t y){
    if(y) while( (x %= y) && (y %= x));
    return x + y;
}

int32_t check(const sMixedNumber* pNumber){
    
    int32_t a = pNumber->whole;
    int32_t b = pNumber->numberator;
    int32_t c = pNumber->denominator;

    if(b != 0 && c == 0) return 0;
    if(b < 0 && a != 0) return 0;
    if(c < 0) return 0;
    if(pNumber == NULL) return 0;

    return 1;
}

void simply(sMixedNumber* pNumber){

    int32_t isNegative = 0;
    int32_t div = 0;
    int32_t a = pNumber->whole;
    int32_t b = pNumber->numberator;
    int32_t c = pNumber->denominator;

    if(b != 0 && c != 0){
        if(a > 0) pNumber->whole += b / c;
        else if(a == 0){
            pNumber->whole += b / c;
            if(b < 0) isNegative = 1;
        }
        else pNumber->whole -= b / c;

        b %= c;
        b = abs(b);
        if(b == 0){
            pNumber->numberator = 0;
            pNumber->denominator = 0;
        }else{
            div = gcd(c,b);

            if(div != 1){
                b /= div;
                c /= div;
            }

            if(isNegative && pNumber->whole == 0) pNumber->numberator = -b;
            else pNumber->numberator = b;
            pNumber->denominator = c;
        }
    }else{
        pNumber->numberator = 0;
        pNumber->denominator = 0;
    }
}

int mixed_set(sMixedNumber* pNumber, int32_t a, int32_t b, int32_t c){
    
    pNumber->whole = a;
    pNumber->numberator = b;
    pNumber->denominator = c;
    
    if(!check(pNumber)){
        pNumber->whole = 0;
        pNumber->numberator = 0;
        pNumber->denominator = 0;
        return -1;
    }
    else simply(pNumber);

    return 0;
}

int mixed_print(const sMixedNumber number){
    if(!check(&number)) return -1;
    printf("(%d,%d,%d)\n",number.whole,number.numberator,number.denominator);
    return 0;
}

void mixed_add(sMixedNumber* pNumber, const sMixedNumber r1, const sMixedNumber r2){
    
    if(!check(&r1) || !check(&r2)){
        printf("Error\n");
        return;
    }

    sMixedNumber r1tmp = (sMixedNumber)r1;
    sMixedNumber r2tmp = (sMixedNumber)r2;

    if(r1tmp.whole < 0) r1tmp.numberator = r1tmp.whole * r1tmp.denominator - r1tmp.numberator;
    else r1tmp.numberator = r1tmp.whole * r1tmp.denominator + r1tmp.numberator;
    if(r2tmp.whole < 0) r2tmp.numberator = r2tmp.whole * r2tmp.denominator - r2tmp.numberator;
    else r2tmp.numberator = r2tmp.whole * r2tmp.denominator + r2tmp.numberator;

    pNumber->whole = 0;
    pNumber->denominator = r1tmp.denominator * r2tmp.denominator;
    pNumber->numberator = r1tmp.numberator * r2tmp.denominator + r2tmp.numberator * r1tmp.denominator;
    simply(pNumber);
    
    return;
}

void mixed_sub(sMixedNumber* pNumber, const sMixedNumber r1, const sMixedNumber r2){
    
    if(!check(&r1) || !check(&r2)){
        printf("Error\n");
        return;
    }

    sMixedNumber r1tmp = (sMixedNumber)r1;
    sMixedNumber r2tmp = (sMixedNumber)r2;

    if(r1tmp.whole < 0) r1tmp.numberator = r1tmp.whole * r1tmp.denominator - r1tmp.numberator;
    else r1tmp.numberator = r1tmp.whole * r1tmp.denominator + r1tmp.numberator;
    if(r2tmp.whole < 0) r2tmp.numberator = r2tmp.whole * r2tmp.denominator - r2tmp.numberator;
    else r2tmp.numberator = r2tmp.whole * r2tmp.denominator + r2tmp.numberator;

    pNumber->whole = 0;
    pNumber->denominator = r1tmp.denominator * r2tmp.denominator;
    pNumber->numberator = r1tmp.numberator * r2tmp.denominator - r2tmp.numberator * r1tmp.denominator;
    simply(pNumber);
    
    return;
}