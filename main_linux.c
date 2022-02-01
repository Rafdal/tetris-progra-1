/* 
 * File:   main.c
 * Author: r2d2
 *			Ramiro A. Merello (rmerello@itba.edu.ar)
 *
 * Created on June 4, 2016, 6:38 PM
 */
#include "./backend/game.h"
#include "./frontend/easy_timer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_image.h> //NO OLVIDAR INCLUIR ALLEGRO_IMAGE EN LINKER
#include <allegro5/allegro_audio.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_acodec.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "./frontend/keyboard.h"
#include "./frontend/textblocks.h"


#define BLOCKSZ 50
#define ANCHO   10
#define ALTO    16
#define PATH_LATO "./frontend/images/Lato-Black.ttf"

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_BITMAP *image = NULL;
ALLEGRO_BITMAP *muroH = NULL;
ALLEGRO_BITMAP *muroV = NULL;
ALLEGRO_SAMPLE *sample = NULL;
ALLEGRO_EVENT_QUEUE * event_queue = NULL;
ALLEGRO_BITMAP *pieza_blanca = NULL;


/*void playAudio(void){
    al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
}
*/


/* char matriz [16][10]={      // Esto no lo estas usando
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,1,1,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,5,5,5,0,0,0},
    {0,0,0,0,0,5,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
}; */


// P R O T O T I P O S
int initialize_display(void);
void end_program (void);
void update_display(void);
void keypress_callback(uint8_t key);
void read_events(void);
void update_menu_display(void);
void animation_row_compleate(void);
void main_game_start(void);
//int init_audio(void);


int main (void){

	game_init(); // inicializar la libreria del juego

    easytimer_set_realTimeLoop(read_events); // Leer eventos durante delays

	int error = initialize_display();
//	int init_audio();
    if(error){
        printf("Error al iniciar");
        return 0;   //si algo fallo termino el programa
    }
    keyb_on_press(keypress_callback);
    keyb_use_press_callback_for_longpress(KEYB_DOWN);
    keyb_use_press_callback_for_longpress(KEYB_LEFT);
    keyb_use_press_callback_for_longpress(KEYB_RIGHT);

    // menu_set_event_listener_display(read_events, update_menu_display);
   
   // menu_run(main_menu);
   main_game_start();
    
    end_program();//borro todo

    return 0;

}

void main_game_start(void){

    game_data_t game_data;
    uint64_t lastMillis;

    game_start(); // iniciar el juego

    while ((game_data = game_get_data()).state != GAME_QUIT)
    {
        read_events(); // Leer teclado y eventos (Allegro)
        
        if(game_data.state == GAME_RUN && easytimer_get_millis()-lastMillis >= game_data.speed_interval){
            game_move_down();
            game_run();
			animation_row_compleate();
            update_display();
            lastMillis = easytimer_get_millis();
        }

        if(game_data.state == GAME_LOSE){
            printf("Perdiste! The Game\n");
            break;
            #warning BREAK HARDCODEADO
            // Aca deberia ir alguna funcion de mostrar score o guardarlo, etc (MAS ADELANTE LO VEMOS)
        }
    }
    printf("Leaving game...\n");
} // main_game_start


void animation_row_compleate(void)
{
	int i;
    int z;
    float reductor;
    float angulo;
    float decremento= 0.1;

    for(reductor=2.1, angulo=0; reductor>=0; angulo+=(3.1415/8)){
 
        for(z=1; z<=ANCHO; z++){

	        for( i=0; row_compleate[i] != 0 && i< WIDTH ; i++)
	    {
                al_draw_scaled_bitmap(image, 0, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image), BLOCKSZ*z, BLOCKSZ*(row_compleate[i]), BLOCKSZ, BLOCKSZ, 0);
              //pongo el fondo en negro
                al_draw_tinted_scaled_rotated_bitmap(pieza_blanca,  al_map_rgba_f(1, 1, 1, 1), al_get_bitmap_width(pieza_blanca)/2, al_get_bitmap_height(pieza_blanca)/2, (BLOCKSZ/2 +BLOCKSZ*z), (BLOCKSZ/2 +BLOCKSZ*(row_compleate[i])),reductor, reductor, angulo, 0);
                //se va haciendo mas chia a medida que rota
                al_flip_display();
                easytimer_delay(5);
                
            }
            reductor-=decremento;
        }

	}
    for(i=0; row_compleate[i] != 0 && i< WIDTH ; i++){
        for(z=1; z<=ANCHO; z++){
            al_draw_scaled_bitmap(image, 0, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image), BLOCKSZ*z, BLOCKSZ*(row_compleate[i]), BLOCKSZ, BLOCKSZ, 0);
            al_flip_display();
        } //pongo el fondo en negro de nuevo
		delete_row(row_compleate[i]);
		row_compleate[i]= 0;
    }
}

