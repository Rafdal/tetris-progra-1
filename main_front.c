/* 
 * File:   main.c
 * Author: r2d2
 *			Ramiro A. Merello (rmerello@itba.edu.ar)
 *
 * Created on June 4, 2016, 6:38 PM
 */

#include <stdio.h>

#include "./frontend/game_screen.h"

const color_t colors[4] = {
    {255, 0, 0}, // RED
    {0, 255, 0}, // GREEN
    {0, 0, 255}, // BLUE
    {128, 128, 128}, // GREY
};

int main(void){
    game_screen_begin();

    game_screen_clear();
    
    for(int i=0; i<4; i++){
        game_screen_draw(16+i*34, 16, colors[i]);
    }

    game_screen_show();
    while (run)
    {
        game_screen_run();
    }
    game_screen_end();
}