#include <stdio.h>
#include "rpi_display.h"
#include "dispEmu.h" // RASPBERRY DISPLAY EMULATOR

char matrix[RPI_HEIGHT][RPI_WIDTH];

// Funcion que inicializa el display de la RPI
void rpi_init_display (void)
{
	disp_init(); //Inicializa el display
	disp_clear(); //Borra el buffer
	int y,x;
	for (y = 0 ; y < RPI_HEIGHT ; y++)
	{
		for (x = 0 ; x < RPI_WIDTH; x++)
		{
			matrix[y][x] = (char)0;
		}
	}
}

void rpi_set_display(uint8_t x, uint8_t y, uint8_t val){
	matrix[y][x] = (char)val;
}


void rpi_copyToDis (void* pfromMat, uint8_t height, uint8_t width, uint8_t x_offset, uint8_t y_offset)
{
	if (height+y_offset <= RPI_HEIGHT && width+x_offset <= RPI_WIDTH) //Analiza si es posible copiar la matriz dada en la de la RPI
	{
		char (*fromMat)[height] = (char**)(*(char*)pfromMat);
		int y, x;
		for( y = y_offset; y < height ; y++)
		{
			for (x = x_offset; x < width ; x++)
			{
				if(y < RPI_HEIGHT && x < RPI_WIDTH){
					printf("copying (%d,%d)=%u\n", y, x, fromMat[y][x]);
					matrix[y][x] = fromMat[y][x];
				}
				else{
					printf("rpi_display.c > rpi_copyToDis(...)\nERROR: Intento de escritura en matrix(y, x)=(%d, %d)\n", y,x);
				}
			}
		}
	} else {
		printf("ERROR: Exceden los tamaños de la matriz sobre la RPI: (%d,%d)\n", height+y_offset, width+x_offset);
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
