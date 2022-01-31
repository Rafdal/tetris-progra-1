#include "rpi_text.h"
#include "rpi_display.h"
#include "easy_timer.h"
#include "ctype.h"

// Crea un bloque de texto (funciona, probado en otro lugar)
rpi_text_block_t* rpi_text_create(const char* str, int8_t x, int8_t y)
{
    rpi_text_block_t* block = (rpi_text_block_t*) malloc(sizeof(rpi_text_block_t));
    if (block == NULL)
        return NULL; // error

    uint8_t size = strlen(str);

	block->x= x;
	block->y = y;

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

// Destruye un bloque de texto (funciona, probado en otro lugar)
void rpi_text_destroy(rpi_text_block_t* block){
    int i;
    for(i=0;i<RPI_TEXT_HEIGHT;i++)
        free(block->string.data[i]);
    free(block->string.data);
    free(block);
}

// SIN PROBAR, NO SE SI FUNCIONA
void rpi_text_print(rpi_text_block_t *block, int8_t y, int8_t x){
    if(block != NULL && block->string.data != NULL){
        int i,j;
        for(i=0; i<5 ; i++)
        {
            for(j=0; j<block->string.width; j++)
            {
                if (y+i < RPI_HEIGHT && x+j < RPI_WIDTH )
                rpi_set_display(y+i, x+j, (char)block->string.data[i][j]);
            }
        }
    }
}


void rpi_slide(rpi_text_block_t *block, uint64_t speed_interval)
{
	if(block != NULL && (easytimer_get_millis() - (block->timestamp) >= speed_interval) )
	{

		rpi_text_print(block, block->y, block->x);
		rpi_run_display();

		block->timestamp = easytimer_get_millis();
		if(-(block->x) != block->string.width)
		{
			(block->x)--;
		} else
			block->x = RPI_WIDTH;
	}
}
