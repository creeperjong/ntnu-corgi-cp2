#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FOLDER 1
#define ARCHIVE 0

typedef struct _node {
    char filename[32];
    int32_t layer;
    int32_t type;
    struct _node** sub;
} node;

void print_tree(node* root);

void print_tree(node* root){
    
    if(root->type == FOLDER){

        int32_t idx = 0;

        for(int32_t i = 0;i < root->layer - 1;i++) printf("    ");
        printf("+-- %s/\n",root->filename);
        
        while(root->sub[idx] != NULL){
            print_tree(root->sub[idx]);
            idx++;
        }
    }

    if(root->type == ARCHIVE){
        
        for(int32_t i = 0;i < root->layer - 1;i++) printf("    ");
        printf("+-- %s\n",root->filename);
    }

    return;
}


int main(int argc, char *argv[]){

    if(argc != 2) return 0;

    int32_t fd = 0;
    uint16_t total_num = 0;
    uint32_t CDH_offset = 0;
    uint64_t foundIdx = 0;
    uint64_t filesize = 0;
    uint8_t* pFile = NULL;
    struct stat filestat = {0};

    fd = open(argv[1],O_RDWR);
    if(fd == -1){
        printf("Error: No such file or directory\n");
        return 0;
    }

    fstat(fd,&filestat);
    filesize = filestat.st_size;

    pFile = mmap(0, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    for(uint64_t i = filesize - 22;i >= 0;i--){
        if(strncmp(&pFile[i],"PK\5\6",4) == 0){
            memcpy(&CDH_offset,&pFile[i+16],sizeof(CDH_offset));
            memcpy(&total_num,&pFile[i+10],sizeof(total_num));
            break;
        }
    }

    node root = {0};
    node* now = NULL;
    node* pre = NULL;
    root.layer = 0;
    root.type = FOLDER;
    root.sub = (node**)calloc(total_num,sizeof(node*));

    for(uint64_t i = CDH_offset;i < filesize;i++){
        if(strncmp(&pFile[i],"PK\1\2",4) == 0){
            uint16_t namelen = 0;
            memcpy(&namelen,&pFile[i+28],sizeof(namelen));
            
            char* path = NULL;
            char* token = NULL;
            char* tmp = NULL;
            path = (char*)calloc(namelen + 1,sizeof(char));
            tmp = (char*)calloc(namelen + 1,sizeof(char));

            memcpy(path,&pFile[i+46],namelen);
            memcpy(tmp,path,strlen(path));

            token = strtok(path,"/");
            now = &root;
            while(token != NULL){
                for(int32_t j = 0;j < total_num;j++){
                    if(now->sub[j] == NULL){
                        pre = now;
                        now = (node*)malloc(sizeof(node));
                        now->sub = (node**)calloc(total_num,sizeof(node*));
                        strncpy(now->filename,token,strlen(token)+1);
                        now->layer = pre->layer + 1;
                        now->type = ARCHIVE;
                        pre->sub[j] = now;
                        pre->type = FOLDER;
                        break;
                    }
                    else if(strncmp(now->sub[j]->filename,token,strlen(token)) == 0){
                        now = now->sub[j];
                        break;
                    }
                }

                token = strtok(NULL, "/");
            }

            if(tmp[strlen(tmp) - 1] == '/'){
                now->type = FOLDER;
            }
        }
    }

    printf(".\n");
    print_tree(root.sub[0]);

    munmap(pFile, filesize);
    close(fd);

    return 0;
}