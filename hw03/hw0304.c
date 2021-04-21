#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
    FILE* rgba_bmp = NULL;
    char input_filename[32] = "";
    char output_filename[32] = "";
    uint8_t alpha = 0;

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
    if((rgba_bmp = fopen(output_filename,"wb")) == NULL){
        perror("Error");
        return 0;
    }

    printf("Alpha (0-31): ");
    scanf("%hhd",&alpha);

    sHeader header = {0};

    fread(&header,sizeof(sHeader),1,origin_bmp);

    if(header.bpp != 24){
        printf("Error: The bpp is not 24.\n");
        return 0;
    }

    //BITMAPV3INFOHEADER
    //Addtional 16 bytes for rgba masks

    header.size = 70 + header.width * header.height * 4;
    header.offset = 70;
    header.header_size = 56;
    header.bpp = 32;
    header.compression = 3;
    header.bitmap_size = header.width * header.height * 4;
    uint32_t mask[4] = {0x00FF0000,0x0000FF00,0x000000FF,0xFF000000};   //RGBA
    fwrite(&header,sizeof(sHeader),1,rgba_bmp);
    fwrite(&mask,sizeof(uint32_t),4,rgba_bmp);

    int32_t col_origin = 0;
    int32_t col_modified = 0;
    int32_t alphaIdx = 0;
    uint8_t* row_origin = NULL;
    uint8_t* row_modified = NULL;

    col_origin = header.width * 3 + header.width % 4;
    col_modified = header.width * 4;
    row_origin = (uint8_t*)malloc(sizeof(uint8_t) * col_origin);
    row_modified = (uint8_t*)malloc(sizeof(uint8_t) * col_modified);

    while(!feof(origin_bmp)){
        fread(row_origin,1,col_origin,origin_bmp);
    
        alphaIdx = 0;
        for(int32_t i = 0;i < header.width * 3;i = i + 3){
            row_modified[alphaIdx] = row_origin[i];         //B
            row_modified[alphaIdx + 1] = row_origin[i+1];   //G
            row_modified[alphaIdx + 2] = row_origin[i+2];   //R
            row_modified[alphaIdx + 3] = alpha;             //A
            alphaIdx += 4;
        }

        fwrite(row_modified,1,col_modified,rgba_bmp);
    }

    fclose(origin_bmp);
    fclose(rgba_bmp);

    return 0;
}