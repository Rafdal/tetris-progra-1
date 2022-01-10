#include <stdio.h>
#include "rpi_display.h"
#include "disdrv.h" // RASPBERRY DISPLAY EMULATOR

// char matrix[RPI_HEIGHT][RPI_WIDTH];

matrix_hand_t matrix;

// Funcion que inicializa el display de la RPI. true = OK | false = ERROR
bool rpi_init_display (void)
{
	disp_init(); //Inicializa el display
	disp_clear(); //Borra el buffer

	return mat_init(&matrix, RPI_HEIGHT, RPI_WIDTH);
}

void rpi_set_display(uint8_t y, uint8_t x, uint8_t val){
	mat_set_byte(&matrix, y, x, val);
}


void rpi_copyToDis (matrix_hand_t fromMat, uint8_t y_offset, uint8_t x_offset)
{
	mat_copyFromTo(&fromMat, &matrix, y_offset, x_offset);
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

			out = mat_get_byte(&matrix, y, x);
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
