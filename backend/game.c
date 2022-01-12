#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

#define game_level 1

//   C O N S T A N T E S

const char BLOCK_0[] = { // Bloque nulo
    0
};

const char BLOCK_1[] = {
    0, 0, 0, 0,
    1, 1, 1, 1,                                                                                                                                                       // bazinga
    0, 0, 0, 0,
    0, 0, 0, 0,
};

const char BLOCK_2[] = {
    2, 2,
    2, 2,
};

const char BLOCK_3[] = {
    0, 3, 0,
    3, 3, 3,
    0, 0, 0,
};

const char BLOCK_4[] = {
    0, 4, 4,
    4, 4, 0,
    0, 0, 0,
};

const char BLOCK_5[] = {
    5, 5, 0,
    0, 5, 5,
    0, 0, 0,
};

const char BLOCK_6[] = {
    6, 0, 0,
    6, 6, 6,
    0, 0, 0,
};

const char BLOCK_7[] = {
    0, 0, 7,
    7, 7, 7,
    0, 0, 0,
};

/* const char TEST_BLOCK[] = {
        1, 2, 3,
        8, 0, 4,
        7, 6, 5,
}; */

typedef struct{
    const char *data;
    uint8_t size;
}BLOCK_t;

BLOCK_t blocks[] = {
        { BLOCK_0, 1, },
        { BLOCK_1, 4, },
        { BLOCK_2, 2, },
        { BLOCK_3, 3, },
        { BLOCK_4, 3, },
        { BLOCK_5, 3, },
        { BLOCK_6, 3, },
        { BLOCK_7, 3, },
        // { TEST_BLOCK, 3},
};


// V A R I A B L E S   Y   C O N S T A N T E S

static char matrix[HEIGHT][WIDTH]; // Privada
static char static_matrix[HEIGHT][WIDTH]; // Privada
//char game_public_matrix [HEIGHT][WIDTH]; //Publica (definida en el .h)

// datos del juego (coordenadas x,y, rotacion, estado del juego, etc)
static game_data_t game_data;

// Variables para el manejo de colisiones
static bool colision = false; // true si hubo colision (con piso o pieza)

// Variables y constantes para la correccion de la posicion de la pieza (se usa para los limites de la matriz)
enum {LEFT, RIGHT, R_LEFT, R_RIGHT, DOWN};
static uint8_t last_movement; // Ultimo movimiento efectuado (se usa para FSM de correccion de posicion)
static bool bad_movement = false; // true si hay que corregir el movimiento

// P R O T O T I P O S    P R I V A D O S
static void _render(void); // Renderiza el bloque en la matriz
static char _block(uint8_t x, uint8_t y); // Accede a los datos del bloque con coordenadas cartesianas
static int _can_write(uint8_t x, uint8_t y); // devuelve 1 si se puede escribir, si no se puede, corrige la posicion del bloque
static void _undo_movement(void); // deshace el movimiento anterior
static uint8_t _check_row_complete (void); // chequea si una fila se elimino y en caso de serlo devuelve en numero de fila
static void _delete_row (uint8_t row); // elimina y desplaza la fila completa
static void _update_game_public_matrix (void); // actualiza los valores de la matriz publica (la cual contiene la suma de la matriz estatic y dinamica)
static void _clear_matrix(void);
static void _update_score(int streak, char lvl);

// F U N C I O N E S

// Inicia el juego con las matrices en blanco
void game_init(void){
    srand(time(NULL));
    _clear_matrix();
    game_data.id = 0; // ningun bloque
    game_data.state = GAME_RUN;
    game_data.score = 0;
    game_data.speed_interval = GAME_DEFAULT_SPEED_INTERVAL;
    int i,j;
    for(i=0; i<HEIGHT; i++){
        for(j=0; j<WIDTH; j++){
            static_matrix[i][j] = (char)0; // inicio la matriz en cero
            game_public_matrix[i][j] = (char)0;
        }
    }
}

game_data_t game_get_data(void){
    return game_data;
}

// Borra la matriz del bloque
void _clear_matrix(void){
    int i,j;
    for(i=0; i<HEIGHT; i++){
        for(j=0; j<WIDTH; j++){
            matrix[i][j] = (char)0;
        }
    }
}


