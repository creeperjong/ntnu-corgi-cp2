#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[32];
    int32_t point;
    int32_t win;
    int32_t draw;
    int32_t lose;
    int32_t goalScored;
    int32_t goalAgainst;
    int32_t redCards;
    int32_t winHome;
    int32_t winAway;
    int32_t isOccupied;
} sTeam;

void calculate_the_data(FILE* record,sTeam* team_data,char* the_most_scoring_gap);

void calculate_the_data(FILE* record,sTeam* team_data,char* the_most_scoring_gap){
    char nowData[512] = "";
    char homeName[32] = "";
    char awayName[32] = "";
    int32_t FTHG = 0;
    int32_t FTAG = 0;
    char FTR = 0;
    int32_t HR = 0;
    int32_t AR = 0;
    int32_t max_gap = INT32_MIN;
    int32_t firstLine = 1;

    while(fgets(nowData,512,record) != NULL){

        nowData[strlen(nowData) - 1] = 0;
        
        if(firstLine){  //skip the first line
            firstLine = 0;
            continue;
        }

        //Use regular expression to parse the string by sscanf

        sscanf(nowData,"%*[^,],%[^,],%[^,],%d,%d,%c,%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%d,%d,%*s",homeName,awayName,&FTHG,&FTAG,&FTR,&HR,&AR);

        //Home

        for(int32_t i = 0;i < 20;i++){
            if(strncmp(team_data[i].name,homeName,32) == 0 || team_data[i].isOccupied == 0){

                if(team_data[i].isOccupied == 0){
                    team_data[i].isOccupied = 1;
                    strncpy(team_data[i].name,homeName,32);
                }    

                if(FTR == 'H'){
                    team_data[i].point += 3;
                    team_data[i].win += 1;
                    team_data[i].winHome += 1;
                }
                else if(FTR == 'A') team_data[i].lose += 1;
                else{
                    team_data[i].point += 1;
                    team_data[i].draw += 1;
                }

                team_data[i].goalScored += FTHG;
                team_data[i].goalAgainst += FTAG;
                team_data[i].redCards += HR;

                break;
            }
        }

        //Away

        for(int32_t i = 0;i < 20;i++){
            if(strncmp(team_data[i].name,awayName,32) == 0 || team_data[i].isOccupied == 0){

                if(team_data[i].isOccupied == 0){
                    team_data[i].isOccupied = 1;
                    strncpy(team_data[i].name,awayName,32);
                }    

                if(FTR == 'A'){
                    team_data[i].point += 3;
                    team_data[i].win += 1;
                    team_data[i].winAway += 1;
                }
                else if(FTR == 'H') team_data[i].lose += 1;
                else{
                    team_data[i].point += 1;
                    team_data[i].draw += 1;
                }

                team_data[i].goalScored += FTAG;
                team_data[i].goalAgainst += FTHG;
                team_data[i].redCards += AR;

                break;
            }
        }

        //find the most scoring gap

        if(max_gap < abs(FTHG - FTAG)){
            snprintf(the_most_scoring_gap,512,"%.10s,%s(%d) vs %s(%d)",nowData,homeName,FTHG,awayName,FTAG);
            max_gap = abs(FTHG - FTAG);
        }
    }

}
    


int main(){
    
    FILE* record = NULL;
    int32_t choice = 0;
    char filename[64] = "";
    char the_most_scoring_gap[128] = "";

    //Open the file

    printf("Please open a season record: ");
    fgets(filename,64,stdin);
    filename[strlen(filename) - 1] = 0;

    if(strncmp(&filename[strlen(filename) - 4],".csv",4) != 0){
        printf("Error: The file name extension is not \".csv\"\n");
        return 0;
    }
    if((record = fopen(filename,"r")) == NULL){
        perror("Error");
        return 0;
    }

    printf("1) Who is the winner in this season?\n");
    printf("2) Which team gets the most scores?\n");
    printf("3) Which team gets the most red cards?\n");
    printf("4) Which team wins the most games at home?\n");
    printf("5) Which team wins the most games away from home?\n");
    printf("6) Which game has the most scoring gap?\n");
    printf("7) Team information.\n");
    printf("8) Exit\n");

    //store the data of every team

    sTeam team_data[20] = {0};
    calculate_the_data(record,team_data,the_most_scoring_gap);

    do{
        printf("Choice (1-7, 8:exit): ");
        scanf("%d",&choice);

        if(choice != 8){

            int32_t ans = INT32_MIN;
            int32_t ansIdx = 0;
            char team_search[32] = "";

            switch(choice){
                case 1:

                    for(int32_t i = 0;i < 20;i++){
                        if(ans < team_data[i].point){
                            ans = team_data[i].point;
                            ansIdx = i;
                        }
                    }

                    printf("The winner is %s.\n",team_data[ansIdx].name);
                    
                    break;
                case 2:
                    
                    for(int32_t i = 0;i < 20;i++){
                        if(ans < team_data[i].goalScored) ans = team_data[i].goalScored;
                    }
                    for(int32_t i = 0;i < 20;i++){
                        if(team_data[i].goalScored == ans) printf("%s, %d\n",team_data[i].name,ans);
                    }
                    
                    break;
                case 3:

                    for(int32_t i = 0;i < 20;i++){
                        if(ans < team_data[i].redCards) ans = team_data[i].redCards;
                    }
                    for(int32_t i = 0;i < 20;i++){
                        if(team_data[i].redCards == ans) printf("%s, %d\n",team_data[i].name,ans);
                    }
                    
                    break;
                case 4:

                    for(int32_t i = 0;i < 20;i++){
                        if(ans < team_data[i].winHome) ans = team_data[i].winHome;
                    }
                    for(int32_t i = 0;i < 20;i++){
                        if(team_data[i].winHome == ans) printf("%s, %d\n",team_data[i].name,ans);
                    }
                    
                    break;
                case 5:

                    for(int32_t i = 0;i < 20;i++){
                        if(ans < team_data[i].winAway) ans = team_data[i].winAway;
                    }
                    for(int32_t i = 0;i < 20;i++){
                        if(team_data[i].winAway == ans) printf("%s, %d\n",team_data[i].name,ans);
                    }
                    
                    break;
                case 6:
                    printf("%s\n",the_most_scoring_gap);
                    break;
                case 7:
                    fgets(team_search,32,stdin);    //Eliminate \n
                    printf("Team: ");
                    fgets(team_search,32,stdin);
                    team_search[strlen(team_search) - 1] = 0;

                    for(int32_t i = 0;i < 20;i++){
                        if(strncmp(team_search,team_data[i].name,32) == 0){
                            printf("Points: %d\n",team_data[i].point);
                            printf("Win/Draw/Lose: %d/%d/%d\n",team_data[i].win,team_data[i].draw,team_data[i].lose);
                            printf("Goals Scored/Goals Against: %d/%d\n",team_data[i].goalScored,team_data[i].goalAgainst);
                            break;
                        }
                        if(i == 19) printf("Error: There is no team called \"%s\".\n",team_search);
                    }

                    break;
            }
        }

    }while(choice != 8);

    fclose(record);

    return 0;
}