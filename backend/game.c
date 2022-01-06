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

// V A R I A B L E S

char matrix[HEIGHT][WIDTH]; // Privada
char game_matrix[HEIGHT][WIDTH]; // Publica
block_data_t block_data;

// datos del bloque (coordenadas x,y, rotacion, etc del centro del bloque)

// Variables para el manejo de colisiones
bool colision = false; // true si hubo colision (con piso o pieza)

// Variables para la correccion de la posicion de la pieza (se usa para los limites de la matriz)
enum {LEFT, RIGHT, R_LEFT, R_RIGHT, DOWN};
uint8_t last_movement; // Ultimo movimiento efectuado (se usa para FSM de correccion de posicion)
bool bad_movement = false; // true si hay que corregir el movimiento

// P R O T O T I P O S    P R I V A D O S
void render(void); // Renderiza el bloque en la matriz
char block(uint8_t x, uint8_t y); // Accede a los datos del bloque con coordenadas cartesianas
int can_write(uint8_t x, uint8_t y); // devuelve 1 si se puede escribir, si no se puede, corrige la posicion del bloque

// F U N C I O N E S

// Inicia el juego con las matrices en blanco
void init_game(void){
    clear_matrix();
    block_data.id = 0; // ningun bloque
    int i,j;
    for(i=0; i<HEIGHT; i++){
        for(j=0; j<WIDTH; j++){
            game_matrix[i][j] = (char)0;
        }
    }
}

// Borra la matriz del bloque
void clear_matrix(void){
    int i,j;
    for(i=0; i<HEIGHT; i++){
        for(j=0; j<WIDTH; j++){
            matrix[i][j] = (char)0;
        }
    }
}


// Funcion AUXILIAR que imprime la matriz gral en la terminal
void print_matrix(void){
    printf("Rotation: %u\nx,y: %u, %u\n", block_data.rot, block_data.x, block_data.y);
    for (int i = 0; i < 32; i++)        //34 es lo que queda bien jajaj
        putchar('_');
    putchar('\n');
    for(int y=0; y<HEIGHT; y++)
    {
        putchar('|');
        for(int x=0; x<WIDTH; x++)
        {
            if(matrix[y][x] == 0)
                putchar('-');
            else
                putchar('0'+matrix[y][x]); // La data

            printf("  ");
        }
        putchar('|');
        putchar('\t');
        putchar('\t');

        putchar('|');
        for(int x=0; x<WIDTH; x++)
        {
            if(game_matrix[y][x] == 0)
                putchar('-');
            else
                putchar('0'+game_matrix[y][x]); // La data

            printf("  ");
        }
        putchar('|');
        putchar('\n');
    }
    for (int i = 0; i < 32; i++)
        putchar('_');
    putchar('\n');
}

// Funcion auxiliar para manejar un arreglo unidimensional (de una matriz) con coordenadas cartesianas
char block(uint8_t x, uint8_t y){
    return blocks[block_data.id].data[x+y*blocks[block_data.id].size];
}

// Funcion que devuelve un ID de un bloque de manera aleatoria
uint8_t next_block (void)
{
    srand(time(NULL));
    return rand() % 7 + 1;
}


// Chequea la validez de las coordenadas para la matriz general. Si da error, corrige las coordenadas y devuelve cero
int can_write(uint8_t y, uint8_t x){
    if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT){
        if(game_matrix[y][x] > 0)
		{
            printf("Colision?\n");

			colision = true;
			block_data.y--;
			return 0;
		}
        return 1;
    }else{
        switch (last_movement) { // Realizamos el movimiento contrario para corregir la posicion de la pieza
            case RIGHT:
                move_block(0); // La movemos a la izquierda
                break;

            case LEFT:
                move_block(1); // La movemos a la derecha
                break;

            case R_RIGHT:
                rotate_block(0);
                break;

            case R_LEFT:
                rotate_block(1);
                break;

            case DOWN: // Choque con el piso
                colision = true;

                block_data.y--;
                break;
        }

        printf("Error! se intento escribir matriz[%u][%u]\n", y, x);
        return 0;
    }
}

void run_game(void){
    render();
    if(bad_movement){
        bad_movement = false;
        render(); // Si dio mal, corregimos la posicion
    }
    if(colision){
        int i,j;
        for(i=0; i<HEIGHT; i++){
            for(j=0; j<WIDTH; j++){
                if(matrix[i][j] > 0)
                    game_matrix[i][j] = matrix[i][j];
            }
        }
        insert_block(0); // Borramos el bloque
        clear_matrix();
        colision = false;
    }
}

