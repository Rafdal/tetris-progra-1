#ifndef TETRIS_PROGRA_1_RPI_DISPLAY_H
#define TETRIS_PROGRA_1_RPI_DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include "matrix_handler.h"

//   C O N S T A N T E S
#define RPI_WIDTH 16
#define RPI_HEIGHT 16

//   P R O T O T I P O S
void rpi_clear_display(void); // Borra el todos los datos del display

void rpi_init_display (void); // Inicializa el display de la RPI.

void rpi_set_display(uint8_t y, uint8_t x, char val); // Setea en la matriz un valor segun las posiciones dadas

void rpi_copyToDis (matrix_hand_t* pfromMat, uint8_t y_offset, uint8_t x_offset); // Recibe un puntero a una matriz y esta la copia en la matriz del juego

void rpi_run_display (void); // Ejecucion del display

void rpi_clear_area(uint8_t from_y, uint8_t from_x, uint8_t to_y, uint8_t to_x); // Limpia una cierta area del display

#endif //TETRIS_PROGRA_1_RPI_DISPLAY_H
