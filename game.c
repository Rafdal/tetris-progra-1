#include <stdio.h>
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

// P R O T O T I P O S    P R I V A D O S

void render(void);
char block(uint8_t x, uint8_t y);

// F U N C I O N E S

void clear_matrix(void){
    int i,j;
    for(i=0; i<HEIGHT; i++){
        for(j=0; j<WIDTH; j++){
            matrix[i][j] = (char)0;
        }
    }
}

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

// AUX FUNCTION

char block(uint8_t x, uint8_t y){
    return blocks[block_id].data[x+y*blocks[block_id].size];
}

// Actualiza la matriz con los datos de coordenadas del bloque
void render(void){
    clear_matrix();
    uint8_t x,y;
    uint8_t size = blocks[block_id].size;

    switch (rotation)
    {
        case 0:
            for(y=0; y<blocks[block_id].size; y++){
                for(x=0; x<blocks[block_id].size; x++){
                    matrix[y_pos+y-size/2][x_pos+x-size/2] = block(x,y);
                }
            }
            break;
        case 1:
            for(y=0; y<blocks[block_id].size; y++){
                for(x=0; x<blocks[block_id].size; x++){
                    matrix[y_pos+y-size/2][x_pos+x-size/2] = block(y, size-1-x);
                }
            }
            break;

        case 2:
            for(y=0; y<blocks[block_id].size; y++){
                for(x=0; x<blocks[block_id].size; x++){
                    matrix[y_pos+y-size/2][x_pos+x-size/2] = block(size - x -1 , size -y-1);
                }
            }
            break;

        case 3:
            for(y=0; y<blocks[block_id].size; y++){
                for(x=0; x<blocks[block_id].size; x++){
                    matrix[y_pos+y-size/2][x_pos+x-size/2] = block(size-y-1, x);
                }
            }
        default:
            printf("Error rotacion incorrecta\n");
            break;

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
}


void descend_block (void){
    printf("descend\n");
    y_pos++;
}

void move_block (int direction){
    printf("move %u\n", direction);
    if (direction) {
        x_pos++;
    }else{
        x_pos--; // TODO: OJO que se va de la matriz!!!
    }
}

// direction = 1  GIRA A LA DERECHA
// direction = 0  GIRA A LA IZQUIERDA
void rotate_block(int direction){
    printf("rotate %u\n", direction);
    if (direction){
        if (rotation == 3){
            rotation = 0;
        }
        else
            rotation++;
    }
    else{
        if (rotation == 0){
            rotation = 3;
        }
        else
            rotation--;
    }
}