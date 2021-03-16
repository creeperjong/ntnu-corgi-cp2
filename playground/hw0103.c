#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include"hw0103.h"

int main()
{
   char *temp = pStr01;
   pStr01 = pStr02;
   pStr02 = temp;
   print_answers();
   return 0; 
}