#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"


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

BLOCK_t blocks[] = {
        { BLOCK_0, 1, },
        { BLOCK_1, 4, },
        { BLOCK_2, 2, },
        { BLOCK_3, 3, },
        { BLOCK_4, 3, },
        { BLOCK_5, 3, },
        { BLOCK_6, 3, },
        { BLOCK_7, 3, },
};


// V A R I A B L E S   Y   C O N S T A N T E S

static char matrix[HEIGHT][WIDTH]; // Privada
static char static_matrix[HEIGHT][WIDTH]; // Privada

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
static void _update_game_public_matrix (void); // actualiza los valores de la matriz publica (la cual contiene la suma de la matriz estatic y dinamica)
static void _clear_matrix(void); // Borra la matriz del bloque
static void _update_score(int streak, uint8_t lvl);
static void _init_arr_next_block (void); //Inicializa el arreglo con los proximos bloques
static void _update_next_block (void); //Actualiza el arreglo con las proximas piezas una vez que la primera pieza de este arreglo ya fue impresa en el juego
static void _update_level (void); //Actualiza el nivel del juego dependiendo del score obtenido
static void _refresh_next_block_mat (void); //Actualiza la matriz de la pieza siguiente

// F U N C I O N E S


// Inicia la libreria
void game_init(void){
    srand(time(NULL));
    game_data.state = GAME_IDLE;
}

// Inicia el Juego
void game_start(void){
    _clear_matrix();
    colision = false;
    bad_movement = false;
    game_data.state = GAME_RUN;
    game_data.id = 0; // Ningun bloque
    game_data.score = 0;
	game_data.game_level = 1; // Inicializa el nivel del juego en 1
    game_data.speed_interval = GAME_DEFAULT_SPEED_INTERVAL;
    int i,j;
    for(i=0; i<HEIGHT; i++){
        for(j=0; j<WIDTH; j++){
            static_matrix[i][j] = (char)0; // inicio la matriz en cero
            game_public_matrix[i][j] = (char)0;
        }
    }
	_init_arr_next_block(); //Inicializa el arreglo con las proximas piezas
}

// Sale del juego
void game_quit(void){
    game_data.state = GAME_QUIT;
    _clear_matrix();
    int i,j;
    for(i=0; i<HEIGHT; i++){
        for(j=0; j<WIDTH; j++){
            static_matrix[i][j] = (char)0; // inicio la matriz en cero
            game_public_matrix[i][j] = (char)0;
        }
    }
}

// Inicia el arreglo y matriz de la pieza siguente
void _init_arr_next_block (void) //Inicializa el arreglo con los proximos bloques
{
	//Antes de cargar la matriz nueva la limpio asi se carga correctamente con nuevas piezas
	int i, j,k;
	for(i=0 ; i < 12 ; i++)
	{
		for (j=0 ; j<4; j++)
		{
			next_block_public_matrix[i][j] = 0;
		}
	}

	for (i = 0 ; i < 4 ; i++)
	{
		id_next_block[i] = game_get_next_block(); //Llena el arreglo con id´s de los proximos bloques
		arr_next_block[i] = blocks[id_next_block[i]];

	}

	for (k = 1 ; k < 4 ; k++)
	{
		char size = arr_next_block[k].size;
		for ( i = 0 ; i < size ; i++)
		{
			for (j = 0; j < size; j++)
			{
				next_block_public_matrix[i+4*(k-1)][j] = arr_next_block[k].data[i*size + j];
			}
		}
	}
}

void _refresh_next_block_mat (void)
{
	int i, j, k;
	//Antes de cargar la matriz nueva la limpio asi se carga correctamente con nuevas piezas
	for(i=0 ; i < 12 ; i++)
	{
		for (j=0 ; j<4; j++)
		{
			next_block_public_matrix[i][j] = 0;
		}
	}

	for (k = 1 ; k < 4 ; k++)
	{
		char size = arr_next_block[k].size;

		for ( i = 0 ; i < size ; i++)
		{
			for (j = 0; j < size; j++)
			{
				next_block_public_matrix[i+4*(k-1)][j] = arr_next_block[k].data[i*size + j];
			}
		}
	}
}

