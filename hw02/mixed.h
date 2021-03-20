#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct _sMixedNumber{
    int32_t whole;
    int32_t numberator;
    int32_t denominator;
} sMixedNumber;

int gcd(int32_t x, int32_t y);
//gcd
int check(const sMixedNumber* pNumber);
//check
void simply(sMixedNumber* pNumber);
//simply pNumber
int mixed_set(sMixedNumber* pNumber, int32_t a, int32_t b, int32_t c);
// return -1 if invalid; otherwise , return 0.
int mixed_print(const sMixedNumber number);
// in the form of (a,b,c)
void mixed_add(sMixedNumber* pNumber, const sMixedNumber r1, const sMixedNumber r2);
// pNumber = r1 + r2
void mixed_sub(sMixedNumber* pNumber, const sMixedNumber r1, const sMixedNumber r2);
// pNumber = r1 - r2
void mixed_mul(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2);
// pNumber = r1 * r2
void mixed_div(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2);
// pNumber = r1 / r2