/* // Funcion AUXILIAR que imprime la matriz gral en la terminal
void print_matrix(void){
    printf("Rotation: %u\nx,y: %u, %u\n", game_data.rot, game_data.x, game_data.y);
    for (int i = 0; i < 32; i++)        //34 es lo que queda bien jajaj
        putchar('_');
    putchar('\n');
    for(int y=0; y<HEIGHT; y++)
    {
        putchar('|');
        for(int x=0; x<WIDTH; x++)
        {
            char out = matrix[y][x]+static_matrix[y][x];
            if(out == 0)
                putchar('-');
            else
                putchar('0'+out); // La data

            printf("  ");
        }
        putchar('|');
        putchar('\n');
    }
    for (int i = 0; i < 32; i++)
        putchar('_');
    putchar('\n');
} */


// Funcion auxiliar para manejar un arreglo unidimensional (de una matriz) con coordenadas cartesianas
char _block(uint8_t x, uint8_t y){
    return blocks[game_data.id].data[x+y*blocks[game_data.id].size];
}

// Funcion que devuelve un ID de un bloque de manera aleatoria
uint8_t game_get_next_block (void)
{
    return rand() % 7 + 1;
}

void _undo_movement(void){
    switch (last_movement) { // Realizamos el movimiento contrario para corregir la posicion de la pieza
        case RIGHT:
            colision = false;
            game_move_horizontal(0); // La movemos a la izquierda
            break;

        case LEFT:
            colision = false;
            game_move_horizontal(1); // La movemos a la derecha
            break;

        case R_RIGHT:
            colision = false;
            game_rotate(0);
            break;

        case R_LEFT:
            colision = false;
            game_rotate(1);
            break;

        case DOWN: // Choque con el piso
            colision = true;

            game_data.y--;
            break;
    }
}


// Chequea la validez de las coordenadas para la matriz general. Si da error, corrige las coordenadas y devuelve cero
int _can_write(uint8_t y, uint8_t x){
    if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT){

        if(static_matrix[y][x] > 0){
            printf("Colision?\n");
            colision = true;
            return 0;
        }

        return 1;
    }else{
        printf("Error! se intento escribir matriz[%u][%u]\n", y, x);
        return 0;
    }
}

void game_run(void){
    _render();
    // TODO: capaz seria mejor poner un switch case
    if(bad_movement){
        bad_movement = false;
        _undo_movement(); // Si dio mal, corregimos la posicion
        _render();
    }
    if(colision){
        int i,j;
        for(i=0; i<HEIGHT; i++){
            for(j=0; j<WIDTH; j++){
                if(matrix[i][j] > 0)
                    static_matrix[i][j] = matrix[i][j]; // Copiamos el bloque
            }
        }
        game_insert_block(0); // Borramos el bloque
        _clear_matrix();

        colision = false;
		int streak = 0;

		while (_check_row_complete())
		{
			int row = _check_row_complete();

			printf("Compleate Row: %d\n", row);
			_delete_row(row);

			streak++;
			_update_score(streak, game_level);
			printf("score is: %d points\n", game_data.score);
		}
   	 }
	_update_game_public_matrix();
}

void _update_game_public_matrix (void)
{
	int i, j;
	for (i=0 ; i < HEIGHT; i ++)
	{
		for (j=0 ; j < WIDTH;  j++)
		{
			game_public_matrix[i][j] = matrix[i][j] + static_matrix[i][j];
		}
	}
}

void _delete_row (uint8_t row)
{
	int i,j;
	for (j= 0; j< WIDTH ; j++)
	{
		static_matrix[row][j] = 0;
	}

	printf("TEST\n");
	for ( i = row ; i > 0 ; i--)
	{
		for( j = 0 ; j < WIDTH; j++)
		{
			static_matrix[i][j] = static_matrix[i-1][j];
			static_matrix[i-1][j] = 0 ;
		}
	}
}

uint8_t _check_row_complete (void)
{
	int i , j;
	for (i = 0 ; i< HEIGHT ; i++)
	{
		for (j= 0 ; (j<WIDTH) && (static_matrix[i][j] != 0); j++)
		{
			//Do nothing
		}
		if (j == WIDTH)	//Si el for recorrio todas las columnas de esa fila y su contenido fue diferente a cero
			return i;	//Devuelvo la fila completa
	}
	return 0; //Devuelve cero si no existe fila completa
}


