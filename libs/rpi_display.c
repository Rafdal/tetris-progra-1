#include <stdio.h>
#include <assert.h>
#include "rpi_display.h"
#include "disdrv.h" // RASPBERRY DISPLAY

static char matrix[RPI_HEIGHT][RPI_WIDTH]; //Matriz del display de la RPI

// F U N C I O N E S

// Borra el todos los datos del display
void rpi_clear_display(void){
	disp_clear(); //Borra el buffer
	int x,y;
	for(y=0; y<RPI_HEIGHT; y++){
		for(x=0; x<RPI_WIDTH; x++){
			matrix[y][x] = (char)0;
		}
	}
}

// Inicializa el display de la RPI.
void rpi_init_display (void)
{
	printf("Inicializando display\n");
	disp_init(); //Inicializa el display
	disp_clear(); //Borra el buffer
	rpi_clear_display(); //Limpia el display
}

// Setea en la matriz un valor segun las posiciones dadas
void rpi_set_display(uint8_t y, uint8_t x, char val){
	//assert(y<RPI_HEIGHT && x < RPI_WIDTH);
	if(y<RPI_HEIGHT && x < RPI_WIDTH)
	{
		matrix[y][x] = val;
	}
}

// Recibe un puntero a una matriz y esta la copia en la matriz del juego
void rpi_copyToDis (matrix_hand_t* pfromMat, uint8_t y_offset, uint8_t x_offset)
{
	assert(pfromMat->height+y_offset <= RPI_HEIGHT);
	assert(pfromMat->width+x_offset <= RPI_WIDTH);

	int x,y;
	for(y=0; y<pfromMat->height; y++){
		for(x=0; x<pfromMat->width; x++){
			matrix[y+y_offset][x+x_offset] = mat_get_byte(pfromMat, y, x);
		}
	}
}

//Recibe un puntero a una matriz y esta la imprime en la matriz del juego en una cierta area
void rpi_copyToDis_area (matrix_hand_t *pfromMat, uint8_t x_init, uint8_t x_offset , uint8_t y_init, uint8_t y_offset)
{
	assert(pfromMat->height+y_offset <= RPI_HEIGHT);
	assert(pfromMat->width+x_offset <= RPI_WIDTH);

	int x,y;
	for(y=y_init; y<pfromMat->height; y++){
		for(x=x_init; x<pfromMat->width; x++){
			matrix[y+y_offset][x+x_offset] = mat_get_byte(pfromMat, y, x);
		}
	}
}

// Ejecucion del display
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

// Limpia una cierta area del display
void rpi_clear_area(uint8_t from_y, uint8_t from_x, uint8_t to_y, uint8_t to_x){
	uint8_t x, y;
	for (y = from_y; y < to_y ; y++)
	{
		for (x= from_x; x < to_x ; x++)
		{
			rpi_set_display(y,x, 0);
		}
	}
}