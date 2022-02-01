#include <stdio.h>
#include "debug.h"

// Ejemplo

int main(void){
    debug_new_file("debug.txt");

    printf("a\n");
    for (int i = 0; i < 8; i++)
    {
        deb_println("Hola %u, %s\n", i, "jaja");
    }

    return 0;
}