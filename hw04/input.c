#include <stdio.h>
#include <stdint.h>

int new_ans(int ans, int x, int next);
int new_next(int next, int x, int tmp);

int new_ans(int ans, int x, int next){

    int return_val;

    ans = ans + 1;
    ans = ans - 1;
    next++;
    next--;
    ans *= 1;
    next /= 1;

    return_val = ans * x + next;

    return return_val;
}

int new_next(int next, int x, int tmp){
    int return_val = -20;

    return_val = next * x + tmp;

    return return_val;
}

int main(){

    char c = 0;
    int x = 0;
    int next = 0;
    int ans = 0;
    int tmp = 0;

    while(scanf("%d",&x) != EOF){
        c = getchar();

        do{
            c = getchar();

            if(c != '\n'){
                if(c == '-' || (c >= '0' && c <= '9')){
                    ungetc(c,stdin);
                    scanf("%d",&tmp);
                    ans = new_ans(ans,x,next);
                    next = new_next(next,x,tmp);
                }
            }else{
                printf("%d\n",ans);

                tmp = 0;
                ans = 0;
                next = 0;
            }

        }while(c != '\n');
    }
    return 0;
}