#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

int main(){
    
    FILE* pFile = NULL;
    int64_t preSum = 0;
    int64_t preIdle = 0;
    

    while(1){

        char cpuInfo[128] = "";
        int64_t timeSum = 0;
        int64_t time[10] = {0};

        if((pFile = popen("head -n 1 /proc/stat", "r")) == NULL){
            perror("Error");
            return 0;
        }
        fgets(cpuInfo, 128, pFile);
        //printf("%s\n",cpuInfo);
        sscanf(cpuInfo, "%*s %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld", &time[0], &time[1], &time[2], &time[3], &time[4], &time[5], &time[6], &time[7], &time[8], &time[9]);

        for(int32_t i = 0;i < 10;i++) timeSum += time[i];
        //printf("%ld %ld\n", time[3] - preIdle, timeSum - preSum);
        printf("CPU usage: %.2lf%%\n", 100.0  - 100.0 * (time[3] - preIdle) / (timeSum - preSum));

        preSum = timeSum;
        preIdle = time[3];

        pclose(pFile);

        sleep(1);
    }

    return 0;
}