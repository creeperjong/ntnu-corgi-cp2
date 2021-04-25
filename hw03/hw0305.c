#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define RED "\033[1;91m"
#define GREEN "\033[1;92m"
#define YELLOW "\033[1;93m"
#define BLUE "\033[1;94m"
#define PURPLE "\033[1;95m"
#define CYAN "\033[1;96m"
#define RESET "\033[m"

int main(){

    int32_t fd = -1;
    int32_t year = 0;
    int32_t month = 0;
    int32_t nowIdx = 0;
    uint64_t filesize = 0;
    char filename[32] = "";
    uint8_t* savedata = NULL;
    char clear[12] = {0x1b, 0x5b, 0x48, 0x1b, 0x5b, 0x32, 0x4a, 0x1b, 0x5b, 0x33, 0x4a, 0x00};
    struct stat filestat = {0};

    printf(GREEN"Welcome to SAN5PK Savedata Editor!\n"RESET);

    // while(1){
    //     printf("Please enter the file name (It should be \"SAVEDATA.S5P\"): ");

    //     if(fgets(filename,32,stdin) != NULL){
    //         filename[strlen(filename) - 1] = 0;
    //         if(strncmp(&filename[strlen(filename) - 4],".S5P",4) == 0){
    //             fd = open(filename, O_RDWR);
                
    //             if(fd < 0) printf("Error: No such file or directory\n");
    //             else break;
    //         }
    //         else printf("Warning: The file extension is not \".S5P\"\n");
    //     }
    //     else printf("Error!\n");
    // }

    fd = open("SAVEDATA.S5P",O_RDWR);

    fstat(fd,&filestat);
    filesize = filestat.st_size;
    savedata = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    while(1){
        printf("Please enter the year and the month in the savedata(format: yyy mm): ");
        scanf("%d %d",&year,&month);

        int32_t isFound = 0;
        uint8_t search[8] = {0};
        search[5] = year % 256;
        search[6] = year / 256;
        search[7] = month;


        for(int32_t i = 0;i < filesize;i++){
            if(memcmp(&savedata[i],search,8) == 0){
                nowIdx = i;
                printf("i = %x\n",i+5);
                isFound = 1;
                break;
            }
        }
        if(isFound) break;
    }

    

    munmap(savedata, filesize);
    close(fd);

    return 0;
}