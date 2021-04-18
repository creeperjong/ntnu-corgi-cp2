#include <stdio.h>
#include <stdint.h>
#include <math.h>

int main(){
    char* str = "11/08/17,Arsenal,Leicester,4,3,H,2,2,D,M Dean,27,6,10,3,9,12,9,4,0,1,0,0,1.53,4.5,6.5,1.5,4.6,6.75,1.47,4.5,6.5,1.44,4.4,6.5,1.53,4.55,6.85,1.53,4.2,6,1.53,4.5,6.5,41,1.55,1.51,4.6,4.43,6.89,6.44,37,1.65,1.61,2.43,2.32,21,-1,1.91,1.85,2.1,2.02,1.49,4.73,7.25";
    char homeName[32] = "";
    char awayName[32] = "";
    int32_t FTHG = 0;
    int32_t FTAG = 0;
    char FTR = 0;
    int32_t HS = 0;
    int32_t AS = 0;
    int32_t HST = 0;
    int32_t AST = 0;
    int32_t HR = 0;
    int32_t AR = 0;
    sscanf(str,"%*[^,],%[^,],%[^,],%d,%d,%c,%*[^,],%*[^,],%*[^,],%*[^,],%d,%d,%d,%d,%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%d,%d,%*s",homeName,awayName,&FTHG,&FTAG,&FTR,&HS,&AS,&HST,&AST,&HR,&AR);

    printf("%s %s %d %d %c %d %d %d %d %d %d\n",homeName,awayName,FTHG,FTAG,FTR,HS,AS,HST,AST,HR,AR);

    printf("%lf\n",cos(M_PI));

    return 0;
}