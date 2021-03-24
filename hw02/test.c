#include <stdio.h>
#include <stdio.h>

int main(){
    char string[10] = "";
    snprintf(string,9,"1232344");
    snprintf(string,9,"56");

    printf("%s\n",string);
}