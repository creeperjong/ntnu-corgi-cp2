#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define _toINT -'0'
#define _toASCII +'0'

typedef struct _sBigNum{
    char val[1025];
    int32_t digit;
    int32_t pn;
} sBigNum;

void align( sBigNum *num01 , sBigNum *num02 );

void print( const sBigNum num );

int32_t set( sBigNum *pNum, char *str );

int32_t compare( const sBigNum num01 , const sBigNum num02 );

int32_t digits( const sBigNum num );

void add( sBigNum *pResult ,const sBigNum num01 , const sBigNum num02 );

void subtract( sBigNum *pResult , const sBigNum num01 , const sBigNum num02 );

void multiply( sBigNum *pResult , const sBigNum num01 , const sBigNum num02 );

void divide( sBigNum *pQuotient , sBigNum *pRemainder , const sBigNum num01 , const sBigNum num02 );

int32_t power( sBigNum *pResult , int32_t n, int32_t k );

int32_t combine( sBigNum *pResult , int32_t n, int32_t k );