#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

typedef struct {
    char		bm[2];
    uint32_t	size;
    uint32_t	reserve;
    uint32_t	offset;
    uint32_t	header_size;
    uint32_t	width;
    uint32_t	height;
    uint16_t	planes;
    uint16_t	bpp;
    uint32_t	compression;
    uint32_t	bitmap_size;
    uint32_t	hres;
    uint32_t	vres;
    uint32_t	used;
    uint32_t	important;
}__attribute__ ((packed)) sHeader;

typedef struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
}__attribute__ ((packed)) sPixel;

struct option long_options[] = {
    {"angle", 1, NULL, 'a'},
    {"input", 1, NULL, 'i'},
    {"output", 1, NULL, 'o'},
    {"help", 0, NULL, 'h'},
    {0, 0, 0, 0}
};

int main(int argc, char* argv[]){

    int32_t opt_a = 0;
    int32_t opt_i = 0;
    int32_t opt_o = 0;
    int32_t opt_h = 0;
    char filename_input[32] = "";
    char filename_output[32] = "";
    char degree_char[8] = "";
    double angle = 0;
    int32_t degree = 0;
    FILE* fp_input = NULL;
    FILE* fp_output = NULL;
    char c = 0;
    char** gbg = NULL;

    while((c = getopt_long(argc, argv, "a:i:o:h", long_options, NULL)) != -1){
        switch(c){
            case 'a':
                opt_a = 1;
                strncpy(degree_char, optarg, 8);
                break;
            case 'i':
                opt_i = 1;
                strncpy(filename_input, optarg, 32);
                break;
            case 'o':
                opt_o = 1;
                strncpy(filename_output, optarg, 32);
                break;
            case 'h':
                opt_h = 1;
                break;
        }
    }

    if(opt_h){
        printf("fin02:\n");
        printf("\t-a, --angle: angle for clockwise rotation\n");
        printf("\t-i, --input: input file\n");
        printf("\t-o, --output: output file\n");
        printf("\t-h, --help: This description\n");
        return 0;
    }

    if(!(opt_a && opt_i && opt_o)) return 0;

    degree = (int32_t)strtol(degree_char, gbg, 10);

    angle = degree * (M_PI / 180.0);

    if((fp_input = fopen(filename_input, "rb")) == NULL){
        perror("Error");
        return 0;
    }
    if((fp_output = fopen(filename_output, "wb")) == NULL){
        perror("Error");
        return 0;
    }


    sHeader header = {0};

    fread(&header, sizeof(sHeader), 1, fp_input);
    
    uint32_t width = header.width;
    uint32_t height = header.height;

    header.width = (uint32_t)(width * fabs(cos(angle)) + height * fabs(sin(angle)));
    header.height = (uint32_t)(width * fabs(sin(angle)) + height * fabs(cos(angle)));

    //printf("%d %d\n", header.width, header.height);

    fwrite(&header, sizeof(sHeader), 1, fp_output);

    uint32_t shift_x = 0;
    uint32_t shift_y = 0;

    if(degree <= 90){
        shift_x = 0;
        shift_y = (uint32_t)round((width * sin(degree * (M_PI / 180.0))));
    }
    if(degree > 90 && degree <= 180){
        shift_x = (uint32_t)round((width * sin((degree - 90) * (M_PI / 180.0))));
        shift_y = header.height - 1;
    }
    if(degree > 180 && degree <= 270){
        shift_x = header.width;
        shift_y = (uint32_t)round((height * cos((degree - 180) * (M_PI / 180.0))));
    }
    if(degree > 270 && degree <= 360){
        shift_x = (uint32_t)round((height * cos((degree - 270) * (M_PI / 180.0))));
        shift_y = 0;
    }

    //printf("shift: %d %d\n", shift_x, shift_y);

    uint8_t* origin_padding = NULL;
    uint8_t* modified_padding = NULL;
    sPixel** origin_buf = (sPixel**)malloc(sizeof(sPixel*) * height);
    sPixel** modified_buf = (sPixel**)malloc(sizeof(sPixel*) * header.height);
    
    origin_padding = (uint8_t*)malloc(sizeof(uint8_t) * (width % 4));
    modified_padding = (uint8_t*)malloc(sizeof(uint8_t) * (header.width % 4));
    for(uint32_t i = 0;i < height;i++){
        origin_buf[i] = (sPixel*)malloc(sizeof(sPixel) * width);
        fread(origin_buf[i], sizeof(sPixel), width, fp_input);
        fread(origin_padding, sizeof(uint8_t), width % 4, fp_input);
    }
    for(uint32_t i = 0;i < header.height;i++){
        
        modified_buf[i] = (sPixel*)malloc(sizeof(sPixel) * header.width);

        //white

        memset(modified_buf[i], 255, sizeof(sPixel) * header.width);
    }

    for(uint32_t y = 0;y < height;y++){
        for(uint32_t x = 0;x < width;x++){

            uint32_t new_x = (uint32_t)(x * cos(-angle) - y * sin(-angle)) + shift_x;
            uint32_t new_y = (uint32_t)(x * sin(-angle) + y * cos(-angle)) + shift_y;
            if(new_x < 0) new_x = 0;
            if(new_y < 0) new_y = 0;
            if(new_x >= header.width) new_x = header.width - 1;
            if(new_y >= header.height) new_y = header.height - 1;
            //printf("%d %d -> %d %d\n", x, y, new_x, new_y);

            modified_buf[new_y][new_x] = origin_buf[y][x];

        }
    }

    for(uint32_t i = 0;i < header.height;i++){
        fwrite(modified_buf[i], sizeof(sPixel), header.width, fp_output);
        fwrite(modified_padding, sizeof(uint8_t), header.width % 4, fp_output);
    }

    
    for(int32_t i = 0;i < height;i++) free(origin_buf[i]);
    for(int32_t i = 0;i < header.height;i++) free(modified_buf[i]);
    free(origin_buf);
    free(modified_buf);
    free(origin_padding);
    free(modified_padding);
    fclose(fp_output);
    fclose(fp_input);

    return 0;
}