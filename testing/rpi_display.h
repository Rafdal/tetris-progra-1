#ifndef TETRIS_PROGRA_1_RPI_DISPLAY_H
#define TETRIS_PROGRA_1_RPI_DISPLAY_H

//   C O N S T A N T E S
#define RPI_WIDTH 16
#define RPI_HEIGHT 16

#include <stdint.h>
#include <stdbool.h>
#include "matrix_handler.h"

//   P R O T O T I P O S

void rpi_set_display(uint8_t y, uint8_t x, char val);
void rpi_clear_display(void);

// TODO | HACER
#warning HACER ESTA FUNCION
void rpi_clear_area(uint8_t from_y, uint8_t from_x, uint8_t to_y, uint8_t to_x); // borra el area seleccionada

void rpi_init_display (void); // inicializa el display de la RPI, retorna true si fue exitoso
void rpi_run_display (void); // analiza el valor de la matriz publica y prende el respectivo led sobre la RPI
void rpi_copyToDis (matrix_hand_t* pfromMat, uint8_t y_offset, uint8_t x_offset); // copia la matriz "fromMat" en la matriz de la RPI

#endif //TETRIS_PROGRA_1_RPI_DISPLAY_H
