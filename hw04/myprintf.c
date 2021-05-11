#include "myprintf.h"
#include <stdlib.h>

int32_t myprintf(const char* format, ...){

    int32_t input_num = 0;    
    uint32_t input_num_u = 0;
    int32_t tmp = 0;
    char output[32] = "";
    char* pStr = NULL;

    va_list args;
    va_start(args,format);

    for(int32_t i  = 0;i < strlen(format);i++){
        
        if(format[i] != '%'){
            if(strncmp(&format[i],"\\n",2) == 0){
                fputc('\n',stdout);
                i++;
                continue;
            }
            else{
                fputc(format[i],stdout);
                continue;
            }
        }

        if(strncmp(&format[i],"%d",2) == 0 || strncmp(&format[i],"%i",2) == 0){
            
            memset(output,0,sizeof(output));

            input_num = va_arg(args,int32_t);
            tmp = input_num;
            input_num = abs(input_num);

            for(int32_t j = 0;j < 32;j++){
                output[j] = input_num % 10 + '0';
                input_num /= 10;

                if(input_num == 0){
                    if(tmp < 0){
                        output[j+1] = '-';
                        output[j+2] = 0;
                    }
                    else output[j+1] = 0;

                    break;
                }
            }
            
            for(int32_t j = strlen(output) - 1;j >= 0;j--) fputc(output[j],stdout);

            i++;
            continue;
        }

        if(strncmp(&format[i],"%x",2) == 0){
            
            memset(output,0,sizeof(output));

            input_num_u = va_arg(args,uint32_t);
            
            for(int32_t j = 0;j < 32;j++){
                output[j] = input_num_u % 16 + '0';
                input_num_u /= 16;

                if(output[j] == '0' + 10) output[j] = 'a';
                if(output[j] == '0' + 11) output[j] = 'b';
                if(output[j] == '0' + 12) output[j] = 'c';
                if(output[j] == '0' + 13) output[j] = 'd';
                if(output[j] == '0' + 14) output[j] = 'e';
                if(output[j] == '0' + 15) output[j] = 'f';

                if(input_num_u == 0){
                    output[j+1] = 0;
                    break;
                }
            }

            for(int32_t j = strlen(output) - 1;j >= 0;j--) fputc(output[j],stdout);

            i++;
            continue;
        }

        if(strncmp(&format[i],"%X",2) == 0){
            
            memset(output,0,sizeof(output));

            input_num_u = va_arg(args,uint32_t);
            
            for(int32_t j = 0;j < 32;j++){
                output[j] = input_num_u % 16 + '0';
                input_num_u /= 16;

                if(output[j] == '0' + 10) output[j] = 'A';
                if(output[j] == '0' + 11) output[j] = 'B';
                if(output[j] == '0' + 12) output[j] = 'C';
                if(output[j] == '0' + 13) output[j] = 'D';
                if(output[j] == '0' + 14) output[j] = 'E';
                if(output[j] == '0' + 15) output[j] = 'F';

                if(input_num_u == 0){
                    output[j+1] = 0;
                    break;
                }
            }

            for(int32_t j = strlen(output) - 1;j >= 0;j--) fputc(output[j],stdout);

            i++;
            continue;
        }

        if(strncmp(&format[i],"%s",2) == 0){
            
            pStr = va_arg(args,char*);

            for(int32_t j = 0;j < strlen(pStr);j++) fputc(pStr[j],stdout);

            i++;
            continue;
        }
    }
    
}