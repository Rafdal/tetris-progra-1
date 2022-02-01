#include <stdio.h>

// Ejemplo
#define USAR_DEBUG   // desactiva o activa el uso de la macro DEBUG
#include "debug.h"

int main(void){
    debug_new_file("debug.txt");

    printf("a\n");
    for (int i = 0; i < 8; i++)
    {
        DEBUG("test %u", 3)
    }

    return 0;
}