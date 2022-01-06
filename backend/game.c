#include <stdio.h>
#include <stdbool.h>
#include "game.h"

//   C O N S T A N T E S

#define HEIGHT  16
#define WIDTH   10

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

char matrix[HEIGHT][WIDTH];

uint8_t x_pos;    // coordenadas del centro del bloque
uint8_t y_pos;
uint8_t block_id; // id del bloque
uint8_t rotation; // 0 - 3 (orientacion de la pieza girando en sentido antihorario)

enum {LEFT, RIGHT, R_LEFT, R_RIGHT, DOWN};
uint8_t last_movement;
bool bad_movement = false;

//PRUEBA DE OFFSETS
uint8_t d_left, d_right;


// P R O T O T I P O S    P R I V A D O S

void render(void);
char block(uint8_t x, uint8_t y);
int can_write(uint8_t x, uint8_t y); // devuelve 1 si se puede escribir

// F U N C I O N E S

void clear_matrix(void){
    int i,j;
    for(i=0; i<HEIGHT; i++){
        for(j=0; j<WIDTH; j++){
            matrix[i][j] = (char)0;
        }
    }
}

//Funcion auxiliar que imprime la matriz gral en la terminal
void print_matrix(void){
    render();
    printf("Rotation: %u\nx,y: %u, %u\n", rotation, x_pos, y_pos);
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
        putchar('\n');
    }
    for (int i = 0; i < 32; i++)
        putchar('_');
    putchar('\n');
}

// Funcion auxiliar para manejar una matriz de 1D mediante sintaxis de 2D
char block(uint8_t x, uint8_t y){
    return blocks[block_id].data[x+y*blocks[block_id].size];
}

// chequea la validez de las coordenadas para la matriz general
int can_write(uint8_t y, uint8_t x){
    if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT){
        return 1;
    }else{
        switch (last_movement) {
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
        }

        printf("Error! se intento escribir matriz[%u][%u]\n", y, x);
        return 0;
    }
}

// Actualiza la matriz con los datos de coordenadas del bloque
void render(void){
    clear_matrix();
    uint8_t x,y;
    uint8_t size = blocks[block_id].size;
//Analiza el valor de rotacion y gira la matriz del bloque en sentido horario para luego incluirla en la matriz general
    switch (rotation)
    {
        case 0: // Sin rotacion
            for(y=0; y<blocks[block_id].size && !bad_movement; y++){
                for(x=0; x<blocks[block_id].size && !bad_movement; x++){
                    int i,j; // coordenadas de la matriz general
                    i = y_pos+y-size/2;
                    j = x_pos+x-size/2;

                    char val = block(x,y);
                    if(val > 0)  //Evitamos escribir los ceros para evitar que se escriban fuera de la matriz y evitar seg fault
                    {
                        if (can_write(i,j)){
                            matrix[i][j] = val;
                        }
                        else
                            bad_movement = true;
                    }

                }
            }
            break;
        case 1: // Rotacion de 90°
            for(y=0; y<blocks[block_id].size && !bad_movement; y++){
                for(x=0; x<blocks[block_id].size && !bad_movement; x++){
                    int i,j; // coordenadas de la matriz general
                    i = y_pos+y-size/2;
                    j = x_pos+x-size/2;

                    char val = block(y, size-1-x);
                    if(val > 0)  //Evitamos escribir los ceros para evitar que se escriban fuera de la matriz y evitar seg fault
                    {
                        if (can_write(i,j)){
                            matrix[i][j] = val;
                        }
                        else
                            bad_movement = true;
                    }
                }
            }
            break;

        case 2: // Rotacion de 180°
            for(y=0; y<blocks[block_id].size && !bad_movement; y++){
                for(x=0; x<blocks[block_id].size && !bad_movement; x++){
                    int i,j; // coordenadas de la matriz general
                    i = y_pos+y-size/2;
                    j = x_pos+x-size/2;

                    char val = block(size - x -1 , size -y-1);
                    if(val > 0)  //Evitamos escribir los ceros para evitar que se escriban fuera de la matriz y evitar seg fault
                    {
                        if (can_write(i,j)){
                            matrix[i][j] = val;
                        }
                        else
                            bad_movement = true;
                    }
                }
            }
            break;

        case 3: // Rotacion de 270°
            for(y=0; y<blocks[block_id].size && !bad_movement; y++){
                for(x=0; x<blocks[block_id].size && !bad_movement; x++){
                    int i,j; // coordenadas de la matriz general
                    i = y_pos+y-size/2;
                    j = x_pos+x-size/2;

                    char val = block(size-y-1, x);
                    if(val > 0)  //Evitamos escribir los ceros para evitar que se escriban fuera de la matriz y evitar seg fault
                    {
                        if (can_write(i,j)){
                            matrix[i][j] = val;
                        }
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

    if(bad_movement){
        bad_movement = false;
        render(); // Si dio mal, corregimos la posicion
    }
}

// Se ejecuta una sola vez al principio
// Para insertar un bloque arriba por primera vez
void insert_block(uint8_t id){
    // posiciones iniciales
    x_pos = WIDTH/2;
    y_pos = blocks[id].size/2;
    rotation = 0; // 0 es la orientacion por defecto
    block_id = id;

    d_left = x_pos- WIDTH/2;
    d_right = x_pos - WIDTH/2;
}


void descend_block (void){
    printf("descend\n");
    y_pos++;
    last_movement = DOWN;
}

void move_block (int direction){
    printf("move %u\n", direction);
    if (direction)  //Mueve a Derecha
    {
        x_pos++;
        last_movement = RIGHT;
    }else //Mueve a izquierda
    {
        x_pos--;
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
        if (rotation == 3){
            rotation = 0;
        }
        else
            rotation++;
        last_movement = R_RIGHT;
    }
    else    //Sentido anti-horario
    {
        if (rotation == 0){
            rotation = 3;
        }
        else
            rotation--;
        last_movement = R_LEFT;
    }
}