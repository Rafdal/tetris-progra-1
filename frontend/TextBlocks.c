#include <allegro5/allegro.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_image.h> //NO OLVIDAR INCLUIR ALLEGRO_IMAGE EN LINKER
#include <allegro5/allegro_audio.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_acodec.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_font.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct 
{
    ALLEGRO_COLOR block_color;   //color del fondo
    ALLEGRO_COLOR font_color;   //color del fondo
    uint8_t font_size;              // tamanio de la letra
    char* texto;                //texto a imprimir
} blocktext_t;


blocktext_t* text_init_alleg (char* palabras, ALLEGRO_COLOR color_bloque, ALLEGRO_COLOR color_font, uint8_t tamanio_letra){
blocktext_t * text = (blocktext_t*) calloc(1, sizeof(blocktext_t)); // Asignar memoria a la estructura
    if(text != NULL ){
        // Asignar memoria a los arreglos de informacion de las opciones
        text->texto = (char**) calloc(1, sizeof(char*));

        // Si ocurre un error liberar todo
        if(text->texto == NULL ){
            free(text->texto);
            free(text);
            text = NULL; // (error)
        }

        // Asignar los datos
        text->texto = palabras;
        text->font_size=tamanio_letra;
        text->font_color=color_font;
        text->block_color=color_bloque;
    }
    return text;
}

int text_font_changer (uint8_t tamanio, char * font_family){
    font = al_load_font(font_family, tamanio, 0);
    if (font==NULL)
    {
        return -1;//si hay error retorna -1
    }
    else
    {
    return 0;
    }
}//cambia el tipo de letra y el tamanio

int text_drawer (uint8_t xpos, uint8_t ypos, blocktext_t* bloque)

