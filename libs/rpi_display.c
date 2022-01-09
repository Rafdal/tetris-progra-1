#include <stdio.h>
#include "rpi_display.h"
#include "disdrv.h"

char matrix[RPI_HEIGHT][RPI_WIDTHT];

// Funcion que inicializa el display de la RPI
void init_rpi_display (void)
{
	disp_init(); //Inicializa el display
	disp_clear(); //Borra el buffer
	int y,x;
	for (y = 0 ; y < RPI_HEIGHT ; .y++)
	{
		for (x = 0 ; x < RPI_WIDTH; x++)
		{
			matrix[y][x] = (char)0;
		}
	}
}


void rpi_copyToDis (char** fromMat, uint8_t height, uint8_t width, uint8_t x, uint8_t y)
{
	if (height+y <= RPI_HEIGHT && width+x <= RPI_WIDTH) //Analiza si es posible copiar la matriz dada en la de la RPI
	{
		int i, j;
		for( i = y; i < height ; i++)
		{
			for (j = x; j < width ; j++)
			{
				matrix[i][j]= fromMat[i][j];
			}
		}
	} else
		printf("ERROR: Exceden los tamaños de la matriz sobre la RPI\n")
}


void run_rpi_display (void)
{
	dcoord_t initPoint = {}; //Crea un punto inicial
	int y, x, out;{
		for (initPoint.x = DISP_MIN ; initPoint.x < RPI_WIDTH; initPoint.x++)
		{
			y = initPoint.y;
			x = initPoint.x;

			out = matrix[y][x];
			if (out != 0) // Si existe valor en la matriz diferente a cero, prendo el LED, de caso contrario se apaga
			{
				disp_write(initPoint, D_ON);
			}
			else
				disp_write(initPoint, D_OFF);
		}
	}
	disp_update();
}
