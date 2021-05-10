#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

struct option long_options[] = {
    {"size", 1, NULL, 'z'},
    {"help", 0, NULL, 'h'},
    {0, 0, 0, 0}
};

int main(int argc, char *argv[]){

    int32_t c = 0;
    int32_t ridx = 0;
    int32_t opt_s = 0;
    int32_t opt_r = 0;
    int32_t opt_size = 0;
    int32_t opt_help = 0;
    FILE* file_origin = NULL;
    FILE* file_modified = NULL;
    FILE* file_recover = NULL;
    char filename_origin[32] = "";
    char filename_modified[32] = "";
    char filename_recover[32] = "";
    char input_size[32] = "";
     
    while( (c = getopt_long(argc,argv,"s:r:",long_options,NULL)) != -1 ){
        switch(c){
            case 's':
                opt_s = 1;
                strncpy(filename_origin,optarg,32);
                break;
            case 'r':
                opt_r = 1;
                strncpy(filename_recover,optarg,32);
                ridx = optind;
                break;
            case 'z':
                opt_size = 1;
                strncpy(input_size,optarg,32);
                break;
            case 'h':
                opt_help = 1;
                break;
        }
    }
    
    uint8_t* buf = NULL;
    uint32_t num = 0;
    int32_t cnt = 0;
    int64_t filesize = 0;
    uint64_t size = 1000;
    char** garbage = NULL;

    if(opt_size) size = (uint64_t)strtol(input_size,garbage,10);

    if(opt_s){

        if((file_origin = fopen(filename_origin,"rb")) == NULL){
            perror("Error");
            return 0;
        }
        
        fseek(file_origin,0,SEEK_END);
        filesize = ftell(file_origin);
        rewind(file_origin);

        num = (uint32_t)ceil(((double)filesize / size));

        buf = (uint8_t*)malloc(sizeof(uint8_t) * size);
        
        for(uint32_t i = 1;i <= num;i++){

            strncpy(filename_modified,filename_origin,32);
            filename_modified[strlen(filename_modified)] = '.';
            snprintf(&filename_modified[strlen(filename_modified)],31 - strlen(filename_modified),"%u",i);

            file_modified = fopen(filename_modified,"wb");

            fwrite(&i,sizeof(i),1,file_modified);
            fwrite(&size,sizeof(size),1,file_modified);

            cnt = fread(buf,1,size,file_origin);
            fwrite(buf,1,cnt,file_modified);

            printf("%s ",filename_modified);

            fclose(file_modified);

        }
        printf("\n");

        free(buf);
        fclose(file_origin);

    }


    if(opt_r){

        if((file_recover = fopen(filename_recover,"wb")) == NULL){
            perror("Error");
            return 0;
        }

        while(ridx < argc && argv[ridx][0] != '-'){
            
            if((file_origin = fopen(argv[ridx],"rb")) == NULL){
                perror("Error");
                return 0;
            }

            fread(&num,sizeof(num),1,file_origin);
            fread(&size,sizeof(size),1,file_origin);

            buf = (uint8_t*)malloc(sizeof(uint8_t) * size);
            cnt = fread(buf,1,size,file_origin);

            fseek(file_recover,size * (num - 1),SEEK_SET);
            fwrite(buf,1,cnt,file_recover);

            fclose(file_origin);          

            ridx++;
            free(buf);
        }

        fclose(file_recover);
    }

    return 0;
}