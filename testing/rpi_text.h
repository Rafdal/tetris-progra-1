#ifndef _RPI_TEXT_H_
#define _RPI_TEXT_H_

#include <stdint.h>

#define RPI_TEXT_HEIGHT 5
#define RPI_TEXT_SPACING 4

//Estado del bloque de texto
typedef enum {RPI_TEXT_STATE_SLIDE, RPI_TEXT_STATE_STATIC} rpi_text_state_t;

//Estructura del string de texto
typedef struct {
    uint8_t** data;
    int width;
} rpi_string_block_t;

//Estructura del bloque de texto
typedef struct
{
	rpi_string_block_t string;
	uint8_t str_size;
	uint64_t timestamp;
	int8_t x;
	int8_t x_offset;
	int8_t y;
	int8_t y_offset;
	rpi_text_state_t state;
	uint64_t interval;
}rpi_text_block_t;

// P R O T O T I P O S
rpi_text_block_t* rpi_text_create (uint8_t str_size , int8_t y_offset, int8_t x_offset ); // Crea un bloque de texto

void rpi_text_parse(const char* str, rpi_text_block_t *block); // Le asigna al bloque de texto un string

void rpi_text_destroy(rpi_text_block_t* block); // Destruye un bloque de texto

void rpi_text_print(rpi_text_block_t *block); // Imprime un bloque de texto de manera estatica

void rpi_text_slide(rpi_text_block_t *block, uint64_t speed_interval); //Coloca el estado del bloque en "TEXTO DELIZANTE"

void rpi_text_run(rpi_text_block_t *block); //Analiza el estado del bloque y lo imprime

void set_offset ( rpi_text_block_t* block , int8_t x_offset, int8_t y_offset, int8_t x_slide , int8_t y_slide); //Modifica los offset y las posiciones de impresion del bloque de texto

#endif // _RPI_TEXT_H_