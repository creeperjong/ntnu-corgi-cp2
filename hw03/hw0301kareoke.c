/* 
    This is not the offical version of hw0301,
    This program is just for fun.   
    It is designed to display the subtitle in a more dynamic way,
    just like you see in kareoke.
    You can run it by yourself to see its effect :) 
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RED "\033[1;91m"
#define GREEN "\033[1;92m"
#define YELLOW "\033[1;93m"
#define BLUE "\033[1;94m"
#define PURPLE "\033[1;95m"
#define CYAN "\033[1;96m"
#define BLUE_WHITE "\033[1;97;104m"
#define RED_WHITE "\033[1;97;101m"
#define RESET "\033[m"

typedef struct{
    char name[32];
    char* color;
    int32_t isOccupied;
} sColor;

int main(){
    FILE* lrc = NULL;
    char filename[64] = "";
    char nowstr[64] = "";
    char* nowColor = NULL;
    char* preColor = NULL;
    char clear[12] = {0x1b, 0x5b, 0x48, 0x1b, 0x5b, 0x32, 0x4a, 0x1b, 0x5b, 0x33, 0x4a, 0x00};
    char output[2][64] = {0};
    
    int64_t preTime = 0;
    int64_t nowTime = 0;
    int64_t waitTime = 0;
    int32_t changeColor = 0;

    printf("Open a LRC file: ");
    fgets(filename,64,stdin);
    filename[strlen(filename) - 1] = 0;


    if(strncmp(&filename[strlen(filename) - 4],".lrc",4) != 0){
        printf("Error: The file name extension is not \".lrc\"\n");
        return 0;
    }
    if((lrc = fopen(filename,"r")) == NULL){
        perror("Error");
        return 0;
    }

    sColor color[8] = {0};
    color[0].color = RED;
    color[1].color = BLUE;
    color[2].color = GREEN;
    color[3].color = CYAN;
    color[4].color = PURPLE;
    color[5].color = YELLOW;
    color[6].color = BLUE_WHITE;
    color[7].color = RED_WHITE;


    while(!feof(lrc)){
        memset(nowstr,0,64);
        fgets(nowstr,64,lrc);
        nowstr[strlen(nowstr) - 1] = 0;

        if(nowstr[0] == '[' && (nowstr[1] < '0' || nowstr[1] > '9')) continue;
        if(nowstr[0] == '[' && nowstr[1] >= '0' && nowstr[1] <= '9'){
            nowTime = (nowstr[1] - '0') * 60000;
            nowTime += (nowstr[2] - '0') * 6000;
            nowTime += (nowstr[4] - '0') * 1000;
            nowTime += (nowstr[5] - '0') * 100;
            nowTime += (nowstr[7] - '0') * 10 + nowstr[8] - '0';
            waitTime = (nowTime - preTime) * 10000;

            strncpy(output[0],output[1],64);
            strncpy(output[1],&nowstr[10],64);

            for(int32_t i = 1;i <= strlen(output[1]);i++){
                printf("%s",clear);
                if(preColor != NULL) printf("%s",preColor);
                printf("%s"RESET,output[0]);
                printf("\n");
                printf("%s%.*s"RESET,nowColor,i,output[1]);
                if(i < strlen(output[1])) printf("%s",&output[1][i]);
                printf("\n");
                usleep(waitTime / strlen(output[1]));
            }
            
            preTime = nowTime;
            if(changeColor == 1){
                preColor = nowColor;
                changeColor = 0;
            }
        }
        else if(strrchr(nowstr,':') != NULL){
            for(int32_t i = 0;i < 8;i++){
                if(strncmp(nowstr,color[i].name,64) == 0){
                    preColor = nowColor;
                    nowColor = color[i].color;
                    changeColor = 1;
                    break;
                }
                if(color[i].isOccupied == 0){
                    strncpy(color[i].name,nowstr,32);
                    color[i].isOccupied = 1;
                    preColor = nowColor;
                    nowColor = color[i].color;
                    changeColor = 1;
                    break;
                }
            }
        }
    }


    fclose(lrc);
    

    return 0;
}