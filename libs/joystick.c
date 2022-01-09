#include <stdio.h>
#include "easy_timer.h"
#include "joystick.h"
#include "joydrv.h"

bool key_state[DPAD_KEYS];
bool last_key_state[DPAD_KEYS];

uint64_t btn_timestamp; // last press para debouncear el boton

dpad_callback_t press_cback;
dpad_callback_t long_cback;

bool dpad_is_pressed(uint8_t key){
    return key_state[key];
}

void dpad_on_press(dpad_callback_t c){
    press_cback = c;
} // Setear callback click de tecla (joystick)

void dpad_on_longpress(dpad_callback_t c){
    long_cback = c;
} // Setear callback para Long Press


void dpad_run(void){
    joy_update();
    jcoord_t joy = joy_get_coord();
    jswitch_t btn = joy_get_switch();
    
    // UP KEY
    if(joy.y > DPAD_UMBRAL && joy.x > -DPAD_UMBRAL/4 && joy.x < DPAD_UMBRAL/4)
        key_state[DPAD_UP] = true;
    else if(joy.y < DPAD_DEBOUNCE)
        key_state[DPAD_UP] = false;

    // RIGHT KEY
    if(joy.x > DPAD_UMBRAL && joy.y > -DPAD_UMBRAL/4 && joy.y < DPAD_UMBRAL/4)
        key_state[DPAD_RIGHT] = true;
    else if(joy.x < DPAD_DEBOUNCE)
        key_state[DPAD_RIGHT] = false;

    // LEFT KEY
    if(joy.x < -DPAD_UMBRAL && joy.y > -DPAD_UMBRAL/4 && joy.y < DPAD_UMBRAL/4)
        key_state[DPAD_LEFT] = true;
    else if(joy.x > -DPAD_DEBOUNCE)
        key_state[DPAD_LEFT] = false;

    // DOWN KEY
    if(joy.y < -DPAD_UMBRAL)
        key_state[DPAD_DOWN] = true;
    else if(joy.y > -DPAD_DEBOUNCE)
        key_state[DPAD_DOWN] = false;


    // UPLEFT KEY
    if(joy.y > DPAD_UMBRAL && joy.x > DPAD_UMBRAL/8)
        key_state[DPAD_UPLEFT] = true;
    else if(joy.y < DPAD_DEBOUNCE)
        key_state[DPAD_UPLEFT] = false;

    // UPRIGHT ARROW
    if(joy.y > DPAD_UMBRAL && joy.x < -DPAD_UMBRAL/8)
        key_state[DPAD_UPRIGHT] = true;
    else if(joy.y < DPAD_DEBOUNCE)
        key_state[DPAD_UPRIGHT] = false;


    // ########## TO-DO #########
    /* // Joystick button - Flanco ascendente
    if(!key_state[DPAD_BTN] && btn == J_PRESS &&  (get_millis() - btn_timestamp) >= DPAD_BTN_DEBOUNCE_TIME ){
        key_state[DPAD_BTN] = true;
        btn_timestamp = get_millis();
    }

    if(key_state[DPAD_BTN] && btn == J_NOPRESS && (get_millis() - btn_timestamp) >= DPAD_BTN_DEBOUNCE_TIME){
        key_state = false;
    } */

    int id;
    for(id=0; id<DPAD_KEYS; id++){
        if(key_state[id] != last_key_state[id]){      // Si cambio el estado
            if(key_state[id] && !last_key_state[id]){ // Si fue un flanco ascendente

                if(press_cback != NULL)        // Si el callback no es un puntero nulo
                    press_cback(id);

            }
            last_key_state[id] = key_state[id];
        }
    }
}

void dpad_init(void){
    joy_init();
}