// Actualiza la matriz con los datos de coordenadas del bloque
void render(void){
    clear_matrix();
    uint8_t x,y;
    uint8_t size = blocks[block_data.id].size;
//Analiza el valor de rotacion y gira la matriz del bloque en sentido horario para luego incluirla en la matriz general
    switch (block_data.rot)
    {
        case 0: // Sin rotacion
            for(y=0; y<blocks[block_data.id].size && !bad_movement; y++){
                for(x=0; x<blocks[block_data.id].size && !bad_movement; x++){
                    int i,j; // coordenadas de la matriz general
                    i = block_data.y+y-size/2;
                    j = block_data.x+x-size/2;

                    char val = block(x,y);
                    if(val > 0)  //Evitamos escribir los ceros para evitar que se escriban fuera de la matriz y evitar seg fault
                    {
                        if (can_write(i,j))
                            matrix[i][j] = val;
                        else
                            bad_movement = true;
                    }

                }
            }
            break;
        case 1: // Rotacion de 90°
            for(y=0; y<blocks[block_data.id].size && !bad_movement; y++){
                for(x=0; x<blocks[block_data.id].size && !bad_movement; x++){
                    int i,j; // coordenadas de la matriz general
                    i = block_data.y+y-size/2;
                    j = block_data.x+x-size/2;

                    char val = block(y, size-1-x);
                    if(val > 0)  //Evitamos escribir los ceros para evitar que se escriban fuera de la matriz y evitar seg fault
                    {
                        if (can_write(i,j))
                            matrix[i][j] = val;
                        else
                            bad_movement = true;
                    }
                }
            }
            break;

        case 2: // Rotacion de 180°
            for(y=0; y<blocks[block_data.id].size && !bad_movement; y++){
                for(x=0; x<blocks[block_data.id].size && !bad_movement; x++){
                    int i,j; // coordenadas de la matriz general
                    i = block_data.y+y-size/2;
                    j = block_data.x+x-size/2;

                    char val = block(size - x -1 , size -y-1);
                    if(val > 0)  //Evitamos escribir los ceros para evitar que se escriban fuera de la matriz y evitar seg fault
                    {
                        if (can_write(i,j))
                            matrix[i][j] = val;
                        else
                            bad_movement = true;
                    }
                }
            }
            break;

        case 3: // Rotacion de 270°
            for(y=0; y<blocks[block_data.id].size && !bad_movement; y++){
                for(x=0; x<blocks[block_data.id].size && !bad_movement; x++){
                    int i,j; // coordenadas de la matriz general
                    i = block_data.y+y-size/2;
                    j = block_data.x+x-size/2;

                    char val = block(size-y-1, x);
                    if(val > 0)  //Evitamos escribir los ceros para evitar que se escriban fuera de la matriz y evitar seg fault
                    {
                        if (can_write(i,j))
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
void insert_block(uint8_t id){
    // posiciones iniciales (arriba al centro)
    block_data.x = WIDTH/2;
    block_data.y = blocks[id].size/2;
    block_data.rot = 0; // 0 es la orientacion por defecto
    block_data.id = id;
}


void descend_block (void){
    printf("descend\n");
    block_data.y++;
    last_movement = DOWN;
}

void move_block (int direction){
    printf("move %u\n", direction);
    if (direction)  //Mueve a Derecha
    {
        block_data.x++;
        last_movement = RIGHT;
    }else //Mueve a izquierda
    {
        block_data.x--;
        last_movement = LEFT;
    }
}
// Maquina de estado para el control del sentido de rotacion de la pieza
// direction = 1  GIRA A LA DERECHA
// direction = 0  GIRA A LA IZQUIERDA
void rotate_block(int direction){
    printf("rotate %u\n", direction);
    if (direction) //Sentido horario
    {
        if (block_data.rot == 3){
            block_data.rot = 0;
        }
        else
            block_data.rot++;
        last_movement = R_RIGHT;
    }
    else    //Sentido anti-horario
    {
        if (block_data.rot == 0){
            block_data.rot = 3;
        }
        else
            block_data.rot--;
        last_movement = R_LEFT;
    }
}