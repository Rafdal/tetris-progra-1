#include <stdio.h>

// Ejemplo
#define USAR_DEBUG   // desactiva o activa el uso de la macro DEBUG
#include "debug.h"

int main(void){
    debug_new_file("debug.txt");

    for (int i = 0; i < 8; i++)
    {
        printf("Esto no se imprime en el archivo\n");
        DEBUG("Esto si, test %u, %s", i, "hola");
    }

    return 0;
}