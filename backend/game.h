#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define HEIGHT  16
#define WIDTH   10

char game_public_matrix [HEIGHT][WIDTH];

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t id;
    uint8_t rot;
}block_data_t;

block_data_t game_block_data;

// Funciones del juego
void game_init(void); // inicia el juego en blanco
void game_run(void); // ejecuta el juego


// Imprime la matriz en consola
// void print_matrix(void);

// Inserta un nuevo bloque arriba
void game_insert_block(uint8_t id);

// Desciende las coordenadas del bloque
void game_move_down (void);

// Gira el bloque (1 = horario, 0 = antihorario)
void game_rotate (int direction);

// mueve el bloque en horizontal (1 = derecha, 0 = izquierda)
void game_move_horizontal(int direction);

//Devuelve un ID de bloque aleatorio
uint8_t game_get_next_block (void);

// Actualiza el score
int game_update_score(int score, int streak, char game_level);


#endif // GAME_H
