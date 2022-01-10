#ifndef MATRIX_HANDLER_H
#define MATRIX_HANDLER_H

#include <stdint.h>
#include <stdbool.h>

// NOTA: Ejemplo de uso al final

typedef struct {
    uint8_t* mat;       // puntero al arreglo
    uint8_t width;      // ancho de la matriz
    uint8_t height;     // alto de la matriz
} matrix_hand_t;

#define MAT_COPY_FROM_2D_ARRAY(handler_ptr, array, height, width)   {if(sizeof((array)[0][0]) == sizeof(uint8_t) && mat_validate((handler_ptr), (height), (width))){int y,x; for(y=0; y<(height); y++){for(x=0; x<(width); x++){mat_set_byte((handler_ptr), y, x, (array)[y][x]); }}}}

// Inicializar matriz
// devuelve true si se creo correctamente
bool mat_init(matrix_hand_t *mat, uint8_t _height, uint8_t _widht);

// Retorna true si la matriz esta inicializada con por lo menos ciertas dimensiones
bool mat_validate(matrix_hand_t *mat, uint8_t min_height, uint8_t min_width);

// Eliminar una matriz de la memoria
void mat_delete(matrix_hand_t *mat); 

// visualizar matriz por consola
void mat_print(matrix_hand_t *mat);

// Aceder a un byte de la matriz
uint8_t mat_get_byte(matrix_hand_t *mat, uint8_t y, uint8_t x); 

// Aceder a un byte de la matriz
void mat_set_byte(matrix_hand_t *mat, uint8_t y, uint8_t x, uint8_t byte); 

void mat_copyFromTo(matrix_hand_t *fromMat, matrix_hand_t *toMat, uint8_t offsetY, uint8_t offsetX);

#endif // MATRIX_HANDLER_H


/* 
    *************************
    *     EJEMPLO DE USO    *
    *************************

#include <stdio.h>

#include "matrix_handler.h"

uint8_t array[3][3] = {
    {1,2,3},
    {4,5,6},
    {7,8,9},
};

int main(void){
    matrix_hand_t mat1, mat2; // Creo dos matrices

    if(mat_init(&mat1, 3, 3) &&  mat_init(&mat2, 10, 10)){

        MAT_COPY_FROM_2D_ARRAY(&mat1, array, 3, 3); // Copiar un arreglo 2D de 3x3 a una matriz

        printf("Mat 1:\n");
        mat_print(&mat1);

        // Copiar una matriz dentro de otra, con un offset (y,x) = (3,5)
        mat_copyFromTo(&mat1, &mat2, 3, 5);
        
        printf("Mat 2:\n");
        mat_print(&mat2);
    }
    else{
        printf("error, no se pudieron inicializar las matrices\n");
    }

    // Liberar la memoria asignada a las matrices
    mat_delete(&mat1);
    mat_delete(&mat2);
    return 0;
}

 */