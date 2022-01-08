#define SCREEN_W  640
#define SCREEN_H  480

#include <stdio.h>
#include <allegro5/allegro.h>
#include "easy_timer.h"
#include "teclado_trucho.h"

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;

bool key_state[4] = {false, false, false, false};
bool last_keystate[4] = {false, false, false, false}; //Estado de teclas, true cuando esta apretada
uint64_t key_press_timestamp[4] = {0, 0, 0, 0}; // timestamp de cuando se presiono la tecla
bool redraw = false;

callback_t on_press[4];

callback_t al_salir;

void set_on_pressed(callback_t f, int id){
    if(id>=0 && id <= 3)
        on_press[id] = f;
}

void set_on_exit(callback_t f){
    al_salir = f;
}

bool is_pressed(int key_id){
    return key_state[key_id];
}

bool is_long_pressed(int key_id){
    return (get_millis()-key_press_timestamp[key_id] >= LONG_PRESS_TIMEOUT) && key_state[key_id];
}


int teclado_begin(void){
    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        return -1;
    }



    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");

        return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        fprintf(stderr, "failed to create display!\n");

        al_destroy_event_queue(event_queue);
        return -1;
    }

    al_clear_to_color(al_map_rgb(255, 0, 255));
    al_set_target_bitmap(al_get_backbuffer(display));

    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_register_event_source(event_queue, al_get_keyboard_event_source()); //REGISTRAMOS EL TECLADO

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    return 0;
}

void teclado_run(void){
    ALLEGRO_EVENT ev;


    if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola, VER RETURN EN DOCUMENT.
    {            

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            if(al_salir != NULL)
                al_salir();
        }else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key_state[KEY_UP] = true;
                    break;

                case ALLEGRO_KEY_DOWN:
                    key_state[KEY_DOWN] = true;
                    break;

                case ALLEGRO_KEY_LEFT:
                    key_state[KEY_LEFT] = true;
                    break;

                case ALLEGRO_KEY_RIGHT:
                    key_state[KEY_RIGHT] = true;
                    break;

                case ALLEGRO_KEY_E:
                    break;

                case ALLEGRO_KEY_Q:
                    break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key_state[KEY_UP] = false;
                    break;

                case ALLEGRO_KEY_DOWN:
                    key_state[KEY_DOWN] = false;
                    break;

                case ALLEGRO_KEY_LEFT:
                    key_state[KEY_LEFT] = false;
                    break;

                case ALLEGRO_KEY_RIGHT:
                    key_state[KEY_RIGHT] = false;
                    break;

                case ALLEGRO_KEY_ESCAPE:
                        if(al_salir != NULL)
                            al_salir();
                        break;
                }
            }
        }

        int i;
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN || ev.type == ALLEGRO_EVENT_KEY_UP){
            for(i=0; i<4; i++){

                if(key_state[i] != last_keystate[i]){
                    if(key_state[i] & !last_keystate[i]){
                        key_press_timestamp[i] = get_millis();
                        // printf("Presionada la tecla %u\n", i);
                        if (on_press[i] != NULL){
                            on_press[i]();
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

void teclado_close(void){
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
}