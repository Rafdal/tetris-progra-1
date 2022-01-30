#include "game_effects.h"
#include <allegro5/allegro.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_image.h> //NO OLVIDAR INCLUIR ALLEGRO_IMAGE EN LINKER

void animation_row_compleate(uint8_t* completed_rows)
{
	int i;
    int z;
    float reductor;
    float angulo;
    float decremento= 0.1;
	for( i=0; completed_rows[i] != 0 && i< WIDTH ; i++)
	{

        for(reductor=2.1, angulo=0; reductor>=0; angulo+=(3.1415/8)){
 
            for(z=1; z<=ANCHO; z++){
                al_draw_scaled_bitmap(image, 0, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image), BLOCKSZ*z, BLOCKSZ*(completed_rows[i]), BLOCKSZ, BLOCKSZ, 0);
              //pongo el fondo en negro
                al_draw_tinted_scaled_rotated_bitmap(pieza_blanca,  al_map_rgba_f(1, 1, 1, 1), al_get_bitmap_width(pieza_blanca)/2, al_get_bitmap_height(pieza_blanca)/2, (BLOCKSZ/2 +BLOCKSZ*z), (BLOCKSZ/2 +BLOCKSZ*(completed_rows[i])),reductor, reductor, angulo, 0);
                //se va haciendo mas chia a medida que rota
                al_flip_display();
                al_rest(0.003);
                
            }
            reductor-=decremento;
        }
           for(z=1; z<=ANCHO; z++){
                al_draw_scaled_bitmap(image,(al_get_bitmap_width(image)/8), 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image), BLOCKSZ*z, BLOCKSZ*(completed_rows[i]), BLOCKSZ, BLOCKSZ, 0);
                al_flip_display();
           } //pongo el fondo en negro de nuevo

		delete_row(completed_rows[i]);
		completed_rows[i]= 0;
	}
    al_flush_event_queue(event_queue); // no bajar la pieza despues de la animacion
}