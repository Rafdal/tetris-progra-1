#include <stdio.h>
#include "easy_timer.h"
#include "joystick.h"
#include "joydrv.h"

bool key_state[DPAD_KEYS];
bool last_key_state[DPAD_KEYS];

uint64_t key_timestamp[DPAD_KEYS]; // last press para debouncear el boton

dpad_callback_t press_cback;

bool dpad_is_pressed(uint8_t id){
    if(id >= DPAD_KEYS)
        return false;
    return key_state[id];
}

bool dpad_is_longpressed(uint8_t id){
    if(id >= DPAD_KEYS)
        return false;
    return key_state[id] && get_millis()- key_timestamp[id] >= DPAD_LONG_PRESS;
}

void dpad_on_press(dpad_callback_t c){
    press_cback = c;
} // Setear callback click de tecla (joystick)



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


    // Joystick button (no tuve problemas con el debounceo, asi que lo mando directo)
    if(btn == J_PRESS){
        key_state[DPAD_BTN] = true; //  ### NO DEBOUNCE
    }
    else if(btn == J_NOPRESS){
        key_state[DPAD_BTN] = false;
    }

    int id;
    for(id=0; id<DPAD_KEYS; id++){
        if(key_state[id] != last_key_state[id]){      // Si cambio el estado
            if(key_state[id] && !last_key_state[id]){ // Si fue un flanco ascendente
                key_timestamp[id] = get_millis();

                if(press_cback != NULL)        // Si el callback no es un puntero nulo
                    press_cback(id);

            }
            // actualizo el estado anterior
            last_key_state[id] = key_state[id];
        }
    }
}

void dpad_init(void){
    joy_init();
    int id;
    for(id=0; id<DPAD_KEYS; id++){
        key_state[id] = false;
        last_key_state[id] = false;
    }
}

