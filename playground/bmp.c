#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

struct _sBmpHeader
{
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
}__attribute__((__packed__));

typedef struct _sBmpHeader sBmpHeader;

void print_bmp_header( sBmpHeader *pHeader )
{
    printf( "ID: %c%c\n", pHeader -> bm[0], pHeader -> bm[1] );
    printf( "Size: %u\n", pHeader -> size );
    printf( "Reserve: %u\n", pHeader -> reserve );
    printf( "Offset: %u\n", pHeader -> offset );
    printf( "Header Size: %u\n", pHeader -> header_size );
    printf( "Width: %u\n", pHeader -> width );
    printf( "Height: %u\n", pHeader -> height );
    printf( "Planes: %u\n", pHeader -> planes );
    printf( "Bits Per Pixel: %u\n", pHeader -> bpp );
    printf( "Compression: %u\n", pHeader -> compression );
    printf( "Bitmap Data Size: %u\n", pHeader -> bitmap_size );
    printf( "H-Resolution: %u\n", pHeader -> hres );
    printf( "V-Resolution: %u\n", pHeader -> vres );
    printf( "Used Colors: %u\n", pHeader -> used );
    printf( "Important Colors: %u\n", pHeader -> important );
    
    return;
}


int main()
{
    FILE	*pFile = NULL;
    FILE	*pFile2 = NULL;
    uint8_t headertmp[138] = {0};
    
    if( ( pFile = fopen( "chiwawa.bmp", "r" ) ) == NULL )
    {
        printf( "File could not be opened!\n" );
        return 0;
    }
    
    if( ( pFile2 = fopen( "chiwawa_gray.bmp", "w" ) ) == NULL )
    {
        printf( "File could not be opened!\n" );
        return 0;
    }
    
    sBmpHeader	header;
    fread( headertmp, 138, 1, pFile );
    //print_bmp_header(&header);
    fwrite( headertmp, 138, 1, pFile2 );
    int32_t count_ = 0;
    while( !feof( pFile ) )
    {
        uint8_t	original[1880] = {0};
        uint8_t modified[1880] = {0};
        
        size_t count = fread( original, 1, 1880, pFile );
        
        for( size_t i = 0 ; i < count ; i = i + 3)
        {
            uint8_t gray = (uint8_t)(0.299 * original[i+2]) + (uint8_t)(0.587 * original[i+1]) + (uint8_t)(0.114 * original[i]);
            for(int32_t j = 0;j < 3;j++){
                modified[i + j] = gray;
            }
        }
        
        fwrite( modified, 1, count, pFile2 );
        //if(count_ == 1639680) break;
    }
    
                
    fclose( pFile );
    fclose( pFile2 );
    
    return 0;
}