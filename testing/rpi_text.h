#ifndef _RPI_TEXT_H_
#define _RPI_TEXT_H_

#include <stdint.h>

#define RPI_TEXT_HEIGHT 5
#define RPI_TEXT_SPACING 4

typedef enum {RPI_TEXT_STATE_SLIDE, RPI_TEXT_STATE_STATIC} rpi_text_state_t;

typedef struct {
    uint8_t** data;
    int width;
} rpi_string_block_t;

typedef struct
{
	rpi_string_block_t string;
	uint64_t timestamp;
	int8_t x;
	int8_t x_offset;
	int8_t y;
	int8_t y_offset;
	rpi_text_state_t state;
	uint64_t interval;
}rpi_text_block_t;


rpi_text_block_t* rpi_text_create(const char* str, int8_t y_offset, int8_t x_offset);

void rpi_text_destroy(rpi_text_block_t *block);

void rpi_text_print(rpi_text_block_t *block);

void rpi_text_slide(rpi_text_block_t *block, uint64_t speed_interval);

void rpi_text_run(rpi_text_block_t *block);

#endif // _RPI_TEXT_H_