// Actualiza el arreglo con las proximas piezas una vez que la primera pieza de este arreglo ya fue impresa en el juego
void _update_next_block (void)
{
	int i;
	for (i = 0 ; i < 3 ; i++)
	{
		id_next_block[i] = id_next_block[i+1];
		arr_next_block[i] = arr_next_block[i+1];
	}
	id_next_block[3] = game_get_next_block();
	arr_next_block[3] = blocks[id_next_block[3]];

	_refresh_next_block_mat();
}

// Funcion que devuelve la Data del Juego
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
    }else
	{
        if(y>=255)
            game_data.state = GAME_LOSE;

        printf("Error! se intento escribir matriz[%u][%u]\n", y, x);
        return 0;
    }
}

// Ejecucion del Juego
void game_run(void){
    if(game_data.state == GAME_RUN){
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
            uint8_t streak = _check_row_complete();

			printf("STREAK : %d\n", streak );
			_update_score(streak, game_data.game_level);
			_update_level();

			_update_next_block(); //Una vez usado el primer bloque del arreglo, actualiza este arreglo colocando uno nuevo al final de este
   	 }
		_update_game_public_matrix();
		_refresh_next_block_mat();
    }
}

//Actualizo la matriz de general. [Suma la matriz statica +  la matriz del juego]
void _update_game_public_matrix(void)
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

//Borra pixeles de la matriz
void delete_pixel (uint8_t row, uint8_t px)
{
	static_matrix[row][px] = 0;
	matrix[row][px] = 0;
	game_public_matrix[row][px]=0;
}

// Limpio la fila completa y desplazo lo que quedo suspendido
void delete_row (uint8_t row)
{
	int i,j;

	for ( i = row ; i > 0 ; i--)
	{
		for( j = 0 ; j < WIDTH; j++)
		{
			static_matrix[i][j] = static_matrix[i-1][j];
			matrix[i][j] = matrix[i-1][j];
			game_public_matrix[i][j]= game_public_matrix[i-1][j];

            static_matrix[i-1][j] = (char)0;
			matrix[i-1][j] = (char)0;
			game_public_matrix[i-1][j] = (char)0;
		}
	}
}

//Funcion que detecta si una fila se completo, devolviendo la CANTIDAD DE FILAS COMPLETAS
uint8_t _check_row_complete (void)
{
	int i , j, k = 0;
	for (i = 0 ; i< HEIGHT ; i++)
	{
		for (j= 0 ; (j<WIDTH) && (static_matrix[i][j] != 0); j++)
		{
			//Do nothing
		}
		if (j == WIDTH)	//Si el for recorrio todas las columnas de esa fila y su contenido fue diferente a cero
		{
			row_compleate[k] = i;	//Si existe una fila completa, completa un arreglo publico el cual contiene el numero de fila compleata
			k++;
		}
	}
	return k; //Devuelve la cantidad de filas completas, que luego es usado para el score
}


// Actualiza la matriz con los datos de coordenadas del bloque
void _render(void){
    if(game_data.state == GAME_RUN){
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
    if(game_data.id == 0){
        game_insert_block(id_next_block[0]);
    }
    else{
        game_data.y++;
        last_movement = DOWN;
    }
    printf("descend\n");
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
void _update_score(int streak, uint8_t lvl){
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
}

//Actualiza el Nivel a partir del score obtenido
void _update_level (void)
{
	if ( game_data.score >= 0 && game_data.score<= 500 )
	{
		game_data.game_level = 1;
	}
	else if( game_data.score >500 && game_data.score <= 2000)
	{
		game_data.game_level = 2;
		game_data.speed_interval = 1200;
	}
	else if( game_data.score >2000 && game_data.score <= 3000)
	{
		game_data.game_level = 3;
		game_data.speed_interval = 800;
	}
	else if( game_data.score >3000 && game_data.score <= 5000)
	{
		game_data.game_level = 4;
		game_data.speed_interval = 600;
	}
	else if( game_data.score >5000 && game_data.score <= 10000)
	{
		game_data.game_level = 5;
		game_data.speed_interval = 400;
	}
	else if (game_data.score > 10000)
	{
		game_data.game_level = 6;
		game_data.speed_interval = 200;
	}
}