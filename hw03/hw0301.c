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
    int64_t preTime = 0;
    int64_t nowTime = 0;
    int64_t waitTime = 0;

    //Open the file

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

    //color is a struct array that stored the color correspond to name

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

        if(nowstr[0] == '[' && (nowstr[1] < '0' || nowstr[1] > '9')) continue;  //Other unrelated data tag
        if(nowstr[0] == '[' && nowstr[1] >= '0' && nowstr[1] <= '9'){   //The line which has a time tag
            nowTime = (nowstr[1] - '0') * 60000;
            nowTime += (nowstr[2] - '0') * 6000;
            nowTime += (nowstr[4] - '0') * 1000;
            nowTime += (nowstr[5] - '0') * 100;
            nowTime += (nowstr[7] - '0') * 10 + nowstr[8] - '0';
            waitTime = (nowTime - preTime) * 10000;     //The parameter of usleep is in the unit of microsecond(10^-6 s)

            usleep(waitTime);

            printf("%s%s"RESET,nowColor,&nowstr[10]);   //Render the subtitle and print
            printf("\n");
            preTime = nowTime;
        }
        else if(strrchr(nowstr,':') != NULL){
            for(int32_t i = 0;i < 8;i++){
                if(strncmp(nowstr,color[i].name,64) == 0){  //already has a color
                    nowColor = color[i].color;
                    break;
                }
                if(color[i].isOccupied == 0){   //assign a new color
                    strncpy(color[i].name,nowstr,32);
                    color[i].isOccupied = 1;
                    nowColor = color[i].color;
                    break;
                }
            }
        }
    }



    fclose(lrc);
    

    return 0;
}