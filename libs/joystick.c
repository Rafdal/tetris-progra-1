
#include <stdio.h>
#include <stdbool.h>
#include "joystick.h"
#include "joydrv.h"

bool key_state[6];
bool last_key_state[6];

dpad_callback_t press_cback;
dpad_callback_t long_cback;

void dpad_on_press(dpad_callback_t c){
    press_cback = c;
} // Setear callback click de tecla (joystick)

void dpad_on_longpress(dpad_callback_t c){
    long_cback = c;
} // Setear callback para Long Press



void dpad_run(void){
    joy_update();
    jcoord_t pos = joy_get_coord();
    
    // UP KEY
    if(pos.y > DPAD_UMBRAL && pos.x > -DPAD_UMBRAL/2 && pos.x < DPAD_UMBRAL/2)
        key_state[DPAD_UP] = true;
    else
        key_state[DPAD_UP] = false;

    // RIGHT KEY
    if(pos.x > DPAD_UMBRAL && pos.y > -DPAD_UMBRAL/2 && pos.y < DPAD_UMBRAL/2)
        key_state[DPAD_RIGHT] = true;
    else
        key_state[DPAD_RIGHT] = false;

    // LEFT KEY
    if(pos.x < -DPAD_UMBRAL && pos.y > -DPAD_UMBRAL/2 && pos.y < DPAD_UMBRAL/2)
        key_state[DPAD_LEFT] = true;
    else
        key_state[DPAD_LEFT] = false;

    // DOWN KEY
    if(pos.y < -DPAD_UMBRAL)
        key_state[DPAD_DOWN] = true;
    else
        key_state[DPAD_DOWN] = false;


    // UPLEFT KEY
    if(pos.y > DPAD_UMBRAL && pos.x > DPAD_UMBRAL)
        key_state[DPAD_UPLEFT] = true;
    else
        key_state[DPAD_UPLEFT] = false;

    // UPRIGHT ARROW
    if(pos.y > DPAD_UMBRAL && pos.x < -DPAD_UMBRAL)
        key_state[DPAD_UPRIGHT] = true;
    else
        key_state[DPAD_UPRIGHT] = false;


    int i;
    for(i=0; i<6; i++){
        if(key_state[i] != last_key_state[i]){      // Si cambio el estado
            if(key_state[i] && !last_key_state[i]){ // Si fue un flanco ascendente
                if(press_cback != NULL){        // Si el callback no es un puntero nulo
                    press_cback(i);
                }
            }
            last_key_state[i] = key_state[i];
        }
    }
}

void dpad_init(void){
    joy_init();
}

