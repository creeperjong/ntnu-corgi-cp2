#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

int main(){

    FILE* origin_bmp = NULL;
    FILE* distort_bmp = NULL;
    char input_filename[32] = "";
    char output_filename[32] = "";
    int32_t ang = 0;
    int32_t shift = 0;
    int32_t adjust = 0;
    int32_t pixel_per_offset = 0;
    sHeader header = {0};

    //Open the files

    printf("Please input a BMP file: ");
    fgets(input_filename,32,stdin);
    input_filename[strlen(input_filename) - 1] = 0;
    printf("Please input the output BMP file name: ");
    fgets(output_filename,32,stdin);
    output_filename[strlen(output_filename) - 1] = 0;

    if(strncmp(&input_filename[strlen(input_filename) - 4],".bmp",4) != 0 || strncmp(&output_filename[strlen(output_filename) - 4],".bmp",4) != 0){
        printf("Error: The file name extension is not \".bmp\"\n");
        return 0;
    }
    if((origin_bmp = fopen(input_filename,"rb")) == NULL){
        perror("Error");
        return 0;
    }
    if((distort_bmp = fopen(output_filename,"wb")) == NULL){
        perror("Error");
        return 0;
    }

    printf("Angle (0-90): ");
    scanf("%d",&ang);

    //process special cases (ang = 0 or ang = 90)

    if(ang == 0){
        
        uint8_t* row_modified = NULL;
        int32_t col_modified = 0;
        
        fread(&header,sizeof(sHeader),1,origin_bmp);
        header.width *= 2;
        header.height = 1;
        fwrite(&header,sizeof(sHeader),1,distort_bmp);

        col_modified = header.width * 3 + header.width % 4;
        row_modified = (uint8_t*)malloc(sizeof(uint8_t) * col_modified);
        for(int32_t i = 0;i < col_modified;i++) row_modified[i] = 0;    //all black

        fwrite(row_modified,1,col_modified,distort_bmp);
        return 0;
    }

    if(ang == 90){  //copy the bmp

        fread(&header,sizeof(sHeader),1,origin_bmp);
        fwrite(&header,sizeof(sHeader),1,distort_bmp);

        while(!feof(origin_bmp)){
            uint8_t row_origin[999] = {0};

            int32_t count = fread(row_origin,1,999,origin_bmp);
            fwrite(row_origin,1,count,distort_bmp);
        }

        return 0;
    }

    /* 
        Strategy: construct an coordinate system which (0,0) is at the first pixel
        and (x,y) = (column,row)
        find an equation that passed through (0,0) and the slope is equal to tan(angle)
        Once you find the point which is on the right side of the equation when scanning each row
        you can start to copy the pixels
    */

    double m = 0;
    m = tan(ang * M_PI / 180.0);    //slope

    fread(&header,sizeof(sHeader),1,origin_bmp);
    shift = (int32_t)(header.height * (1.0 / m));
    header.width += shift;
    fwrite(&header,sizeof(sHeader),1,distort_bmp);

    int32_t col_origin = 0;
    int32_t col_modified = 0;
    uint8_t* row_origin = NULL;
    uint8_t* row_modified = NULL;
    
    col_origin = (header.width - shift) * 3 + (header.width - shift) % 4;   //+ mod 4 because it requires multiples of 4 bytes
    col_modified = header.width * 3 + header.width % 4;
    row_origin = (uint8_t*)malloc(sizeof(uint8_t) * col_origin);
    row_modified = (uint8_t*)malloc(sizeof(uint8_t) * col_modified);

    for(int32_t i = 0;i < header.height;i++){
        fread(row_origin,1,col_origin,origin_bmp);
        for(int32_t j = 0;j < col_modified;j++) row_modified[j] = 255;  //filling with white pixel
        for(int32_t j = 0;j < col_modified;j = j + 3){
            if(m * j / 3 - i >= 0.0){
                for(int32_t k = 0;k < col_origin;k++) row_modified[j+k] = row_origin[k];
                fwrite(row_modified,1,col_modified,distort_bmp);
                break;
            }
        }
    }

    fclose(origin_bmp);
    fclose(distort_bmp);

    return 0;
}