#include <stdio.h>
#include "./backend/game.h"
#include "./testing/easy_timer.h"
#include "./testing/teclado_trucho.h"


void key_up(void){
    printf("UP pressed\n");
}

void key_down(void){
    printf("DOWN pressed\n");
    if(game_data.id == 0) //La pieza colisiono
	{
        game_insert_block(game_get_next_block());
        game_run();
        print_matrix();
    }else //La pieza sigue descendiendo
	{
        game_move_down();
        game_run();
        print_matrix();
    }
}

void key_left(void){
    printf("LEFT pressed\n");

    if(is_pressed(KEY_UP)){
        printf("UP+LEFT pressed\n");
        game_rotate(0);
    }else{
        game_move_horizontal(0);
    }
    game_run();
    print_matrix();

}

void key_right(void){
    printf("RIGHT pressed\n");

    if(is_pressed(KEY_UP)){
        printf("UP+RIGHT pressed\n");
        game_rotate(1);
    }else{
        game_move_horizontal(1);
    }
    game_run();
    print_matrix();
}

void long_press(void){
    printf("long press\n");
    int i;
    for(i=0; i<4; i++){
        if(is_pressed(i)){
            switch (i)
            {
            case KEY_DOWN:
                key_down();
                break;
            
            case KEY_LEFT:
                key_left();
                break;

            case KEY_RIGHT:
                key_right();
                break;

            default:
                break;
            }
        }
    }
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

    interval_t longpress_int = set_interval(long_press, 150);

    teclado_begin();
    game_run();
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
        if(is_long_pressed(KEY_DOWN) || is_long_pressed(KEY_LEFT) || is_long_pressed(KEY_RIGHT)){
            run_interval(&longpress_int);
        }
        teclado_run(); // lee las teclas (No bloqueante)
    }

    teclado_close();
    return 0;
}