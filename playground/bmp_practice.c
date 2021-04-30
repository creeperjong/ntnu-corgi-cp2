#include <stdio.h>
#include <stdint.h>

typedef struct {
    char bm[2];
    uint32_t file_size;
    uint32_t reserve;
    uint32_t offset;
    uint32_t header_size;
     int32_t width;
     int32_t height;
    uint16_t plane;
    uint16_t bpp;
    uint32_t compression;
    uint32_t bmp_size;
     int32_t hres;
     int32_t vres;
    uint32_t palette;
    uint32_t important;
}__attribute__((__packed__)) sheader;

void print_header(sheader header);

void print_header(sheader header){
    printf( "ID: %c%c\n", header.bm[0], header.bm[1] );
    printf( "Size: %u\n", header.file_size );
    printf( "Reserve: %u\n", header.reserve );
    printf( "Offset: %u\n", header.offset );
    printf( "Header Size: %u\n", header.header_size );
    printf( "Width: %d\n", header.width );
    printf( "Height: %d\n", header.height );
    printf( "Planes: %u\n", header.plane );
    printf( "Bits Per Pixel: %u\n", header.bpp );
    printf( "Compression: %u\n", header.compression );
    printf( "Bitmap Data Size: %u\n", header.bmp_size );
    printf( "H-Resolution: %d\n", header.hres );
    printf( "V-Resolution: %d\n", header.vres );
    printf( "Used Colors: %u\n", header.palette );
    printf( "Important Colors: %u\n", header.important );
    
    return;
}

int main(){

    FILE* fileR = NULL;
    FILE* fileW = NULL;
    sheader header;
    int32_t count = 0;
    uint8_t tmp = 0;

    if((fileR = fopen("maldives.bmp","r")) == NULL){
        perror("Error");
        return 0;
    }

    if((fileW = fopen("maldives_out.bmp","w")) == NULL){
        perror("Error");
        return 0;
    }

    fread(&header,sizeof(header),1,fileR);
    print_header(header);
    fwrite(&header,sizeof(header),1,fileW);

    while(!feof(fileR)){
        uint8_t original[3840] = {0};
        uint8_t modified[3840] = {0};

        count = fread(original,1,3840,fileR);

        for(int32_t i = 0;i < count;i = i + 3){
            // modified[i] = original[count - 1 - i];
            // if(i % 3 == 2){
            //     tmp = modified[i-2];
            //     modified[i-2] = modified[i];
            //     modified[i] = tmp;
            // }
            uint8_t gray = 0.299 * original[i+2] + 0.587 * original[i+1] + 0.114 * original[i];
            for(int32_t j = 0;j < 3;j++){
                modified[i + j] = gray;
            }
        }

        fwrite(modified,count,1,fileW);
    }

    fclose(fileR);
    fclose(fileW);

    return 0;
}
