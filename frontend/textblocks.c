#include <allegro5/allegro.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdint.h>
#include <stdbool.h>
#include "textblocks.h"

static ALLEGRO_FONT *font= NULL;

blocktext_t* text_init_alleg ( ALLEGRO_COLOR color_bloque, ALLEGRO_COLOR color_font, int tamanio_letra, char* palabras, char* nombre_letra, int x, int y, int align)
{
    blocktext_t * text = (blocktext_t*) calloc(1, sizeof(blocktext_t)); 
    
    // Asignar los datos
    text->texto = palabras;
    text->font_size=tamanio_letra;
    text->font_color=color_font;
    text->block_color=color_bloque;
    text->tipo_letra=nombre_letra;
    text->posx= x;
    text->posy=y;
    text->alineacion=align; 
    
    return text;
}

void text_drawer (blocktext_t* bloque){
    
    int ancho_fondo = al_get_text_width(font, bloque->texto);
    int alto_fondo = al_get_font_line_height(font);

    al_draw_filled_rectangle(bloque->posx, bloque->posy, ((bloque->posx) + ancho_fondo), ((bloque->posy) + alto_fondo), bloque->block_color);
    al_draw_text(font, bloque->font_color, bloque->posx, bloque->posy, bloque->alineacion, bloque->texto);//el parametro es un enum que usa alegro del 0 al 2 
    
}


void text_destroy (blocktext_t * bloque){
    if(bloque != NULL ){
            free(bloque);
        }
}


int text_global_font_changer (blocktext_t * bloque){
    font = al_load_font(bloque->tipo_letra, bloque->font_size, 0);
    if (font==NULL)
    {
       printf("Error al cargar fuente");
       return -1; 
    }
    return 0;
    
}//cambia el tipo de letra y el tamanio

ALLEGRO_FONT * text_font_pointer_fetcher(void){
    return font;
}

void text_number_drawer (blocktext_t * bloque, uint32_t puntos){
    int ancho_fondo = al_get_text_width(font, "00000000000");
    int alto_fondo = al_get_font_line_height(font);

    al_draw_filled_rectangle(bloque->posx, bloque->posy, ((bloque->posx) + ancho_fondo), ((bloque->posy) + alto_fondo), bloque->block_color);
    al_draw_textf(font, bloque->font_color, bloque->posx, bloque->posy, bloque->alineacion, "%i", puntos);

}

void font_destroyer (void){
    al_destroy_font(font);
}