// HACER !
void update_menu_display(void){

   // text_t* titulo_txt = text_init("TETRIS JAJA", 14, NEGRO, ROJO);
   // text_display(titulo_txt, x, y);
   // text_set(titulo_txt, );
}

// Leer los eventos de Allegro (teclado, display, etc)
void read_events(void){
    ALLEGRO_EVENT ev;
    if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola
    {
        keyb_run(&ev); // paso como parametro a

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            game_quit();
        }
    }
}



void keypress_callback(uint8_t key){

    if(easytimer_delay_active()){ // si hay un delay activo no hago nada
        return;
    }

    switch (key)
    {
        case KEYB_UP:
            printf("UP\n");
            break;

        case KEYB_DOWN:
            if(game_get_data().id == 0)
                game_insert_block(id_next_block[0]);
            else
                game_move_down();
            printf("DOWN\n");
            break;

        case KEYB_LEFT:
            game_move_horizontal(0);
            printf("LEFT\n");
            break;

        case KEYB_RIGHT:
            game_move_horizontal(1);
            printf("RIGHT\n");
            break;

        case KEYB_E:
            game_rotate(1);
            printf("UPRIGHT\n");
            break;

        case KEYB_Q:
            game_rotate(0);
            printf("UPLEFT\n");
            break;

        case KEYB_SPACE:
            game_start();
            printf("BTN\n");
            break;

        case KEYB_ESC:
            game_quit();
            printf("Game quit\n");
            break;
            

        default:
            break;
    }
    game_run();
	animation_row_compleate();
    update_display(); // estamos viendo si esto buguea la eliminacion de filas
}


void update_display(void) {

	uint8_t x, y;
	for(x=0; x<WIDTH ; x++)
	{
		for(y=0; y<HEIGHT ; y++)
		{
			float val = (float)game_public_matrix[y][x];
			al_draw_scaled_bitmap(image, (al_get_bitmap_width(image)/8) * val, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image),BLOCKSZ + BLOCKSZ*x, BLOCKSZ*y, BLOCKSZ, BLOCKSZ, 0);
		}
	}
    for(x=0; x<4 ; x++)
	{
		for(y=0; y<3 ; y++)
		{
            float val= (float) next_block_public_matrix[y][x];
            al_draw_scaled_bitmap(image, (al_get_bitmap_width(image)/8) * val, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image),BLOCKSZ*(ANCHO+3) + BLOCKSZ*x, BLOCKSZ*(y+3), BLOCKSZ, BLOCKSZ, 0);
            
        }
    }//DIBUJO PIEZA SIGUIENTE

	al_flip_display(); //despues de esribir toda la matriz muestro lo que escribi
	printf("SCORE:\n%u\n", game_get_data().score);

}


