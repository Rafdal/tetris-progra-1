#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define GAME_HEIGHT  16
#define GAME_WIDTH   10

// ARREGLOS PUBLICOS
char game_public_matrix [GAME_HEIGHT][GAME_WIDTH];  //Matriz publica del juego
char game_next_block_public_matrix[10][5];	        //Matriz publica de la pieza siguiente
uint8_t game_row_complete[4];                       // Filas completadas

#define GAME_LEVEL1_SCORE 0
#define GAME_LEVEL2_SCORE 500
#define GAME_LEVEL3_SCORE 1000
#define GAME_LEVEL4_SCORE 2000
#define GAME_LEVEL5_SCORE 5000
#define GAME_LEVEL6_SCORE 8000
#define GAME_LEVEL7_SCORE 12000
#define GAME_LEVEL8_SCORE 15000

#define GAME_LEVEL1_SPEED 2000
#define GAME_LEVEL2_SPEED 1200
#define GAME_LEVEL3_SPEED 800
#define GAME_LEVEL4_SPEED 600
#define GAME_LEVEL5_SPEED 400
#define GAME_LEVEL6_SPEED 200
#define GAME_LEVEL7_SPEED 150
#define GAME_LEVEL8_SPEED 100

typedef enum {GAME_RUN, GAME_FAST_RUN, GAME_QUIT, GAME_LOSE} game_state_t;

//Estructura de la pieza
typedef struct{
	const char *data;
	uint8_t size;
}BLOCK_t;

//Estructura de los datos del juego
typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t id;
    uint8_t rot;
    uint32_t score;
    game_state_t state;
    uint64_t speed_interval;
	uint8_t game_level;
}game_data_t;


// Funciones del juego
void game_start(void);  // inicia el juego en blanco
void game_run(void);    // ejecuta el juego
void game_restart(void);// reinicia el juego
void game_quit(void);   // sale del juego

void game_fast_run(void);

void game_debug(void); // DEBUG

// Inserta un nuevo bloque arriba
void game_insert_block(void);

// Desciende las coordenadas del bloque
void game_move_down (void);

// Gira el bloque (1 = horario, 0 = antihorario)
void game_rotate (int direction);

// mueve el bloque en horizontal (1 = derecha, 0 = izquierda)
void game_move_horizontal(int direction);

// Obtiene todos los datos del juego
game_data_t game_get_data(void);

void delete_pixel (uint8_t row, uint8_t px);

// Elimina y desplaza la fila completa
void delete_row (uint8_t row);

// Setea el callback de la animacion de la eliminacion de una fila
void game_set_delrow_callback (void (*func) (void));



#endif // GAME_H
