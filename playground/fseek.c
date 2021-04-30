#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    FILE	*pFile = NULL;
    char	fileName[256] = {0};
    
    printf( "Please enter the file name: " );
    
    if( fgets( fileName, sizeof( fileName ), stdin ) == NULL )
    {
        printf( "Error!\n" );
        return 0;
    }
    
    // Since fgets will include '\n', we need to remove this character.
    if( fileName[ strlen( fileName ) - 1 ] == '\n' )
    {
        fileName[ strlen( fileName ) - 1 ] = 0;
    }
    
    if( ( pFile = fopen( fileName, "r" ) ) == NULL )
    {
        printf( "File could not be opened!\n" );
        return 0;
    }
    
    fseek( pFile, 0, SEEK_END ); 
      
    // Printing position of pointer 
    printf( "File Size: %ld bytes\n", ftell( pFile ) );
    
    fclose( pFile );    
    
    return 0;
}