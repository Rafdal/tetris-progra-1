#include "./backend/game.h"
#include <stdio.h>
#include <allegro5/allegro.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_image.h> //NO OLVIDAR INCLUIR ALLEGRO_IMAGE EN LINKER


void printer (void){
    char x, y;
    for(x=0; x<WIDTH ; x++)
    {
        for(y=0; y<HEIGHT ; y++)
        {
        al_draw_scaled_bitmap(image, (al_get_bitmap_width(image)/8) * static_matrix[y][x], 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image),BLOCKSZ + BLOCKSZ*x, BLOCKSZ*y, BLOCKSZ, BLOCKSZ, 0);\
        }
    }
}

void deleteline (char numfil);{
    
}