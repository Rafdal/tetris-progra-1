/* 
 * File:   main.c
 * Author: r2d2
 *			Ramiro A. Merello (rmerello@itba.edu.ar)
 *
 * Created on June 4, 2016, 6:38 PM
 */
#include "./backend/game.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <allegro5/allegro.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_image.h> //NO OLVIDAR INCLUIR ALLEGRO_IMAGE EN LINKER
#include <allegro5/allegro_audio.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_acodec.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <stdbool.h>


#define BLOCKSZ 50
#define ANCHO   10
#define ALTO    16
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_BITMAP *image = NULL;
    ALLEGRO_BITMAP *muroH = NULL;
    ALLEGRO_BITMAP *muroV = NULL;
    ALLEGRO_SAMPLE *sample = NULL;
    ALLEGRO_EVENT_QUEUE * event_queue = NULL;

/*void playAudio(void){
    al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
}
*/

    pthread_t th1;
    bool close_display = false; 

int initialize_display(void);
void printer (void);
void endgame (void);

int main (void){
    int ret= initialize_display();
    if(ret){
        printf("Error al iniciar");
        return 0;   //si algo fallo termino el programa
    }

    pthread_create (&th1, NULL, thread1, NULL);
    pthread_join (th1, NULL);
    return 0;

}

void * thread1 (){
    while (!close_display) {
        ALLEGRO_EVENT ev;
        if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola, VER RETURN EN DOCUMENT.
        {
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                close_display = true;
        }
        printer();
    }
    endgame();//close display
}

int initialize_display(void) {


    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    event_queue = al_create_event_queue(); //creo cola de eventos
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        return -1;
    }

    /*if (!al_install_audio()) {
        fprintf(stderr, "failed to initialize audio!\n");
        return -1;
    }

    if (!al_init_acodec_addon()) {
        fprintf(stderr, "failed to initialize audio codecs!\n");
        return -1;
    }

    if (!al_reserve_samples(1)) {
        fprintf(stderr, "failed to reserve samples!\n");
        return -1;
    }

    const char* path = "tetris.wav";
    sample = al_load_sample(path);

    if (!sample) {
        printf("Audio clip sample \"%s\" not loaded!\n", path);
        return -1;
    }*/

    if (!al_init_image_addon()) { // ADDON necesario para manejo(no olvidar el freno de mano) de imagenes 
        fprintf(stderr, "failed to initialize image addon !\n");
        return -1;
    }

    image = al_load_bitmap("./frontend/images/piezastetris.png");
    if (!image) {
        fprintf(stderr, "failed to load image !\n");
        al_destroy_event_queue(event_queue);
        return 0;
    }

     muroH = al_load_bitmap("./frontend/images/muroH.jpg");
    if (!image) {
        fprintf(stderr, "failed to load image !\n");
        return 0;
    }
     muroV = al_load_bitmap("./frontend/images/muroV.jpg");
    if (!image) {
        fprintf(stderr, "failed to load image !\n");
        return 0;
    }
     
    display = al_create_display(((ANCHO+2)*BLOCKSZ), ((ALTO+1)*BLOCKSZ));
    if (!display) {
        al_destroy_bitmap(image);
        al_destroy_event_queue(event_queue);
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }
    al_register_event_source(event_queue, al_get_display_event_source(display));
    //ahora dibujo el muro horizontal
    al_draw_scaled_bitmap(muroH, 0, 0, al_get_bitmap_width(muroH), al_get_bitmap_height(muroH), 0, BLOCKSZ*(ALTO), al_get_display_width(display), BLOCKSZ, 0);
    //empiezo uso la ultima fila
    //dibujo las paredes
    al_draw_scaled_bitmap(muroV, 0, 0, al_get_bitmap_width(muroV), al_get_bitmap_height(muroV), 0, 0, BLOCKSZ, al_get_display_height(display)-BLOCKSZ, 0);
    al_draw_scaled_bitmap(muroV, 0, 0, al_get_bitmap_width(muroV), al_get_bitmap_height(muroV),al_get_display_width(display)-BLOCKSZ , 0, BLOCKSZ, al_get_display_height(display)-BLOCKSZ, 0);
    
    /*al_draw_scaled_bitmap(image, (al_get_bitmap_width(image)/8)*2, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image), BLOCKSZ-5, 0, BLOCKSZ, BLOCKSZ, 0);
        al_draw_scaled_bitmap(image, (al_get_bitmap_width(image)/8), 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image), BLOCKSZ*2, 0, BLOCKSZ, BLOCKSZ, 0);
            al_draw_scaled_bitmap(image, (al_get_bitmap_width(image)/8)*2, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image), BLOCKSZ*3-4, 0, BLOCKSZ, BLOCKSZ, 0);


    al_draw_tinted_scaled_bitmap(image, al_map_rgba_f(0.2, 0.2, 0.2, 0.2), (al_get_bitmap_width(image)/8)*2, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image), BLOCKSZ*2, BLOCKSZ, BLOCKSZ, BLOCKSZ, 0);
    //void al_draw_bitmap(ALLEGRO_BITMAP *bitmap, float dx, float dy, int flags) 
    /*al_draw_scaled_bitmap(image,
            0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image), //imagen
            0, 0, al_get_display_width(display), al_get_display_height(display), //a que tamaño queres que se dibuje la imagen
            0);*/ //SIn flags podrian usar ALLEGRO_FLIP_HORIZONTAL o ALLEGRO_FLIP_VERTICAL muy utiles

    al_flip_display();
    
    //playAudio();

    return 0;
}

void endgame (void){
    al_destroy_display(display);
    al_destroy_bitmap(image);
    al_destroy_event_queue(event_queue);
    //al_uninstall_audio(); // borrar audio
    //al_destroy_sample(sample);
    //al_shutdown_image_addon(); VER DOCUMENTACION ES LLAMADO AUTOMATICAMENTE AL SALIR DEL PROGRAMA
}

void printer (void){
    char x, y;
    for(x=0; x<WIDTH ; x++)
    {
        for(y=0; y<HEIGHT ; y++)
        {
        al_draw_scaled_bitmap(image, (al_get_bitmap_width(image)/8) * game_public_matrix[y][x], 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image),BLOCKSZ + BLOCKSZ*x, BLOCKSZ*y, BLOCKSZ, BLOCKSZ, 0);\
        }
    }
    al_flip_display(); //despues de esribir toda la matriz muestro lo que escribi

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////AUDIO/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

