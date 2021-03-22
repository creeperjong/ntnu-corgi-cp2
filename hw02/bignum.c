#include "bignum.h"

void align( sBigNum *num01 , sBigNum *num02 ){

    int32_t diff = 0;
    char tmp[1025] = "";

    if(num01->digit == num02->digit) return;
    if(num01->digit > num02->digit){
        diff = num01->digit - num02->digit;
        strncpy(tmp,num02->val,num02->digit);
        memset(num02->val,0,sizeof(num02->val));
        strncpy(&num02->val[diff],tmp,num02->digit);
        memset(num02->val,'0',diff);
    }
    if(num01->digit < num02->digit){
        diff = num02->digit - num01->digit;
        strncpy(tmp,num01->val,num01->digit);
        memset(num01->val,0,sizeof(num01->val));
        strncpy(&num01->val[diff],tmp,num01->digit);
        memset(num01->val,'0',diff);
    }
    return;
}

void print( const sBigNum num ){
    if(num.pn == -1) printf("-");
    for(int32_t i = num.digit - 1;i >= 0;i--) printf("%c",num.val[i]);
    printf("\n");
    return;
}

int32_t set( sBigNum *pNum, char *str ){
    int32_t len = strlen(str);
    int32_t idx = 0;
    
    for(int32_t i = 0;i < len;i++){
        if((str[i] < '0' || str[i] > '9') && str[0] != '-' ) return 0;
        if(i != 0 && str[i] == '-') return 0;
        if(len == 1 && str[0] == '-') return 0;
    }

    if(str[0] != '-'){
        memset(pNum->val,0,sizeof(pNum->val));
        for(int32_t i = 0;i < len;i++) pNum->val[i] = str[len - 1 - i];
        pNum->digit = len;
        pNum->pn = 1;
    }
    if(str[0] == '-'){
        memset(pNum->val,0,sizeof(pNum->val));
        for(int32_t i = 0;i < len - 1;i++) pNum->val[i] = str[len - 1 - i];
        pNum->digit = len - 1;
        pNum->pn = -1;
    }

    return 1;
}

int32_t compare( const sBigNum num01 , const sBigNum num02 ){
    if(num01.pn == 1 && num02.pn == -1) return 1;
    if(num01.pn == -1 && num02.pn == 1) return -1;
    if(num01.pn == 1 && num01.digit > num02.digit) return 1;
    if(num01.pn == -1 && num01.digit > num02.digit) return -1;
    if(num01.pn == 1 && num01.digit < num02.digit) return -1;
    if(num01.pn == -1 && num01.digit < num02.digit) return 1;

    //Positive

    if(num01.pn == 1){
        for(int32_t i = num01.digit - 1;i >= 0;i--){
            if(num01.val[i] == num02.val[i]) continue;
            if(num01.val[i] > num02.val[i]) return 1;
            if(num01.val[i] < num02.val[i]) return -1;
        }
        return 0;
    }

    //Negative

    if(num01.pn == -1){
        for(int32_t i = num01.digit - 1;i >= 0;i--){
            if(num01.val[i] == num02.val[i]) continue;
            if(num01.val[i] > num02.val[i]) return -1;
            if(num01.val[i] < num02.val[i]) return 1;
        }
        return 0;
    }
}

int32_t digits( const sBigNum num ){
    return num.digit;
}

void add( sBigNum *pResult ,const sBigNum num01 , const sBigNum num02 ){
    sBigNum num01tmp = num01;
    sBigNum num02tmp = num02;
    int32_t max_digit = 0;
    int32_t carry = 0;

    if(num01tmp.pn == 1 && num02tmp.pn == -1){
        num02tmp.pn = 1;
        return subtract(pResult,num01tmp,num02tmp);
    }
    if(num01tmp.pn == -1 && num02tmp.pn == 1){
        num01tmp.pn = 1;
        return subtract(pResult,num02tmp,num01tmp);
    }

    //Initialize

    memset(pResult->val,0,sizeof(pResult->val));
    
    //align(&num01tmp,&num02tmp);
    if(num01tmp.digit >= num02tmp.digit) max_digit = num01tmp.digit;
    if(num01tmp.digit < num02tmp.digit) max_digit = num02tmp.digit;
    for(int32_t i = num01tmp.digit;i < max_digit;i++) num01tmp.val[i] = '0';
    for(int32_t i = num02tmp.digit;i < max_digit;i++) num02tmp.val[i] = '0';

    for(int32_t i = 0;i < max_digit;i++){
        pResult->val[i] = (num01tmp.val[i]_toINT + num02tmp.val[i]_toINT + carry) % 10 _toASCII;
        carry = (num01tmp.val[i]_toINT + num02tmp.val[i]_toINT + carry) / 10;
    }

    if(carry) pResult->val[max_digit] = carry _toASCII;

    pResult->digit = strlen(pResult->val);
    pResult->pn = num01tmp.pn;

    return;
}

