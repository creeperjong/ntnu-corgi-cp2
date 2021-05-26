#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>

typedef struct {
    uint8_t word02_high: 2;
    uint8_t word01: 6;
    uint8_t word03_high: 4;
    uint8_t word02_low: 4;
    uint8_t word04: 6;
    uint8_t word03_low: 2;
}__attribute__ ((packed)) sEnc;

int32_t isZero(sEnc buf);
int32_t isEnd(FILE* pFile);
int32_t isZero(sEnc buf){
    return !buf.word01 && !buf.word02_high && !buf.word02_low && !buf.word03_high && !buf.word03_low && !buf.word04;
}
int32_t isEnd(FILE* pFile){

    char c = 0;

    fread(&c, sizeof(char), 1, pFile);
    if(feof(pFile)){
        fseek(pFile, -1, SEEK_CUR);
        return 1;
    }else{
        fseek(pFile, -1 ,SEEK_CUR);
        return 0;
    }
}

char base64[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

struct option long_option[] = {
    {"enc", 1, NULL, 'e'},
    {"dec", 1, NULL, 'd'},
    {"output", 1, NULL, 'o'},
    {0, 0, 0, 0}
};

int main(int argc, char *argv[]){

    char c = 0;
    int32_t opt_e = 0;
    int32_t opt_d = 0;
    int32_t opt_o = 0;
    FILE* enc_file = NULL;
    FILE* dec_file = NULL;
    FILE* output_file = NULL;
    char enc_filename[32] = "";
    char dec_filename[32] = "";
    char output_filename[32] = "";

    while((c = getopt_long(argc, argv, "e:d:o:", long_option, NULL)) != -1){
        switch(c){
            case 'e':
                opt_e = 1;
                strncpy(enc_filename, optarg, strlen(optarg));
                break;
            case 'd':
                opt_d = 1;
                strncpy(dec_filename, optarg, strlen(optarg));
                break;
            case 'o':
                opt_o = 1;
                strncpy(output_filename, optarg, strlen(optarg));
                break;
        }
    }

    if((opt_e ^ opt_d) && opt_o){
        if(opt_e){
            if((enc_file = fopen(enc_filename, "rb")) == NULL){
                perror("Error");
                return 0;
            }
            if((output_file = fopen(output_filename, "wb")) == NULL){
                perror("Error");
                return 0;
            }

            while(1){

                char word01 = 0;
                char word02 = 0;
                char word03 = 0;
                char word04 = 0;
                sEnc buf = {0};

                fread(&buf, sizeof(buf), 1, enc_file);
                if(feof(enc_file) && isZero(buf)) break;

                word01 = base64[buf.word01];
                word02 = base64[buf.word02_low + buf.word02_high * 16];
                word03 = base64[buf.word03_low + buf.word03_high * 4];
                word04 = base64[buf.word04];

                if(feof(enc_file) && !buf.word04 && !buf.word03_low){
                    word04 = '=';
                    if(!buf.word03_high && !buf.word02_low) word03 = '=';
                }

                fwrite(&word01, sizeof(char), 1, output_file);
                fwrite(&word02, sizeof(char), 1, output_file);
                fwrite(&word03, sizeof(char), 1, output_file);
                fwrite(&word04, sizeof(char), 1, output_file);

            }

            fclose(enc_file);
            fclose(output_file);
        }

        if(opt_d){
            if((dec_file = fopen(dec_filename, "rb")) == NULL){
                perror("Error");
                return 0;
            }
            if((output_file = fopen(output_filename, "wb")) == NULL){
                perror("Error");
                return 0;
            }

            while(1){

                char word[4] = "";
                sEnc buf = {0};
                uint8_t output[3] = {0};

                fread(&word, sizeof(char), 4, dec_file);
                if(feof(dec_file)) break;

                for(int32_t i = 0;i < 4;i++){

                    if(word[i] == '='){
                        word[i] = 0;
                        continue;
                    }

                    for(int32_t j = 0;j < 64;j++){
                        if(word[i] == base64[j]){
                            word[i] = j;
                            break;
                        }
                    }

                }

                buf.word01 = word[0];
                buf.word02_high = word[1] / 16;
                buf.word02_low = word[1] % 16;
                buf.word03_high = word[2] / 4;
                buf.word03_low = word[2] % 4;
                buf.word04 = word[3];

                memcpy(output, &buf, sizeof(buf));

                for(int32_t i = 0;i < 3;i++){

                    if((i == 1 || i == 2) && output[i] == 0 && isEnd(dec_file)) break;
                    fwrite(&output[i], sizeof(uint8_t), 1, output_file);

                }

            }

            fclose(dec_file);
            fclose(output_file);
        }
    }


    return 0;
}