// Actualiza la matriz con los datos de coordenadas del bloque
void _render(void){
    _clear_matrix();
    uint8_t x,y;
    uint8_t size = blocks[game_data.id].size;
//Analiza el valor de rotacion y gira la matriz del bloque en sentido horario para luego incluirla en la matriz general
    switch (game_data.rot)
    {
        case 0: // Sin rotacion
            for(y=0; y<blocks[game_data.id].size && !bad_movement; y++){
                for(x=0; x<blocks[game_data.id].size && !bad_movement; x++){
                    int i,j; // coordenadas de la matriz general
                    i = game_data.y+y-size/2;
                    j = game_data.x+x-size/2;

                    char val = _block(x,y);
                    if(val > 0)  //Evitamos escribir los ceros para evitar que se escriban fuera de la matriz y evitar seg fault
                    {
                        if (_can_write(i,j))
                            matrix[i][j] = val;
                        else
                            bad_movement = true;
                    }

                }
            }
            break;
        case 1: // Rotacion de 90°
            for(y=0; y<blocks[game_data.id].size && !bad_movement; y++){
                for(x=0; x<blocks[game_data.id].size && !bad_movement; x++){
                    int i,j; // coordenadas de la matriz general
                    i = game_data.y+y-size/2;
                    j = game_data.x+x-size/2;

                    char val = _block(y, size-1-x);
                    if(val > 0)  //Evitamos escribir los ceros para evitar que se escriban fuera de la matriz y evitar seg fault
                    {
                        if (_can_write(i,j))
                            matrix[i][j] = val;
                        else
                            bad_movement = true;
                    }
                }
            }
            break;

        case 2: // Rotacion de 180°
            for(y=0; y<blocks[game_data.id].size && !bad_movement; y++){
                for(x=0; x<blocks[game_data.id].size && !bad_movement; x++){
                    int i,j; // coordenadas de la matriz general
                    i = game_data.y+y-size/2;
                    j = game_data.x+x-size/2;

                    char val = _block(size - x -1 , size -y-1);
                    if(val > 0)  //Evitamos escribir los ceros para evitar que se escriban fuera de la matriz y evitar seg fault
                    {
                        if (_can_write(i,j))
                            matrix[i][j] = val;
                        else
                            bad_movement = true;
                    }
                }
            }
            break;

        case 3: // Rotacion de 270°
            for(y=0; y<blocks[game_data.id].size && !bad_movement; y++){
                for(x=0; x<blocks[game_data.id].size && !bad_movement; x++){
                    int i,j; // coordenadas de la matriz general
                    i = game_data.y+y-size/2;
                    j = game_data.x+x-size/2;

                    char val = _block(size-y-1, x);
                    if(val > 0)  //Evitamos escribir los ceros para evitar que se escriban fuera de la matriz y evitar seg fault
                    {
                        if (_can_write(i,j))
                            matrix[i][j] = val;
                        else
                            bad_movement = true;
                    }
                }
            }
            break;
        default:
            printf("Error rotacion incorrecta\n");
            break;

    }
}

// Se ejecuta una sola vez al principio
// Para insertar un bloque arriba por primera vez
void game_insert_block(uint8_t id){
    // posiciones iniciales (arriba al centro)
    game_data.x = WIDTH/2;
    game_data.y = blocks[id].size/2;
    game_data.rot = 0; // 0 es la orientacion por defecto
    game_data.id = id;
}

// Mueve la pieza hacia abajo
void game_move_down (void){
    printf("descend\n");
    game_data.y++;
    last_movement = DOWN;
}

// Mueve la pieza horizontalmente (1 = DERECHA | 0 = IZQUIERDA)
void game_move_horizontal (int direction){
    printf("move %u\n", direction);
    if (direction)  //Mueve a Derecha
    {
        game_data.x++;
        last_movement = RIGHT;
    }else //Mueve a izquierda
    {
        game_data.x--;
        last_movement = LEFT;
    }
}

// Maquina de estado para el control del sentido de rotacion de la pieza
// direction = 1  GIRA A LA DERECHA
// direction = 0  GIRA A LA IZQUIERDA
void game_rotate(int direction){
    printf("rotate %u\n", direction);
    if (direction) //Sentido horario
    {
        if (game_data.rot == 3){
            game_data.rot = 0;
        }
        else
            game_data.rot++;
        last_movement = R_RIGHT;
    }
    else    //Sentido anti-horario
    {
        if (game_data.rot == 0){
            game_data.rot = 3;
        }
        else
            game_data.rot--;
        last_movement = R_LEFT;
    }
}

// _update_score actualiza el score segun el nivel en el que se encuentra

void _update_score(int streak, char lvl){
	switch (streak) {
		case 1:
			game_data.score += 40*(lvl+1);
			break;
		case 2:
			game_data.score += 100*(lvl+1);
			break;
		case 3:
			game_data.score += 300*(lvl+1);
			break;
		case 4:
			game_data.score += 1200*(lvl+1);
			printf("TETRIS!!");
			break;
	}
    game_data.speed_interval -= game_data.speed_interval/15;
}