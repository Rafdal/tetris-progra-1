#ifndef _TEXT_H_
#define _TEXT_H_

#include <allegro5/allegro.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_image.h> 
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum {TXT_BLANCO, TXT_VERDE, TXT_ROJO} text_color_t;


//Estructura de datos de los bloques de texto
typedef struct 
{
    ALLEGRO_COLOR block_color;   //color del fondo
    ALLEGRO_COLOR font_color;   //color del fondo
    int font_size;              // tamanio de la letra
    char* texto;                //texto a imprimir
    char* tipo_letra;           //fuente del texto
    int posx;                   //posicion en x del texto
    int posy;                   //posicion en y del texto
    int alineacion;             //alineacion a la izquierda, derecha o centro. Si es centro, las coordenadas x e y son del centro del texto
} blocktext_t;

typedef enum{
    ALINEADO_IZQUIERDA,
    CENTRADO,
    ALINEADO_DERECHA
}text_alineacion_t;

blocktext_t* text_init_alleg (text_color_t color, int tamanio_letra, char* palabras, char* nombre_letra, int x, int y, int align);
//DESCRIPCION: Inicializa un Bloque de Texto, reserva memoria dinamica, se ejecuta 1 vez al principio del programa por cada texto que se desea escribir.
// - [color_bloque]: color a asignarle al fondo del bloque, es un rectangulo. 
// - [color_font]: color de la fuente. se le pasa un ALLEGRO_COLOR, usando al_map_rgb(r, g, b). La combinacion de parametros r, g, b de la funcion de alegro devuelve el color deseado
// - [tamanio_letra]: tamanio de la letra, el fondo se ajusta al tamanio de la letra asi que no hace falta cambiarlo
// - [palabras]: el texto que se desea imprimir en pantalla
// - [tipo_letra]: un string que contiene el path para el archivo de la fuente
// - [x]:posicion en x del display, depende del parametro de alineacion que punto se toma. si esta alineado a la derecha, x e y seran de la esquina superior derecha
// - [y]: lo mismo pero posicion en y del display.
// - [align]: parametro de alineacion de texto. Usar el Enum text_alinecion_t
//RETORNA: NULL en caso de que haya un error y si no un puntero a la estructura blocktext_t
// EJEMPLO: char * pathway_fuente= "./frontend/images/arial.ttf"
//blocktext_t * GAME_OVER = text_init_alleg(al_map_rgb(255,0,0), al_map_rgb(0,255,0), 30, "GAME OVER", pathway_fuente, 350, 100, ALINEADO_IZQUIERDA )
//

int text_drawer (blocktext_t* bloque);
//DESCRIPCION: Dibuja en el display un bloque de texto usando los datos de la estructura blocktext_t. 
// - [bloque]: Puntero a la estructura del bloque para obtener los datos
//RETURN: DEVUELVE 0 SI FUE EXITOSA, 1 SI FALLO EL CAMBIO DE FUENTE
//ATENCION: NO FLIPEA EL DISPLAY, ESO SE HACE A MANO

void text_destroy (blocktext_t * bloque);
//DESCRIPCION: destruye toda la memoria reservada para cada estructura
// - [bloque]: puntero a estructura que se quiere destruir
//OBLIGATORIO LLAMARLA AL FINAL DEL PROGRAMA

ALLEGRO_FONT * text_font_pointer_fetcher(void);
//DESCRIPCION: Devuelve el puntero a ALLEGRO_FONT que se esta utilizando actualmente
//No recibe ningun parametro. En caso de que no se haya inicializado la fuente, devuelve NULL

int text_number_drawer (blocktext_t * bloque, uint32_t puntos);
//DESCRIPCION: Dibuja en el display una variable. Por default dibuja un rectangulo negro detras de la variable para borrar lo escrito anteriormente
// - [bloque]: puntero a estructura que se quiere destruir
// - [puntos]: Variable que se desea dibujar en pantalla
//RETURN: DEVUELVE 0 SI ES EXITOSO, 1 SI EL PUNTERO ES NULO
//ATENCION: NO FLIPEA EL DISPLAY, ESO SE HACE A MANO

void font_destroyer (void);
//DESCRIPCION: destruye la fuente cargada. No necesita ningun parametro
//OBLIGATORIO LLAMARLA AL FINAL DEL PROGRAMA

#endif //_TEXT_H_