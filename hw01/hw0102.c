#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define RED "\e[1;91m"
#define BLUE "\e[1;94m"
#define RESET "\e[m"

int main(){

    int32_t diff = 0;
    char origin[1025] = "";
    char ans[1025] = "";
    char needle[129] = "";
    char replace[129] = "";
    char separate[2] = " ";
    char* preptr = NULL;
    char* ptr = NULL;

    printf("Please enter the original text:\n");
    fgets(origin,1025,stdin);

    printf("Keyword:\n");
    fgets(needle,129,stdin);
    needle[strlen(needle) - 1] = 0;
    // for(int32_t i = 0;i < 3;i++) printf("%d ",needle[i]);
    // printf("\n");

    printf("New word:\n");
    fgets(replace,129,stdin);
    replace[strlen(replace) - 1] = 0;
    // for(int32_t i = 0;i < 7;i++) printf("%d ",replace[i]);
    // printf("\n");

    strncpy(ans,origin,1024);

    diff = strlen(replace) - strlen(needle);

    preptr = ans;

    do{
        ptr = strstr(preptr,needle);

        if(ptr != NULL){
            if(diff > 0){
                for(int32_t i = strlen(ans) - 1;i >= ptr - ans;i--) ans[i + diff] = ans[i];
            }else if(diff < 0){
                for(int32_t i = ptr - ans + strlen(replace);i < strlen(ans) + diff;i++){
                    ans[i] = ans[i - diff];
                    if(i == strlen(ans) + diff - 1) memset(&(ans[i+1]),0,-diff);
                } 
            }
            strncpy(ptr,replace,strlen(replace));
            ans[strlen(ans)] = 0;
            preptr = ptr + strlen(replace);
        }

    }while(ptr != NULL);

    printf("Before:\n");
    for(int32_t i  = 0;i < strlen(origin);i++){
        if(strncmp(origin + i,needle,strlen(needle)) == 0){
            printf(BLUE"%.*s"RESET,(int32_t)strlen(needle),origin + i);
            i += strlen(needle) - 1;
        }
        else printf("%c",origin[i]);
    }
    

    printf("After:\n");
    for(int32_t i  = 0;i < strlen(ans);i++){
        if(strncmp(ans + i,replace,strlen(replace)) == 0){
            printf(RED"%.*s"RESET,(int32_t)strlen(replace),ans + i);
            i += strlen(replace) - 1;
        }
        else printf("%c",ans[i]);
    }

    return 0 ;
}