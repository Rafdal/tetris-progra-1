#define SCREEN_W  640
#define SCREEN_H  480

#include <stdio.h>
#include <allegro5/allegro.h>
#include "easy_timer.h"
#include "teclado_trucho.h"

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;

bool key_state[7] = {false, false, false, false, false, false, false};
bool last_keystate[7] = {false, false, false, false, false, false, false}; //Estado de teclas, true cuando esta apretada
uint64_t key_press_timestamp[7] = {0,0,0,0,0,0,0}; // timestamp de cuando se presiono la tecla
bool redraw = false;

dpad_callback_t on_press;

void dpad_on_press(dpad_callback_t f){
    on_press = f;
}

bool dpad_is_pressed(uint8_t key_id){
    return key_state[key_id];
}

bool dpad_is_longpressed(uint8_t key_id){
    return (get_millis()-key_press_timestamp[key_id] >= LONG_PRESS_TIMEOUT) && key_state[key_id];
}


void dpad_init(void){
    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        return;
    }



    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");

        return;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        fprintf(stderr, "failed to create display!\n");

        al_destroy_event_queue(event_queue);
        return;
    }

    al_clear_to_color(al_map_rgb(255, 0, 255));
    al_set_target_bitmap(al_get_backbuffer(display));

    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_register_event_source(event_queue, al_get_keyboard_event_source()); //REGISTRAMOS EL TECLADO

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    return;
}

void dpad_run(void){
    ALLEGRO_EVENT ev;


    if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola, VER RETURN EN DOCUMENT.
    {            

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            al_destroy_display(display);
            al_destroy_event_queue(event_queue);
        }else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key_state[DPAD_UP] = true;
                    break;

                case ALLEGRO_KEY_DOWN:
                    key_state[DPAD_DOWN] = true;
                    break;

                case ALLEGRO_KEY_LEFT:
                    key_state[DPAD_LEFT] = true;
                    break;

                case ALLEGRO_KEY_RIGHT:
                    key_state[DPAD_RIGHT] = true;
                    break;

                case ALLEGRO_KEY_E:
                    key_state[DPAD_UPRIGHT] = true;
                    break;

                case ALLEGRO_KEY_Q:
                    key_state[DPAD_UPLEFT] = true;
                    break;

                case ALLEGRO_KEY_SPACE:
                    key_state[DPAD_BTN] = true;
                    break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key_state[DPAD_UP] = false;
                    break;

                case ALLEGRO_KEY_DOWN:
                    key_state[DPAD_DOWN] = false;
                    break;

                case ALLEGRO_KEY_LEFT:
                    key_state[DPAD_LEFT] = false;
                    break;

                case ALLEGRO_KEY_RIGHT:
                    key_state[DPAD_RIGHT] = false;
                    break;

                case ALLEGRO_KEY_E:
                    key_state[DPAD_UPRIGHT] = false;
                    break;

                case ALLEGRO_KEY_Q:
                    key_state[DPAD_UPLEFT] = false;
                    break;

                case ALLEGRO_KEY_SPACE:
                    key_state[DPAD_BTN] = false;
                    break;


                case ALLEGRO_KEY_ESCAPE:
                    al_destroy_display(display);
                    al_destroy_event_queue(event_queue);
                    break;
                }
            }
        }

        int i;
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN || ev.type == ALLEGRO_EVENT_KEY_UP){
            for(i=0; i<7; i++){

                if(key_state[i] != last_keystate[i]){
                    if(key_state[i] & !last_keystate[i]){
                        key_press_timestamp[i] = get_millis();
                        // printf("Presionada la tecla %u\n", i);
                        if (on_press != NULL){
                            on_press(i);
                        }
                        
                    }
                    last_keystate[i] = key_state[i];
                }
            }
        }
        
        /* if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_flip_display();
        } */
}