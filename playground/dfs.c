#include <stdio.h>
#include <stdint.h>

char str[4] = {'a','b','c','d'};
int32_t visit[4] = {2,1,1,0};
char arragement[5] = {0};

void dfs(int32_t layer);

void dfs(int32_t layer){
    if(layer == 4) printf("%s\n",arragement);
    else{
        for(int32_t i = 0;i < 4;i++){
            if(!visit[i]) continue;
            
            visit[i]--;
            arragement[layer] = str[i];
            dfs(layer+1);
            visit[i]++;
        }
    }
}

int main(){
    dfs(0);
    return 0;
}