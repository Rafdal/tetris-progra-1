#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "matrix_handler.h"

// Inicializar matriz con ancho y alto
// retorna true si fue exitoso
bool mat_init(matrix_hand_t *mat, uint8_t _height, uint8_t _width){
    if(mat == NULL){
        mat->height = 0;
        mat->width = 0;
        return false; // si el puntero es nulo retorno falso
    }

    // guardo las dimensiones de la matriz
    mat->width = _width;
    mat->height = _height;

    // inicializo el espacio de memoria
    mat->mat = (uint8_t*) calloc (((size_t)_width)*((size_t)_height), sizeof(uint8_t));

    // si hubo error, retorno false
    if(mat->mat == NULL){
        mat->height = 0;
        mat->width = 0;
        return false;
    }
    return true; // si fue exitoso, retorno true
}

// Retorna true si la matriz esta inicializada con por lo menos ciertas dimensiones
bool mat_validate(matrix_hand_t *mat, uint8_t min_height, uint8_t min_width){
    if(mat == NULL)
        printf("mat NULL\n");
    if(mat->mat == NULL)
        printf("mat NULL\n");

    if(mat->height < min_height)
        printf("mat_height: %u < min_height %u\n", mat->height, min_height);

    if(mat->width < min_width)
        printf("mat_width: %u < min_width %u\n", mat->width, min_width);

    return mat != NULL && mat->mat != NULL && mat->height >= min_height && mat->width >= min_width;
}

// Eliminar una matriz de la memoria
void mat_delete(matrix_hand_t *mat){
    if(mat == NULL)
        return;

    mat->width = 0;
    mat->height = 0;
    free(mat->mat);
}

// Aceder a un byte de la matriz
uint8_t mat_get_byte(matrix_hand_t *mat, uint8_t y, uint8_t x){
    assert(mat != NULL && mat->mat != NULL);
    assert(x < mat->width && y < mat->height);

    return mat->mat[x+y*mat->width];
}

// Setear un byte de la matriz
void mat_set_byte(matrix_hand_t *mat, uint8_t y, uint8_t x, uint8_t byte){
    assert(mat != NULL && mat->mat != NULL);
    assert(x < mat->width && y < mat->height);

    mat->mat[x+y*mat->width] = byte;
}

// visualizar matriz
void mat_print(matrix_hand_t *mat){
    int x,y;
    for(y=0; y < mat->height; y++){
        for(x=0; x < mat->width; x++){
            printf("%-3u", mat_get_byte(mat, y, x));
        }
        putchar('\n');
    }
}

void mat_copyFromTo(matrix_hand_t *fromMat, matrix_hand_t *toMat, uint8_t offsetY, uint8_t offsetX){
    assert(mat_validate(toMat, fromMat->height+offsetY, fromMat->width+offsetX));

    int x,y;
    for(y=0; y<fromMat->height; y++){
        for(x=0; x<fromMat->width; x++){
            uint8_t byte = mat_get_byte(fromMat, y, x);
            mat_set_byte(toMat, offsetY+y, offsetX+x, byte);
        }
    }
}
