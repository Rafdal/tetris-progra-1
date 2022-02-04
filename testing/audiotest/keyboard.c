#define SCREEN_W  640
#define SCREEN_H  480

#include <stdio.h>
// #include <assert.h>
#include "easy_timer.h"
#include "keyboard.h"

static ALLEGRO_EVENT_QUEUE *event_queue = NULL;

static bool key_state[KEYB_KEYS];
static bool last_keystate[KEYB_KEYS]; //Estado de teclas, true cuando esta apretada
static uint64_t key_press_timestamp[KEYB_KEYS]; // timestamp de cuando se presiono la tecla
static bool use_press_callback_for_longpress[KEYB_KEYS];

static keyb_callback_t on_press;
static uint64_t lastMillis;


void keyb_on_press(keyb_callback_t f){
    on_press = f;
}

bool keyb_is_pressed(uint8_t key_id){
    return key_state[key_id];
}

bool keyb_is_longpressed(uint8_t key_id){
    return (easytimer_get_millis()-key_press_timestamp[key_id] >= LONG_PRESS_INTERVAL) && key_state[key_id];
}

void keyb_use_press_callback_for_longpress(uint8_t key){
    use_press_callback_for_longpress[key] = true;
}

// Inicializar el teclado
bool keyb_init(ALLEGRO_EVENT_QUEUE *queue){

    if(!al_install_keyboard() || queue == NULL)
        return false; // error

    event_queue = queue;

    int i;
    for(i=0; i<KEYB_KEYS; i++){
        key_state[i] = false;
        last_keystate[i] = false; //Estado de teclas, true cuando esta apretada
        key_press_timestamp[i] = 0; // timestamp de cuando se presiono la tecla
        use_press_callback_for_longpress[i] = false;
    }

    al_register_event_source(event_queue, al_get_keyboard_event_source()); //REGISTRAMOS EL TECLADO
    return true;
}

#ifdef DEBUG
void keyb_debug(void){
    uint8_t i;
    for(i=0; i< KEYB_KEYS; i++){
        
    }
}
#endif 

// Va dentro de if(al_get_next_event(event_queue, pev) ) { ACA }
void keyb_run(ALLEGRO_EVENT* pev){
    if (pev->type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (pev->keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                key_state[KEYB_UP] = true;
                break;

            case ALLEGRO_KEY_DOWN:
                key_state[KEYB_DOWN] = true;
                break;

            case ALLEGRO_KEY_LEFT:
                key_state[KEYB_LEFT] = true;
                break;

            case ALLEGRO_KEY_RIGHT:
                key_state[KEYB_RIGHT] = true;
                break;

            case ALLEGRO_KEY_E:
                key_state[KEYB_E] = true;
                break;

            case ALLEGRO_KEY_Q:
                key_state[KEYB_Q] = true;
                break;

            case ALLEGRO_KEY_ESCAPE:
                key_state[KEYB_ESC] = true;
                break;

            case ALLEGRO_KEY_SPACE:
                key_state[KEYB_SPACE] = true;
                break;
        }
    }
    else if (pev->type == ALLEGRO_EVENT_KEY_UP) {
        switch (pev->keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                key_state[KEYB_UP] = false;
                break;

            case ALLEGRO_KEY_DOWN:
                key_state[KEYB_DOWN] = false;
                break;

            case ALLEGRO_KEY_LEFT:
                key_state[KEYB_LEFT] = false;
                break;

            case ALLEGRO_KEY_RIGHT:
                key_state[KEYB_RIGHT] = false;
                break;

            case ALLEGRO_KEY_E:
                key_state[KEYB_E] = false;
                break;

            case ALLEGRO_KEY_Q:
                key_state[KEYB_Q] = false;
                break;

            case ALLEGRO_KEY_SPACE:
                key_state[KEYB_SPACE] = false;
                break;

            case ALLEGRO_KEY_ESCAPE:
                key_state[KEYB_ESC] = false;
                break;
        }
    }
    int i;
    if(pev->type == ALLEGRO_EVENT_KEY_DOWN || pev->type == ALLEGRO_EVENT_KEY_UP){
        for(i=0; i<KEYB_KEYS; i++){

            if(key_state[i] != last_keystate[i]){
                if(key_state[i] & !last_keystate[i]){
                    key_press_timestamp[i] = easytimer_get_millis();
                    // printf("Presionada la tecla %u\n", i);
                    if (on_press != NULL){
                        on_press(i);
                    }
                    
                }
                last_keystate[i] = key_state[i];
            }
        }
    }
    int id;
    if(easytimer_get_millis()-lastMillis >= KEYB_LONGPRESS_DELAY){
        for(id=0; id< KEYB_KEYS; id++){
            if(use_press_callback_for_longpress[id] && keyb_is_longpressed(id)){
                if(on_press != NULL)        // Si el callback esta seteado
                    on_press(id);
                
            }
        }
        lastMillis = easytimer_get_millis();
    }
}