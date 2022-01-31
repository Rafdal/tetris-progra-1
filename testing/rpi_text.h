#ifndef _RPI_TEXT_H_
#define _RPI_TEXT_H_

#include <stdint.h>

#define RPI_TEXT_HEIGHT 5
#define RPI_TEXT_SPACING 4

typedef struct {
    uint8_t** data;
    int width;
} rpi_string_block_t;

typedef struct
{
	rpi_string_block_t string;
	uint64_t timestamp;
	int8_t x;
	int8_t y;
	uint8_t state; // este
}rpi_text_block_t;


rpi_text_block_t* rpi_text_create(const char* str, int8_t y, int8_t x);

void rpi_text_destroy(rpi_text_block_t *block);

void rpi_text_print(rpi_text_block_t *block, int8_t y, int8_t x);

void rpi_text_slide(rpi_text_block_t *block, uint64_t speed_interval);


#endif // _RPI_TEXT_H_