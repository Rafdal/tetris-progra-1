#include "./backend/game.h"
#include <stdio.h>
#include <allegro5/allegro.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_image.h> //NO OLVIDAR INCLUIR ALLEGRO_IMAGE EN LINKER




void deleteline (int numfil);{
    int x;
    for(x=1; x<=WIDTH; x++){
        al_draw_scaled_bitmap(image, (al_get_bitmap_width(image)/8) * val, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image),BLOCKSZ + BLOCKSZ*x, BLOCKSZ*y, BLOCKSZ, BLOCKSZ, 0);
    }  
}