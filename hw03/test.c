#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(){
    
    uint16_t search[3] = {9000,3000,3500};
    uint8_t buf[6] = {0};
    uint8_t dest[11] = {0xFF,0x12,0x56,0x28,0x23,0xB8,0x0B,0xAC,0x0D,0x12,0x99};

    // memcpy(buf,search,sizeof(buf));
    // for(int32_t i = 0;i < 6;i++) printf("%x ",buf[i]);
    // puts("");
    
    for(int32_t i = 0;i < 11;i++){
        if(memcmp(&dest[i],search,sizeof(search)) == 0){
            printf("%d\n",i);
            break;
        }
    }

    return 0;
}