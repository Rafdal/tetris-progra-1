#include "rpi_text.h"
#include "rpi_display.h"

// Destruye un bloque de texto (funciona, probado en otro lugar)
void rpi_text_destroy(rpi_text_block_t text){
    int i;
    for(i=0;i<RPI_TEXT_HEIGHT;i++)
        free(text.data[i]);
    free(text.data);
    text.data = NULL;
}

// SIN PROBAR, NO SE SI FUNCIONA
void rpi_text_print(rpi_text_block_t text, uint8_t y, uint8_t x){
    int i,j;
    for(i=0; i<5 && y+i < RPI_HEIGHT; i++){
        for(j=0; j<text.width && x+j < RPI_WIDTH; j++){
            dcoord_t pos;
            pos.y=i;
            pos.x=j;

            rpi_set_display(y+i, x+j, (char)text.data[i][j]);
        }
    }
}
/*
void rpi_slide(rpi_text_block_t text, uint8_t y, uint8_t x, uint64_t speed_interval);
{
	int i, j;
	for ( i = y; i < )
}
 */

// Crea un bloque de texto (funciona, probado en otro lugar)
rpi_text_block_t rpi_text_create(const char* str){
    uint8_t size = strlen(str);
    rpi_text_block_t block;

    block.data = (uint8_t**) calloc(RPI_TEXT_HEIGHT, sizeof(uint8_t*));
    if(block.data == NULL) { 
        printf("Error\n");
        return block;
    }

    int i;
    for(i=0; i<RPI_TEXT_HEIGHT; i++){
        block.data[i] = (uint8_t*) calloc(size*RPI_TEXT_SPACING, sizeof(uint8_t));
        if(block.data[i] == NULL){
            for(;i>=0;i--)
                free(block.data[i]);
            free(block.data);
            block.data = NULL;
            printf("Error\n");
            return block;
        }
    }
    block.width = size*RPI_TEXT_SPACING;

    for(i=0; i<size; i++){
        // dcoord_t pos;
        int c = toupper((int)str[i]);

        if(c >= '0' && c <= 'Z'){
            c = c - '0';
            int y,x;
            for(y=0; y<RPI_TEXT_HEIGHT; y++) // y_axis
            {
                for(x=RPI_TEXT_SPACING; x>1; x--) // x_axis
                    block.data[y][i*RPI_TEXT_SPACING-x+RPI_TEXT_SPACING] = (ascii_0_z[c][y] & (0x1 << x)) >> x;
            }
        }
        else if (c == ' '){
            int y,x;
            for(y=0; y<RPI_TEXT_HEIGHT; y++) // y_axis
            {
                for(x=RPI_TEXT_SPACING; x>=0; x--) // x_axis
                {
                    block.data[y][i*RPI_TEXT_SPACING-x+RPI_TEXT_SPACING] = 0;
                }
            }
        }
        else{
            int y,x;
            for(y=0; y<RPI_TEXT_HEIGHT; y++) // y_axis
            {
                for(x=RPI_TEXT_SPACING; x>=0; x--) // x_axis
                {
                    block.data[y][i*RPI_TEXT_SPACING-x+RPI_TEXT_SPACING] = 1; // unknown symbol
                }
            }
        }
    }
    return block;
}