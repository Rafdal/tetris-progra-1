#include <stdio.h>
#include "./backend/game.h"
#include "./libs/easy_timer.h"
#include "./libs/teclado_trucho.h"


void key_up(void){
    printf("UP pressed\n");
}

void key_down(void){
    printf("DOWN pressed\n");
    if(block_data.id == 0) //La pieza colisiono
	{
        insert_block(next_block());
        run_game();
        print_matrix();
    }else //La pieza sigue descendiendo
	{
        descend_block();
        run_game();
        print_matrix();
    }
}

void key_left(void){
    printf("LEFT pressed\n");

    if(is_pressed(KEY_UP)){
        printf("UP+LEFT pressed\n");
        rotate_block(0);
    }else{
        move_block(0);
    }
    run_game();
    print_matrix();

}

void key_right(void){
    printf("RIGHT pressed\n");

    if(is_pressed(KEY_UP)){
        printf("UP+RIGHT pressed\n");
        rotate_block(1);
    }else{
        move_block(1);
    }
    run_game();
    print_matrix();
}

int run=1;

void finish(void){
    printf("finished\n");
    run = 0;
}

//  ##############################
//  #            MAIN            #
//  ##############################
int main(void) {

    teclado_begin();
    run_game();
    print_matrix();

    // Seteo los callbacks de cada tecla
    set_on_exit(finish);
    set_on_pressed(key_up, KEY_UP);
    set_on_pressed(key_down, KEY_DOWN);
    set_on_pressed(key_left, KEY_LEFT);
    set_on_pressed(key_right, KEY_RIGHT);

    // Loop en tiempo-real (no bloqueante)
    while (run)
    {
        teclado_run(); // lee las teclas (No bloqueante)
    }

    teclado_close();
    return 0;
}