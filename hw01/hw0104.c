#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct item{
    char key[2049];
    char value[2049];
    struct item* sub;
    int32_t type;
    int32_t index;  //type 1:doesnt have subset, type 2:have subset,
} element;          //type -1:note as end sign

int32_t element_num(char* now,int32_t arrayLayer);

int32_t element_num(char* now,int32_t arrayLayer){
    int32_t ans = 0;
    int32_t layer = 0;
    int32_t i = 0;

    if(arrayLayer == 0){
        do{
            if(now[i] == '{' || now[i] == '[') layer++;
            else if(now[i] == '}' || now[i] == ']') layer--;
            else if(layer == 1 && now[i] == ',') ans++;

            i++;

        }while(layer);
    }else{
        do{
            if(now[i] == '[') layer++;
            else if(now[i] == ']') layer--;
            else if(layer == 1 && now[i] == ',') ans++;

            i++;

        }while(layer);
    }

    return ans + 1;
}

int main(){

    int32_t choice = 0;
    int32_t layer = 0;
    int32_t buf_idx = 0;
    int32_t isWord = 0;
    int32_t firstKey = 0;
    int32_t firstElement = 0;
    int32_t arrayLayer = 0;
    int32_t setLayer = 0;
    int32_t idxFinder = 0;
    char c = 0;
    char json[2049] = "";
    char key[2049] = "";
    char buf[2049] = "";
    element* head = NULL;
    element* now = NULL;
    element* pre = NULL;

    printf("Please enter the JSON string:\n");
    fgets(json,2049,stdin);

    for(int32_t i = 0;i < strlen(json);i++){
        if(json[i] == '{'){

            if(arrayLayer > 0) setLayer++;

            if(arrayLayer == 0 || setLayer > arrayLayer ){
                createNewSub:
                    pre = now;
                    now = (element*)malloc(sizeof(element) * (element_num(&json[i],arrayLayer) + 1));
                    // printf("%d\n",(element_num(&json[i],arrayLayer) + 1));
                    firstElement = 1;
                    firstKey = 1;

                    if(pre != NULL) (now + element_num(&json[i],arrayLayer))->sub = pre + 1;
                    else (now + element_num(&json[i],arrayLayer))->sub = NULL;

                    (now + element_num(&json[i],arrayLayer))->type = -1;
                    
                    if(head == NULL) head = now;
                    else{
                        pre->sub = now;
                        pre->type = 2;
                    }
            }
            else if(setLayer == arrayLayer && setLayer != 0) firstKey = 1;
        }
        else if(json[i] == '}'){

            if(arrayLayer > 0) setLayer--;

            if(arrayLayer == 0 || (setLayer >= arrayLayer && setLayer != 0)){
                closeNewSub:
                    if(buf[0] != 0){
                        strncpy(now->value,buf,2048);
                        memset(buf,0,sizeof(buf));
                        now->type = 1;
                        buf_idx = 0;
                        now++;
                    }
                    if(now->sub != NULL) now = now->sub;
            }
        }
        else if(json[i] == '['){
            arrayLayer++;
            goto createNewSub;
        }
        else if(json[i] == ']'){
            arrayLayer--;
            goto closeNewSub;
        }
        else if(json[i] == ':'){
            strncpy(now->key,buf,2048);
            if(firstKey){
                if(firstElement){
                    now->index = 0;
                    firstElement = 0;
                }else{
                    now->index = (now - 1)->index + 1;
                }
                firstKey = 0;
            }
            else now->index = (now - 1)->index;
                
            memset(buf,0,sizeof(buf));
            buf_idx = 0;
        }
        else if(json[i] == ',' && buf[0] != 0){
            strncpy(now->value,buf,2048);
            memset(buf,0,sizeof(buf));
            now->type = 1;
            buf_idx = 0;
            now++;
        }
        else if(json[i] == '"') isWord = !isWord;
        else if(isWord || (json[i] >= '0' && json[i] <= '9')) buf[buf_idx++] = json[i];
    }

    now = head;

    // while(now->type != -1 || now->sub != NULL){
    //     if(now->type == 1){
    //         printf("%s %s %d %d\n",now->key,now->value,now->type,now->index);
    //         now++;
    //     }
    //     else{
    //         printf("%s %d %d\n",now->key,now->type,now->index);
    //         now = now->sub;
    //         printf("switch\n");
    //     }
    // }

    do{

        printf("Choice (0:Exit,1:Get) : ");
        scanf("%d",&choice);
        c = getchar();

        if(choice == 0) printf("Bye\n");
        else{
            printf("Key: ");
            fgets(key,2049,stdin);
            key[strlen(key) - 1] = 0;

            now = head;
            for(int32_t i = 0;i < strlen(key) + 1;i++){
                if((key[i] == '.' || key[i] == '[') && buf[0] != 0){

                        while(now->type != -1){
                            if(idxFinder != -1){
                                if(now->index != idxFinder) break;
                            }
                            if(strncmp(buf,now->key,2048) == 0 && now->type == 2){
                                now = now->sub;
                                break;
                            }
                            now++;
                        }
                    
                    if(now->type == -1 || (idxFinder != -1 && now->index != idxFinder && strncmp(buf,now->key,2048) == 0)){
                        printf("Error: The key doesn't exist.\n");
                        break;
                    }
                    memset(buf,0,sizeof(buf));
                    buf_idx = 0;
                    idxFinder = -1;

                }else if(key[i] == ']'){
                    idxFinder = atoi(buf);

                    while(now->index != idxFinder) now++;

                    memset(buf,0,sizeof(buf));
                    buf_idx = 0;
                }
                else if(key[i] == '\0'){

                    while(now->type != -1){
                        if(idxFinder != -1){
                            if(now->index != idxFinder) break;
                        }
                        if(strncmp(now->key,buf,2048) == 0 && now->type == 1){
                            printf("Value: %s\n",now->value);
                            break;
                        }
                        now++;
                    }

                    if(now->type == -1 || (idxFinder != -1 && now->index != idxFinder)){
                        printf("Error: The key doesn't exist.\n");
                        break;
                    }
                    memset(buf,0,sizeof(buf));
                    buf_idx = 0;
                    idxFinder = -1;
                }
                else if(key[i] != '.' && key[i] != '[') buf[buf_idx++] = key[i];

                //printf("%c :%s %s %d %d\n",key[i],now->key,now->value,now->type,now->index);
            }
        }

    }while(choice != 0);

    return 0;
}