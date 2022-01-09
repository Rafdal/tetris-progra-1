#include <stdio.h>
#include "game_screen.h"
#include <allegro5/allegro.h>

#define FPS    60.0
#define SCREEN_W  640
#define SCREEN_H  480
#define CUADRADITO_SIZE 32
#define MOVE_RATE  4.0



ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *cuadradito = NULL;

int game_screen_begin(void){
    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }


    cuadradito = al_create_bitmap(CUADRADITO_SIZE, CUADRADITO_SIZE);
    if (!cuadradito) {
        fprintf(stderr, "failed to create cuadradito bitmap!\n");
        return -1;
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_bitmap(cuadradito);
        return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_bitmap(cuadradito);
        al_destroy_event_queue(event_queue);
        return -1;
    }

    al_set_target_bitmap(cuadradito);
    al_clear_to_color(al_map_rgb(255, 0, 255));
    al_set_target_bitmap(al_get_backbuffer(display));

    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    run = true;
    return 0;
}

void game_screen_draw(float x, float y, color_t color){
    al_set_target_bitmap(cuadradito);
    al_clear_to_color(al_map_rgb(color.r, color.g, color.b));

    al_set_target_bitmap(al_get_backbuffer(display));
    al_draw_bitmap(cuadradito, x, y, 0);
}

void game_screen_clear(void){
    al_set_target_bitmap(al_get_backbuffer(display));
    al_clear_to_color(al_map_rgb(0, 0, 0));
}

void game_screen_show(void){
    al_flip_display();
}

void game_screen_run(void){
    ALLEGRO_EVENT ev;
    if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola, VER RETURN EN DOCUMENT.
    {
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            run = false;

    }
}

void game_screen_end(void){
    al_destroy_bitmap(cuadradito);
    al_destroy_timer(timer);
    al_destroy_display(display);
}