#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

int main(){

    int32_t ans = 0;
    int32_t count = 0;
    int32_t choice = 0;

    srand(time(0));
    for(int32_t i = 0;i < 10;i++){
        printf("Please enter your choice(1: odd, 2:even):");
        scanf("%d",&choice);
        ans = rand() % 100 + 1;
        if(ans % 2 == 0){
            if(choice == 2){
                
            }else{

            }
        }else{
            if(choice == 1){

            }else{

            }
        }


    }

    return 0;
}