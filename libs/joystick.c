#include <stdio.h>
#include "easy_timer.h"
#include "joystick.h"
#include "joydrv.h"

// const char dpad_key_names[][DPAD_NAMELEN] = {"UP", "RIGHT", "DOWN", "LEFT", "UPLEFT", "UPRIGHT", "BTN"};

//  #############################
//  #     PRIVATE VARIABLES     #
//  #############################

// bool idle_flag;
bool key_state[DPAD_KEYS];
bool last_key_state[DPAD_KEYS];

uint64_t key_timestamp[DPAD_KEYS]; // last press para debouncear el boton

dpad_callback_t on_press;

//  #############################
//  #      PUBLIC FUNCTIONS     #
//  #############################

void dpad_init(void){

    joy_init();
    int id;
    // idle_flag = false;
    for(id=0; id<DPAD_KEYS; id++){
        key_state[id] = false;
        last_key_state[id] = false;
    }
}

bool dpad_is_pressed(uint8_t id){
    if(id >= DPAD_KEYS)
        return false;
    return key_state[id];
}

bool dpad_is_longpressed(uint8_t id){
    if(id >= DPAD_KEYS)
        return false;
    return key_state[id] && get_millis()-key_timestamp[id] >= DPAD_LONG_PRESS;
}

void dpad_on_press(dpad_callback_t c){
    on_press = c;
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
    if(joy.y > DPAD_UMBRAL && joy.x < -DPAD_UMBRAL/8)
        key_state[DPAD_UPLEFT] = true;
    else if(joy.y < DPAD_DEBOUNCE)
        key_state[DPAD_UPLEFT] = false;

    // UPRIGHT ARROW
    if(joy.y > DPAD_UMBRAL && joy.x > DPAD_UMBRAL/8)
        key_state[DPAD_UPRIGHT] = true;
    else if(joy.y < DPAD_DEBOUNCE)
        key_state[DPAD_UPRIGHT] = false;


    // boton del joystick (no tuve problemas con el debounceo, asi que lo mando directo)
    if(btn == J_PRESS){
        key_state[DPAD_BTN] = true; //  ### SIN DEBOUNCE
    }
    else if(btn == J_NOPRESS){
        key_state[DPAD_BTN] = false;
    }

    int id;
    for(id=0; id<DPAD_KEYS; id++){ // analizo todos los botones
        if(key_state[id] != last_key_state[id]){      // Si cambio el estado
            if(key_state[id] && !last_key_state[id]){ // Si fue un flanco ascendente
                // idle_flag = false;
                key_timestamp[id] = get_millis();

                if(on_press != NULL)        // Si el callback esta seteado
                    on_press(id);

            }
            // actualizo el estado anterior
            last_key_state[id] = key_state[id];
        }
    }

    /* if(!idle_flag){ // si antes se habia presionado alguna tecla
        idle_flag = true;
        for(id=0; id<DPAD_KEYS && idle_flag; id++){
            idle_flag &= !key_state[id]; // reviso si no hay ninguna presionada (button release, flanco descendente)
        }

        if(idle_flag){  // si efectivamente no habia ninguna
            if(on_press != NULL)
                on_press(DPAD_idle);  // marco que el joystick esta suelto
        }
    } */
        
}

