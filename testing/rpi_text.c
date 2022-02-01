#include "rpi_text.h"

#include "rpi_display.h"
#include "rpi_chars.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "easy_timer.h"

// Crea un bloque de texto
rpi_text_block_t* rpi_text_create(const char* str, int8_t y_offset, int8_t x_offset)
{
    rpi_text_block_t* block = (rpi_text_block_t*) malloc(sizeof(rpi_text_block_t));
    if (block == NULL)
        return NULL; // error

    uint8_t size = strlen(str);

	block->x_offset= x_offset;
	block->x = 0;

	block->y_offset = y_offset;
	block->y = 0;

    block->string.data = (uint8_t**) calloc(RPI_TEXT_HEIGHT, sizeof(uint8_t*));
    if(block->string.data == NULL) 
        return NULL; // error

    int i;
    for(i=0; i<RPI_TEXT_HEIGHT; i++){
        block->string.data[i] = (uint8_t*) calloc(size*RPI_TEXT_SPACING, sizeof(uint8_t));
        if(block->string.data[i] == NULL){
            for(;i>=0;i--)
                free(block->string.data[i]);
            free(block->string.data);
            free(block);
            return NULL;
        }
    }
    block->string.width = size*RPI_TEXT_SPACING;

    for(i=0; i<size; i++){
        // dcoord_t pos;
        int c = toupper((int)str[i]);

        if(c >= '0' && c <= 'Z'){
            c = c - '0';
            int y,x;
            for(y=0; y<RPI_TEXT_HEIGHT; y++) // y_axis
            {
                for(x=RPI_TEXT_SPACING; x>1; x--) // x_axis
                    block->string.data[y][i*RPI_TEXT_SPACING-x+RPI_TEXT_SPACING] = (ascii_0_z[c][y] & (0x1 << x)) >> x;
            }
        }
        else if (c == ' '){
            int y,x;
            for(y=0; y<RPI_TEXT_HEIGHT; y++) // y_axis
            {
                for(x=RPI_TEXT_SPACING; x>=0; x--) // x_axis
                {
                    block->string.data[y][i*RPI_TEXT_SPACING-x+RPI_TEXT_SPACING] = 0;
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
    return block;
}

// Destruye un bloque de texto
void rpi_text_destroy(rpi_text_block_t* block){
    int i;
    for(i=0;i<RPI_TEXT_HEIGHT;i++)
        free(block->string.data[i]);
    free(block->string.data);
    free(block);
}

// Imprime un bloque de texto de manera estatica
void rpi_text_print(rpi_text_block_t *block){
    if(block->string.data != NULL){
        int i,j;
        for(i=0; i<5 ; i++)
        {
            for(j=0; j<block->string.width; j++)
            {
                if (block->y_offset+block->y+i < RPI_HEIGHT && block->x_offset+block->x+j < RPI_WIDTH )
                rpi_set_display(block->y_offset+block->y+i, block->x_offset+block->x+j, (char)block->string.data[i][j]);
            }
        }
    }
}

void rpi_text_slide(rpi_text_block_t *block, uint64_t speed_interval){
    block->interval = speed_interval;
    block->state = RPI_TEXT_STATE_SLIDE;
}

void rpi_text_run(rpi_text_block_t *block){
    if(block != NULL){
        if(block->state == RPI_TEXT_STATE_SLIDE){
			if (easytimer_get_millis() - (block->timestamp) >= block->interval)
            {

                rpi_text_print(block);
                rpi_run_display();

                block->timestamp = easytimer_get_millis();
                if(-(block->x) != block->string.width)
                {
                    (block->x)--;
                } else
                    block->x = RPI_WIDTH;
            }
        }
		else
			rpi_text_print(block);
    }

}