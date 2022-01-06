#ifndef GAME_H
#define GAME_H

#include <stdint.h>

void clear_matrix(void);

void print_matrix(void);

void insert_block(uint8_t id);

void descend_block (void);

void rotate_block (int direction);

void move_block(int direction);

//void printt(void); //Funcion Auxliar para printear una matriz, LUEGO SACAR



#endif // GAME_H