int initialize_display(void) {
    if (!al_init()) {
        printf( "failed to initialize allegro!\n");
        return -1;
    }

    event_queue = al_create_event_queue(); //creo cola de eventos
    if (!event_queue) {
        printf( "failed to create event_queue!\n");
        return -1;
    }

    if(!keyb_init(event_queue)){
        printf("No se pudo inicializar el teclado!\n");
        return -1;
    }

     ///////////////////////////////////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////////

    if (!al_init_image_addon()) { // ADDON necesario para manejo(no olvidar el freno de mano) de imagenes 
        printf("failed to initialize image addon !\n");
        return -1;
    }
    if (!al_init_font_addon()) { 
        fprintf(stderr, "failed to initialize font addon !\n");
        return -1;
    }
    if (!al_init_primitives_addon()) { 
        fprintf(stderr, "failed to initialize primitives addon !\n");
        return -1;
    }
    if (!al_init_ttf_addon()) {  
        fprintf(stderr, "failed to initialize ttf addon !\n");
        return -1;
    }

    image = al_load_bitmap("./frontend/images/piezastetris.png");
    if (!image) {
       printf( "failed to load image !\n");
        al_destroy_event_queue(event_queue);
        return -1;
    }

    muroH = al_load_bitmap("./frontend/images/muroH.jpg");
    if (!muroH) {
        printf( "failed to load muroH !\n");
        al_destroy_bitmap(image);
        al_destroy_event_queue(event_queue);
        return -1;
    }
    muroV = al_load_bitmap("./frontend/images/muroV.jpg");
    if (!muroV) {
        printf("failed to load muroV !\n");
        al_destroy_bitmap(image);
        al_destroy_bitmap(muroH);
        al_destroy_event_queue(event_queue);
        return -1;
    }
    pieza_blanca = al_load_bitmap("./frontend/images/white_tile.png");
    if (!pieza_blanca) {
        printf("failed to load pieza_blanca !\n");
        al_destroy_bitmap(image);
        al_destroy_bitmap(muroH);
        al_destroy_bitmap(muroV);
        al_destroy_event_queue(event_queue);
        return -1;
    }

    display = al_create_display(((ANCHO+8)*BLOCKSZ), ((ALTO+1)*BLOCKSZ));
    if (!display) {
        al_destroy_bitmap(image);
        al_destroy_bitmap(muroH);
        al_destroy_bitmap(muroV);
        al_destroy_event_queue(event_queue);
        fprintf(stderr,"failed to create display!\n");
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    
    
    printf("hasta aca todo piola\n");
    blocktext_t * pieza_sig = text_init_alleg(al_map_rgb(255,255,255), al_map_rgb(0,0,0), 30, "PIEZA SIGUIENTE", PATH_LATO, BLOCKSZ*(ANCHO+2.5), BLOCKSZ, ALINEADO_IZQUIERDA );
    
    if(pieza_sig==NULL){
        printf("se cago el bloque");
    }
    
    if(text_global_font_changer(pieza_sig))
    {
        printf("error con text_global_font_changer");
    } //no funca esto
     
   if(pieza_sig==NULL){
       printf("error con la pieza sig");
        al_destroy_bitmap(image);
        al_destroy_bitmap(muroH);
        al_destroy_bitmap(muroV);
        al_destroy_event_queue(event_queue);
        al_destroy_display(display);
   }
    text_drawer(pieza_sig);
    
   
    //ahora dibujo el muro horizontal
    al_draw_scaled_bitmap(muroH, 0, 0, al_get_bitmap_width(muroH), al_get_bitmap_height(muroH), 0, BLOCKSZ*(ALTO), ((ANCHO+2)*BLOCKSZ), BLOCKSZ, 0);
    //empiezo uso la ultima fila
    //dibujo las paredes
    al_draw_scaled_bitmap(muroV, 0, 0, al_get_bitmap_width(muroV), al_get_bitmap_height(muroV), 0, 0, BLOCKSZ, al_get_display_height(display)-BLOCKSZ, 0);
    al_draw_scaled_bitmap(muroV, 0, 0, al_get_bitmap_width(muroV), al_get_bitmap_height(muroV),((ANCHO+2)*BLOCKSZ)-BLOCKSZ , 0, BLOCKSZ, al_get_display_height(display)-BLOCKSZ, 0);
        
    
    al_flip_display();
    
    //playAudio();

    return 0;
}

int init_audio(void) {
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_SAMPLE *sample = NULL;
	//ALLEGRO_SAMPLE *sample1 = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;

    if (!al_install_audio()) {
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

    sample = al_load_sample("audio.wav");
	//sample1 = al_load_sample("audio2.wav");

    if (!sample) {
        printf("Audio clip sample not loaded!\n");
        return -1;
    }



    //Loop the sample until the display closes.
    al_play_sample(sample, 1.0, -1.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

//	al_play_sample(sample1, 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

   /*   while (!display_close) {
        ALLEGRO_EVENT ev;
        if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola, VER RETURN EN DOCUMENT.
        {
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                display_close = true;
        }

    }
   */
    al_uninstall_audio();
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(sample);
    return 0;
}


void end_program (void){
    al_destroy_display(display);
    al_destroy_bitmap(image);
    al_destroy_bitmap(muroH);
    al_destroy_bitmap(muroV);
    al_destroy_event_queue(event_queue);
    al_uninstall_keyboard();
    //al_uninstall_audio(); // borrar audio
    //al_destroy_sample(sample);
    //al_shutdown_image_addon(); VER DOCUMENTACION ES LLAMADO AUTOMATICAMENTE AL SALIR DEL PROGRAMA
    printf("Game Ended\n");
}

/*void deleteline (int numfil){
    int x;
    int reductor;
    float angulo;
        for(x=1; x<=WIDTH; x++){
            al_draw_scaled_bitmap(image,(al_get_bitmap_width(image)/8), 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image), BLOCKSZ*x, BLOCKSZ*numfil, BLOCKSZ, BLOCKSZ, 0);
        }  //pongo el fondo en negro
        for(reductor=1, angulo=(3,1415/8); reductor<10; reductor++, angulo+=(3,1415/8)){
            for(x=1; x<=WIDTH; x++){
                al_draw_tinted_scaled_rotated_bitmap_region(whitepiece, 0, 0, al_get_bitmap_width(whitepiece), al_get_bitmap_height(whitepiece), al_map_rgba_f(1, 1, 1, 1), al_get_bitmap_width(whitepiece)/2,al_get_bitmap_height(whitepiece), (BLOCKSZ/2 +BLOCKSZ*x), (BLOCKSZ/2 +BLOCKSZ*numfil),1/reductor, 1/reductor, angulo, 0);
                //se va haciendo mas chia a medida que rota
            }
        }
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////AUDIO/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