void subtract( sBigNum *pResult , const sBigNum num01 , const sBigNum num02 ){
    sBigNum num01tmp = num01;
    sBigNum num02tmp = num02;
    int32_t max_digit = 0;
    int32_t idx = 0;
    char tmp[1025] = "";
    
    if(num01tmp.pn == 1 && num02tmp.pn == -1){
        num02tmp.pn = 1;
        return add(pResult,num01tmp,num02tmp);
    }
    if(num01tmp.pn == -1 && num02tmp.pn == 1){
        num02tmp.pn = -1;
        return add(pResult,num01tmp,num02tmp);
    }

    //Initialize

    memset(pResult->val,0,sizeof(pResult->val));

    if(num01tmp.pn == 1 && num02tmp.pn == 1){
        if(compare(num01tmp,num02tmp) == -1){
            pResult->pn = -1;
            return subtract(pResult,num02tmp,num01tmp);
        }
    }else if(num01tmp.pn == -1 && num02tmp.pn == -1){
        num01tmp.pn = 1;
        num02tmp.pn = 1;
        return subtract(pResult,num02tmp,num01tmp);
    }

    //align(&num01tmp,&num02tmp);
    if(num01tmp.digit >= num02tmp.digit) max_digit = num01tmp.digit;
    if(num01tmp.digit < num02tmp.digit) max_digit = num02tmp.digit;
    for(int32_t i = num01tmp.digit;i < max_digit;i++) num01tmp.val[i] = '0';
    for(int32_t i = num02tmp.digit;i < max_digit;i++) num02tmp.val[i] = '0';

    for(int32_t i = 0;i < max_digit;i++){
        pResult->val[i] = num01tmp.val[i] - num02tmp.val[i] _toASCII;
        if(pResult->val[i] < '0'){
            pResult->val[i] += 10;
            num01tmp.val[i+1]--;
        }
    }

    while(pResult->val[idx] != 0) idx++;
    while(pResult->val[--idx] == '0') pResult->val[idx] = 0;

    pResult->digit = strlen(pResult->val);
    if(pResult->pn != -1) pResult->pn = num01tmp.pn;

    return;
}

void multiply( sBigNum *pResult , const sBigNum num01 , const sBigNum num02 ){

    sBigNum num01tmp = num01;
    sBigNum num02tmp = num02;
    sBigNum nowSum = {
        .val = "",
        .digit = 0,
        .pn = 1
    };
    sBigNum preSum = {
        .val = "",
        .digit = 0,
        .pn = 1
    };
    int32_t max_digit = 0;
    int32_t carry = 0;
    int32_t shift = 0;
    int32_t idx = 0;
    char tmp[1025] = "";

    //Initialize

    memset(pResult->val,0,sizeof(pResult->val));

    if(num01tmp.digit >= num02tmp.digit) max_digit = num01tmp.digit;
    if(num01tmp.digit < num02tmp.digit) max_digit = num02tmp.digit;
    for(int32_t i = num01tmp.digit;i < max_digit;i++) num01tmp.val[i] = '0';
    for(int32_t i = num02tmp.digit;i < max_digit;i++) num02tmp.val[i] = '0';

    for(int32_t i = 0;i < num01tmp.digit;i++){
        preSum = nowSum;
        memset(nowSum.val,0,sizeof(nowSum.val));
        memset(nowSum.val,'0',shift);
        for(int32_t j = 0;j < num02tmp.digit;j++){
            nowSum.val[j + shift] = ((num01tmp.val[i]_toINT) * (num02tmp.val[j]_toINT) + carry) % 10 _toASCII;
            carry = ((num01tmp.val[i]_toINT) * (num02tmp.val[j]_toINT) + carry) / 10;
        }
        if(carry) nowSum.val[num02tmp.digit + shift] = carry _toASCII;

        nowSum.digit = strlen(nowSum.val);
        add(&nowSum,nowSum,preSum);
        
        carry = 0;
        shift++;
    }

    //output

    *(pResult) = nowSum;
    pResult->pn = num01tmp.pn * num02tmp.pn;
    pResult->digit = strlen(&pResult->val[idx]);

    return;
}

void divide( sBigNum *pQuotient , sBigNum *pRemainder , const sBigNum num01 , const sBigNum num02 ){
    
    sBigNum num01tmp = num01;
    sBigNum num02tmp = num02;
    sBigNum multi_10 = {
        .val = "",
        .digit = 0,
        .pn = 1
    };
    sBigNum count = {
        .val = "",
        .digit = 0,
        .pn = 1
    };
    sBigNum countUnit = {
        .val = "",
        .digit = 0,
        .pn = 1
    };
    sBigNum one = {
        .val = "",
        .digit = 1,
        .pn = 1
    };
    sBigNum nowUnit = {
        .val = "",
        .digit = 0,
        .pn = 1
    };

    //Initialize

    memset(pQuotient->val,0,sizeof(pQuotient->val));
    memset(pRemainder->val,0,sizeof(pRemainder->val));
    num01tmp.pn = 1;
    num01tmp.pn = 1;
    one.val[0] = '1';

    memset(multi_10.val,'0',num01tmp.digit);
    multi_10.val[num01tmp.digit - 1] = '1';
    multi_10.digit = num01tmp.digit;

    do{
        multiply(&nowUnit,num02tmp,multi_10);

        if(compare(num01tmp,nowUnit) != -1){
            multiply(&countUnit,one,multi_10);
            while(compare(num01tmp,nowUnit) != -1){
                subtract(&num01tmp,num01tmp,nowUnit);
                add(&count,count,countUnit);
            }
        }
        
        if(multi_10.digit > 1){
            multi_10.val[multi_10.digit - 1] = 0;
            multi_10.val[multi_10.digit - 2] = '1';
            multi_10.digit--;
        }

    }while(compare(nowUnit,num02tmp) != 0);

    *(pQuotient) = count;
    *(pRemainder) = num01tmp;

    if(num01.pn == 1 && num02.pn == 1){
        pQuotient->pn = 1;
        pRemainder->pn = 1;
    }
    if(num01.pn == 1 && num02.pn == -1){
        pQuotient->pn = 1;
        pRemainder->pn = 1;
    }
    if(num01.pn == -1 && num02.pn == 1){
        pQuotient->pn = -1;
        pRemainder->pn = -1;
    }
    if(num01.pn == -1 && num02.pn == -1){
        pQuotient->pn = 1;
        pRemainder->pn = -1;
    }

    return;
}