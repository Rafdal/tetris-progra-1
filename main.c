#include <stdio.h>
#include "game.h"
#include "easytimer.h"

void down(void);
void rot(int dir);

int main(void){

    insert_block(7);
    print_matrix();

    int i;
    for (i = 0; i < 4; ++i) {
        rot(1);
    }

    return 0;
}

void down(void){
    descend_block();
    print_matrix();
}
void rot(int dir){
    rotate_block(dir);
    print_matrix();
}