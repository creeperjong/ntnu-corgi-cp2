#include <stdio.h>
#include <stdint.h>

#define BIT_1 input.bits.b1
#define BIT_2 input.bits.b2
#define BIT_3 input.bits.b3
#define BIT_4 input.bits.b4
#define BIT_5 input.bits.b5
#define BIT_6 input.bits.b6
#define BIT_7 input.bits.b7
#define BIT_8 input.bits.b8

typedef union{
    struct{
        unsigned char b1:1;
        unsigned char b2:1;
        unsigned char b3:1;
        unsigned char b4:1;
        unsigned char b5:1;
        unsigned char b6:1;
        unsigned char b7:1;
        unsigned char b8:1;
    } bits;
    unsigned char byte;
} uByte;

int main(){

    uByte input;
    int32_t choice = 0;

    printf("Please enter a byte (0-255): ");
    scanf("%hhd",&input.byte);

    do{
        printf("Data: %d %d%d%d%d%d%d%d%d\n",input.byte,BIT_8,BIT_7,BIT_6,BIT_5,BIT_4,BIT_3,BIT_2,BIT_1);
        printf("Flip bit (1-8, 0: exit): ");
        scanf("%d",&choice);

        if(choice != 0){
            if(choice == 1) BIT_8 = !BIT_8;
            if(choice == 2) BIT_7 = !BIT_7;
            if(choice == 3) BIT_6 = !BIT_6;
            if(choice == 4) BIT_5 = !BIT_5;
            if(choice == 5) BIT_4 = !BIT_4;
            if(choice == 6) BIT_3 = !BIT_3;
            if(choice == 7) BIT_2 = !BIT_2;
            if(choice == 8) BIT_1 = !BIT_1;
        }

    }while(choice != 0);
    printf("Bye\n");

    return 0;
}