#ifndef _RPI_TEXT_H_
#define _RPI_TEXT_H_

#include "rpi_chars.h"
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "disdrv.h" // RASPBERRY DISPLAY

#define RPI_TEXT_HEIGHT 5
#define RPI_TEXT_SPACING 4

typedef struct {
    uint8_t** data;
    unsigned int width;
} rpi_text_block_t;

rpi_text_block_t rpi_text_create(const char* str);
void rpi_text_destroy(rpi_text_block_t text);

void rpi_text_print(rpi_text_block_t text, uint8_t y, uint8_t x);

void rpi_slide(rpi_text_block_t text, uint8_t y, uint8_t x, uint64_t speed_interval);

#endif // _RPI_TEXT_H_