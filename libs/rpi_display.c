#include <stdio.h>
#include <assert.h>
#include "rpi_display.h"
#include "disdrv.h" // RASPBERRY DISPLAY EMULATOR

char matrix[RPI_HEIGHT][RPI_WIDTH];

void rpi_clear_display(void){
	disp_clear(); //Borra el buffer
	int x,y;
	for(y=0; y<RPI_HEIGHT; y++){
		for(x=0; x<RPI_WIDTH; x++){
			matrix[y][x] = (char)0;
		}
	}

}

// Funcion que inicializa el display de la RPI. true = OK | false = ERROR
void rpi_init_display (void)
{
	disp_init(); //Inicializa el display
	disp_clear(); //Borra el buffer
	int x,y;
	for(y=0; y<RPI_HEIGHT; y++){
		for(x=0; x<RPI_WIDTH; x++){
			matrix[y][x] = (char)0;
		}
	}
}

void rpi_set_display(uint8_t y, uint8_t x, char val){
	assert(y<RPI_HEIGHT && x < RPI_WIDTH);
	matrix[y][x] = val;
}


void rpi_copyToDis (matrix_hand_t* pfromMat, uint8_t y_offset, uint8_t x_offset)
{
	assert(pfromMat->height+y_offset < RPI_HEIGHT);
	assert(pfromMat->width+x_offset < RPI_WIDTH);

	int x,y;
	for(y=y_offset; y<RPI_HEIGHT; y++){
		for(x=x_offset; x<RPI_WIDTH; x++){
			matrix[y][x] = mat_get_byte(pfromMat, y, x);
		}
	}
}


void rpi_run_display (void)
{
	dcoord_t initPoint = {}; //Crea un punto inicial
	int y, x, out;
	for(initPoint.y = DISP_MIN ; initPoint.y < RPI_WIDTH; initPoint.y++){
		for (initPoint.x = DISP_MIN ; initPoint.x < RPI_WIDTH; initPoint.x++)
		{
			y = initPoint.y;
			x = initPoint.x;

			out = matrix[y][x];
			if (out > 0) // Si existe valor en la matriz diferente a cero, prendo el LED, de caso contrario se apaga
			{
				disp_write(initPoint, D_ON);
			}
			else
				disp_write(initPoint, D_OFF);
		}
	}
	disp_update();
}
