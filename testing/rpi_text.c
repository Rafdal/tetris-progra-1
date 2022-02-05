/*
		LIBRERIA DE MANEJO DE TEXTO
	Usos: Esta libreria sirve para crar bloques de textos y luego poder imprimirlos en el display
 	de la RPI de forma "Estatica" y "Deslizante"

*/
#include "rpi_text.h"
#include "rpi_display.h"
#include "rpi_chars.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "easy_timer.h"

static void __rpi_text_print(rpi_text_block_t *block); // Funcion privada. Print sin cambiar block->state

// Crea un bloque de texto
rpi_text_block_t* rpi_text_reserve (uint8_t str_max_size)
{
    rpi_text_block_t* block = (rpi_text_block_t*) malloc(sizeof(rpi_text_block_t));
    if (block != NULL)
	{
		block->str_max_size = str_max_size;
        block->state = RPI_TEXT_STATE_OFF;
		block->x = 0;
		block->y = 0;
    
        int i;
        for(i=0; i<RPI_TEXT_HEIGHT; i++){
            block->string.data[i] = (uint8_t*) calloc(str_max_size*RPI_TEXT_SPACING, sizeof(uint8_t));
            if(block->string.data[i] == NULL){
                for(;i>=0;i--)
                    free(block->string.data[i]);
                free(block);
                block = NULL;
            }
        }
	}
    return block;
}

// Le asigna al bloque de texto un string
void rpi_text_set(const char* str, rpi_text_block_t *block)
{
    if(block != NULL && str != NULL){
        block->str_size = strlen(str);
        block->string.width = block->str_size*RPI_TEXT_SPACING;

        if (block->str_size > block->str_max_size){
            printf("rpi_text Error: str_size > str_max_size\n");  // No se reservo la memoria suficiente
            block->string.width = 0;
            block->str_size = 0;
            return;
        }

        // block->state = RPI_TEXT_STATE_OFF;
        int i;
        for(i=0; i < block->string.width; i++){
            int c = toupper((int)str[i]); // Solo usamos mayusculas

            if(c >= '0' && c <= 'Z'){ // rango de caracteres rpi_chars.h
                c = c - '0';
                int y,x;
                for(y=0; y<RPI_TEXT_HEIGHT; y++) // y_axis
                {
                    for(x=RPI_TEXT_SPACING; x>1; x--) // x_axis
                    {   // Pasar los 5 bits al arreglo: 01010 -> [0,1,0,1,0] con mascaras
                        block->string.data[y][i*RPI_TEXT_SPACING-x+RPI_TEXT_SPACING] = (ascii_0_z[c][y] & (0x1 << x)) >> x;
                    }
                }
            }
            else if (c == ' '){
                int y,x;
                for(y=0; y<RPI_TEXT_HEIGHT; y++) // y_axis
                {
                    for(x=RPI_TEXT_SPACING; x>=0; x--) // x_axis
                    {
                        block->string.data[y][i*RPI_TEXT_SPACING-x+RPI_TEXT_SPACING] = 0; // Espacio vacio
                    }
                }
            }
            else{
                int y,x;
                for(y=0; y<RPI_TEXT_HEIGHT; y++) // y_axis
                {
                    for(x=RPI_TEXT_SPACING; x>=0; x--) // x_axis
                    {
                        block->string.data[y][i*RPI_TEXT_SPACING-x+RPI_TEXT_SPACING] = 1; // unknown symbol
                    }
                }
            }
        }
    }
}

// Destruye un bloque de texto
void rpi_text_destroy(rpi_text_block_t* block){
    if( block != NULL){
        int i;
        for(i=0;i<RPI_TEXT_HEIGHT && block->string.data[i] != NULL;i++){
            free(block->string.data[i]);
        }
        free(block);
    }
}

void __rpi_text_print(rpi_text_block_t *block){
    if(block != NULL){
        int i,j;
        for(i=0; i<RPI_TEXT_HEIGHT ; i++)
        {
            for(j=0; j<block->string.width; j++)
            {
                if (block->y_offset+block->y+i < RPI_HEIGHT && block->x_offset+block->x+j < RPI_WIDTH )
                {
                    // Si esta dentro del rango del display, seteamos el ledo
                    // Con esto nos despreocupamos de escribir fuera del display.
                    rpi_set_display(block->y_offset+block->y+i, block->x_offset+block->x+j, (char)block->string.data[i][j]);
                }
            }
        }
    }
}

// Imprime un bloque de texto de manera estatica (sin animacion)
void rpi_text_print(rpi_text_block_t *block){
    __rpi_text_print(block);
    block->state=RPI_TEXT_STATE_STATIC;
}

// Coloca el estado del bloque en "TEXTO DELIZANTE"
void rpi_text_slide(rpi_text_block_t *block, uint64_t speed_interval){
    block->interval = speed_interval;
    block->state = RPI_TEXT_STATE_SLIDE;
}

// Funcion para deslizar el bloque (si corresponde)
// Se debe ejecutar en Real Time
void rpi_text_run(rpi_text_block_t *block){
    if(block != NULL){
        if(block->state == RPI_TEXT_STATE_SLIDE){
			if (easytimer_get_millis() - (block->timestamp) >= block->interval)
            {

                __rpi_text_print(block);
                rpi_run_display();

                block->timestamp = easytimer_get_millis();
                if(-(block->x) != block->string.width - 1)
                {
                    (block->x)--;
                } else
                    block->x = RPI_WIDTH;
            }
        }
    }
}

//Modifica los offset y las posiciones de impresion del bloque de texto
void rpi_text_set_offset ( rpi_text_block_t* block , int8_t x_offset, int8_t y_offset, int8_t x , int8_t y)
{
	block->x_offset = x_offset;
	block->y_offset = y_offset;
	block->x = x;
	block->y = y;
}

//Funcion que muestra el texto deslizante solo una vez
void rpi_text_one_slide (rpi_text_block_t* block)
{
	if(block != NULL){
		if(block->state == RPI_TEXT_STATE_SLIDE){
			if (easytimer_get_millis() - (block->timestamp) >= block->interval)
			{
				__rpi_text_print(block);
				rpi_run_display();

				block->timestamp = easytimer_get_millis();
				if(-(block->x) != block->string.width +5)
				{
					(block->x)--;
				} else
					block->state = RPI_TEXT_STATE_STATIC;
			}
		}
		else
			__rpi_text_print(block);
	}
}
