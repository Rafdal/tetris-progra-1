#include <allegro5/allegro.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdint.h>
#include <stdbool.h>
#include "textblocks.h"

static ALLEGRO_FONT *font = NULL;

blocktext_t* text_init_alleg (text_color_t color, int tamanio_letra, char* palabras, char* nombre_letra, int x, int y, int align)
{
    blocktext_t * text = (blocktext_t*) calloc(1, sizeof(blocktext_t)); 
    
    if( text != NULL){

        switch (color)  //Dependiendo del parametro recibido, pongo colores predeterminados. Usamos esto ya que no usamos muchos otros colores.
        {
        case TXT_VERDE:
            text->font_color=al_map_rgb(0, 255, 0);
            break;
        case TXT_BLANCO:
            text->font_color=al_map_rgb(255, 255, 255);
            break;
        case TXT_ROJO:
            text->font_color=al_map_rgb(255, 0, 0);
            break;
        
        default:
            text->font_color=al_map_rgb(255, 255, 255);
            break;
        }
        // Asignar los datos
        text->texto = palabras;
        text->font_size=tamanio_letra;
        text->tipo_letra=nombre_letra;
        text->posx=x;
        text->posy=y;
        text->alineacion=align; 
    }
    return text;
}

int text_drawer (blocktext_t* bloque){
    if (bloque == NULL)
    {
       printf("Error, PUNTERO NULO en text_drawer: %u\n", __LINE__);
       return 1;
    }
    font = al_load_font(bloque->tipo_letra, bloque->font_size, 0);//cambia el tipo de letra y el tamanio
    
    if(font != NULL){
        
        al_draw_text(font, bloque->font_color, bloque->posx, bloque->posy, bloque->alineacion, bloque->texto);//el parametro es un enum que usa alegro del 0 al 2 
    }// escribo lo que contiene el bloque
    return 0;
}


void text_destroy (blocktext_t * bloque){
    if(bloque != NULL ){
        free(bloque);
    }
}

ALLEGRO_FONT * text_font_pointer_fetcher(void){
    return font;
}

int text_number_drawer (blocktext_t * bloque, uint32_t puntos){
    if(bloque==NULL){
       printf("Error, PUNTERO NULO en text_drawer: %u\n", __LINE__);
       return 1;
    }
    font = al_load_font(bloque->tipo_letra, bloque->font_size, 0);//cambia el tipo de letra y el tamanio

    int ancho_fondo = al_get_text_width(font, "00000000000"); //fondo de ancho predeterminado para simplificar...
    int alto_fondo = al_get_font_line_height(font); // ...y alto del tamanio de la letra

    al_draw_filled_rectangle(bloque->posx, bloque->posy, ((bloque->posx) + ancho_fondo), ((bloque->posy) + alto_fondo), al_map_rgb(0, 0, 0));
    // dibujo el fondo para borrar lo escrito antes
    al_draw_textf(font, bloque->font_color, bloque->posx, bloque->posy, bloque->alineacion, "%i", puntos);
    //ahora escribo la variable
    return 0;
}

void font_destroyer (void){
    al_destroy_font(font);
}