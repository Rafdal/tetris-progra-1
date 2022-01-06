#ifndef GAME_H
#define GAME_H

#include <stdint.h>

// Borra la matriz
void clear_matrix(void);

// Imprime la matriz en consola
void print_matrix(void);

// Inserta un nuevo bloque arriba
void insert_block(uint8_t id);

// Desciende las coordenadas del bloque
void descend_block (void);

// Gira el bloque (1 = horario, 0 = antihorario)
void rotate_block (int direction);

// mueve el bloque en horizontal (1 = derecha, 0 = izquierda)
void move_block(int direction);

#endif // GAME_H