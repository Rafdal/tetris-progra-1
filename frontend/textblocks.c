#include <allegro5/allegro.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_image.h> //NO OLVIDAR INCLUIR ALLEGRO_IMAGE EN LINKER
#include <allegro5/allegro_audio.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_acodec.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdint.h>
#include <stdbool.h>
#include "textblocks.h"


blocktext_t* text_init_alleg ( ALLEGRO_COLOR color_bloque, ALLEGRO_COLOR color_font, int tamanio_letra, char* palabras, char* nombre_letra, int x, int y, int align)
{
blocktext_t * text = (blocktext_t*) calloc(1, sizeof(blocktext_t)); // Asignar memoria a la estructura
    if(text != NULL ){
        // Asignar memoria a los arreglos de informacion de las opciones
        text->texto = (char**) calloc(1, sizeof(char*));
        text->tipo_letra = (char**) calloc(1, sizeof(char*));

        // Si ocurre un error liberar todo
        if(text->texto == NULL ){
            free(text->texto);
            free(text->tipo_letra);
            free(text);
            text = NULL; // (error)
        }

        // Asignar los datos
        text->texto = palabras;
        text->font_size=tamanio_letra;
        text->font_color=color_font;
        text->block_color=color_bloque;
        text->tipo_letra=nombre_letra;
        text->posx= x;
        text->posy=y;
        text->alineacion=align;
    }
    return text;
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

void text_drawer (blocktext_t* bloque){
    
    int ancho_fondo = al_get_text_width(font, bloque->tipo_letra);
    int alto_fondo = al_get_font_line_height(font);

    al_draw_filled_rectangle(bloque->posx, bloque->posy, ((bloque->posx) + ancho_fondo), ((bloque->posy) + alto_fondo), bloque->block_color);
    al_draw_text(font, bloque->font_color, bloque->posx, bloque->posy, bloque->alineacion, bloque->texto);//el parametro es un enum que usa alegro del 0 al 2 
    
}

void text_colour_changer (blocktext_t * bloque, ALLEGRO_COLOR color_fondo, ALLEGRO_COLOR color_letras){
    bloque->block_color= color_fondo;
    bloque->font_color= color_letras;
}//para ocultar el texto simplemente pasarle el mismo color de fondo que de letra


void text_location_changer (blocktext_t * bloque, int newX, int newY){
    bloque->posx=newX;
    bloque->posy=newY;
}

void text_destroy (blocktext_t * bloque){
    if(bloque != NULL ){
            free(bloque->texto);
            free(bloque->tipo_letra);
            free(bloque);
        }

}

void text_font_changer (blocktext_t* bloque,int tamanio, char * font_family){
    bloque->font_size=tamanio;
    bloque->tipo_letra= font_family;
}