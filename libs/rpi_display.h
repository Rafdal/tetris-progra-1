#ifndef TETRIS_PROGRA_1_RPI_DISPLAY_H
#define TETRIS_PROGRA_1_RPI_DISPLAY_H

//   C O N S T A N T E S
#define RPI_WIDTHT 16
#define RPI_HEIGHT 16

//   P R O T O T I P O S
void init_rpi_display (void); // inicializa el display de la RPI
void run_rpi_display (void); // analiza el valor de la matriz publica y prende el respectivo led sobre la RPI
void rpi_copyToDis (char** fromMat, uint8_t height, uint8_t width, uint8_t x, uint8_t y); // copia la matriz "fromMat" en la matriz de la RPI

#endif //TETRIS_PROGRA_1_RPI_DISPLAY_H