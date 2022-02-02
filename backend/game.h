#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define HEIGHT  16
#define WIDTH   10

#define GAME_DEFAULT_SPEED_INTERVAL 2000  // milisegundos

char game_public_matrix [HEIGHT][WIDTH]; //Matriz publica del juego
char next_block_public_matrix[12][4];	//Matriz publica de la pieza siguiente

uint8_t row_compleate[4]; 	//Arreglo de las filas siguientes

typedef enum {GAME_IDLE, GAME_RUN, GAME_PAUSE, GAME_QUIT, GAME_LOSE} game_state_t;

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

// Variable en donde se van a guardar las proximas piezas
uint8_t id_next_block[4];
BLOCK_t arr_next_block[4];

// Funciones del juego
void game_init(void);   // inicia la libreria
void game_start(void);  // inicia el juego en blanco
void game_run(void);    // ejecuta el juego
void game_restart(void);// reinicia el juego
void game_quit(void);   // sale del juego



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

// Obtiene todos los datos del juego
game_data_t game_get_data(void);

void delete_pixel (uint8_t row, uint8_t px);

// Elimina y desplaza la fila completa
void delete_row (uint8_t row);

// Setea el callback de la animacion de la eliminacion de una fila
void game_set_delrow_callback (void (*func) (void));



#endif // GAME_H
