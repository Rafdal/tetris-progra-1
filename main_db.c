#include <stdio.h>

int main(void){

    
    FILE* file = fopen("tabla.csv", "r");

    if(file != NULL){
        char c;
        while((c = fgetc(file)) != EOF){
            putchar(c);
        }
        putchar('\n');
    }
    fclose(file);

    return 0;
}