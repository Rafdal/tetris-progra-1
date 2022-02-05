/* 
 * File:   main_linux.c
 * Author: RD,AC,AS,SV
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
#include "./backend/menu.h"
#include "./backend/menu.h"


#define BLOCKSZ 50
#define ANCHO   10
#define ALTO    16
#define PATH_TTF "./frontend/images/Tetris.ttf"

// **************************************
// *	 V A R S . G L O B A L E S		*
// **************************************
//BITMAPS
ALLEGRO_BITMAP *image = NULL;
ALLEGRO_BITMAP *muroH = NULL;
ALLEGRO_BITMAP *muroV = NULL;
ALLEGRO_BITMAP *pieza_blanca = NULL;
ALLEGRO_BITMAP *tetris_cartel = NULL;
ALLEGRO_BITMAP *diagrama_teclado = NULL;

//UTILIDADES ALLEGRO
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_SAMPLE *sample = NULL;
ALLEGRO_EVENT_QUEUE * event_queue = NULL;

//UTILIDADES EXTRA
blocktext_t * score=NULL;
blocktext_t * nivel=NULL;
menu_t *principal_menu = NULL;
menu_t *pausa_menu = NULL;


/*void playAudio(void){
    al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
}
*/



// ******************************
// *	P R O T O T I P O S		*
// ******************************

int initialize_alleg(void);
void initialize_display_game (void);
void end_program (void);
void update_display(void);
void keypress_callback(uint8_t key);
void read_events(void);
void display_menu_display(void);
void animation_row_compleate(void);
void main_game_start(void);
void how_to_play (void);
//int init_audio(void);

// ******************************
// *	C A L L B A C K S		*
// ******************************
//CALLBACK DE EXIT GAME
void exit_game(void){
    game_quit();                // Finalizar juego
    menu_force_close(pausa_menu); // Cerrar menu pausa
}

//CALLBACK DE REINICIO DE JUEGO
void restart_game(void){
	initialize_display_game(); //inicio el display del juego
    menu_force_close(pausa_menu); // Cerrar menu pausa
	game_init();	//Inicio el juego
    game_start(); 	//Corre el juego
}

//CALLBACK DE RENAUDAR JUEGO
void resume_game(void)
{
	initialize_display_game(); //inicio el display del juego
	menu_force_close_current();	//Cierra el menu
	game_run();	//Corre el juego
}

//CALLBACK DE CONTROLES/INSTRUCCIONES
void how_to_play (void){
    easytimer_delay(300);
    al_draw_scaled_bitmap(diagrama_teclado, 0, 0, al_get_bitmap_width(diagrama_teclado), al_get_bitmap_height(diagrama_teclado), 0, 0, al_get_display_width(display), al_get_display_height(display), 0);
    al_flip_display();
    while ((!keyb_is_pressed(KEYB_ENTER)) && (!keyb_is_pressed(KEYB_ESC)) && (!keyb_is_pressed(KEYB_SPACE)))
    {
        read_events();
    }
    
}


int main (void){

	game_init(); // inicializar la libreria del juego

    easytimer_set_realTimeLoop(read_events); // Leer eventos durante delays

	int error = initialize_alleg(); //inicializo Allegro

    
    keyb_on_press(keypress_callback);
    keyb_use_press_callback_for_longpress(KEYB_DOWN);
    keyb_use_press_callback_for_longpress(KEYB_LEFT);
    keyb_use_press_callback_for_longpress(KEYB_RIGHT);

//	int init_audio();
    if(error){
        printf("Error al iniciar");
        return 0;   //si algo fallo termino el programa
    }
    
    principal_menu = menu_init(3, "MENU PRINCIPAL", NULL, MENU_ACTION_DO_NOTHING);
    pausa_menu = menu_init(3, "MENU DE PAUSA", NULL, MENU_ACTION_JUST_EXIT);


    if(principal_menu == NULL || pausa_menu == NULL){
        printf("Error NULL menu!\n");
        return -1;
    }
    

    // CALLBACKS DE OPCIONES DE MENU MAIN
    menu_set_option(principal_menu, 0, "JUGAR", main_game_start);
    menu_set_option(principal_menu, 1, "COMO JUGAR", how_to_play );
    menu_set_option(principal_menu, 2, "SALIR", menu_force_close_current);

	// CALLBACKS DE OPCIONES DEL MENU DE PAUSA
	menu_set_option(pausa_menu, 0, "REANUDAR", resume_game);
    menu_set_option(pausa_menu, 1, "REINICIAR", restart_game);
    menu_set_option(pausa_menu, 2, "SALIR", exit_game);



    menu_set_event_listener_display(read_events, display_menu_display);
    game_set_delrow_callback(animation_row_compleate);
   
    printf("Run\n");
    menu_run(principal_menu);
   //main_game_start();
    
    end_program();//borro todo

    return 0;

}

void main_game_start(void){

    game_data_t game_data;
    uint64_t lastMillis;

    game_start(); // iniciar el juego
    initialize_display_game();

    while ((game_data = game_get_data()).state != GAME_QUIT)
    {
        read_events(); // Leer teclado y eventos (Allegro)
        
        if(game_data.state == GAME_RUN && easytimer_get_millis()-lastMillis >= game_data.speed_interval){
            game_move_down();
            game_run();
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
    if(row_compleate[0])
    {
        int i;
        int z;
        float reductor;
        float angulo;
        float decremento= 0.1;
        int contador_filas_destruidas;
        int indicador;

        for(reductor=2.1, angulo=0, indicador=0; reductor>=0; angulo+=(3.1415/8)){
    
            for(z=1; z<=ANCHO; z++){

                for( i=0, contador_filas_destruidas=0; row_compleate[i] != 0 && i< WIDTH ; i++){
                    al_draw_scaled_bitmap(image, 0, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image), BLOCKSZ*z, BLOCKSZ*(row_compleate[i]), BLOCKSZ, BLOCKSZ, 0);
                //pongo el fondo en negro
                    al_draw_tinted_scaled_rotated_bitmap(pieza_blanca,  al_map_rgba_f(1, 1, 1, 1), al_get_bitmap_width(pieza_blanca)/2, al_get_bitmap_height(pieza_blanca)/2, (BLOCKSZ/2 +BLOCKSZ*z), (BLOCKSZ/2 +BLOCKSZ*(row_compleate[i])),reductor, reductor, angulo, 0);
                    //se va haciendo mas chia a medida que rota
                    al_flip_display();
                    easytimer_delay(5);
                    contador_filas_destruidas++; //incremento contador
                    if(contador_filas_destruidas==4 && !indicador){
                        al_draw_text(text_font_pointer_fetcher(),al_map_rgb(0,120,120), BLOCKSZ*6,BLOCKSZ*4,CENTRADO,"T E T R I S !");
                        al_flip_display();
                        indicador++;
                        }
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
}

// HACER !
void display_menu_display(void){

    menu_t menu_data = menu_get_current_menu_data();
    uint8_t id;
    al_clear_to_color(al_map_rgb(0,0,0));   //fondo negro
    al_draw_scaled_bitmap(tetris_cartel, 0, 0, al_get_bitmap_width(tetris_cartel), al_get_bitmap_height(tetris_cartel),BLOCKSZ*3, BLOCKSZ, al_get_display_width(display)-BLOCKSZ*6, BLOCKSZ*8, 0);
    
    blocktext_t * menuprin = text_init_alleg(al_map_rgb(0,0,0), al_map_rgb(255,255,255), 45, menu_data.title, PATH_TTF, al_get_display_width(display)/2, al_get_display_height(display)/2+BLOCKSZ*2, CENTRADO );
    if(text_global_font_changer(menuprin))
    {
        printf("error con text_global_font_changer");
    }
    
    text_drawer(menuprin);

for(id=0; id<menu_data.n_options; id++)
	{
        if(menu_data.current_option == id){
            blocktext_t * menuop = text_init_alleg(al_map_rgb(0,0,0), al_map_rgb(0,255,0), 30, menu_data.option_titles[id], PATH_TTF, al_get_display_width(display)/2,(al_get_display_height(display)/2)+(BLOCKSZ*(4+id)), CENTRADO );
            text_drawer(menuop);
            text_destroy(menuop);
        }// si es la que esta siendo apuntada, la dibujo en verde

        else{
            blocktext_t * menuop = text_init_alleg(al_map_rgb(0,0,0), al_map_rgb(255,255,255), 30, menu_data.option_titles[id], PATH_TTF, al_get_display_width(display)/2, (al_get_display_height(display)/2)+(BLOCKSZ*(4+id)), CENTRADO );
            text_drawer(menuop);
            text_destroy(menuop);
        }//si no, en blanco
        al_flip_display();
    }   //imprimo cada opcion del menu en pantalla una abajo de la otra
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
    printf("se mantiene presionada\n");
        return;
    }
    if(menu_is_current_available()){
switch (key)
        {
            case KEYB_UP:
                menu_go_up();
                printf("menu UP\n");
                break;

            case KEYB_DOWN:
                menu_go_down();
                printf("menu DOWN\n");
                break;

            case KEYB_LEFT:
                menu_go_back();
                printf("menu LEFT\n");
                break;

            case KEYB_SPACE:
                menu_go_select();
                printf("menu BTN\n");
                break;

            case KEYB_ENTER:
                menu_go_select();
                printf("menu BTN\n");
                break;

            default:
                break;
        }
            menu_t data = menu_get_current_menu_data();
            printf("menu: %s, option: %u %s\n", data.title, data.current_option, data.option_titles[data.current_option]);
    }
    else if(game_get_data().state == GAME_RUN){

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

        /*case KEYB_SPACE:
            game_start();
            printf("BTN\n");
            break;*/

        case KEYB_ESC:
            menu_run(pausa_menu);
            printf("Juego Pausado\n");
            break;
            

        default:
        break;
    }
    game_run();
    update_display(); // estamos viendo si esto buguea la eliminacion de filas
    }
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
            al_draw_scaled_bitmap(image, (al_get_bitmap_width(image)/8) * val, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image),BLOCKSZ*(ANCHO+3+x), BLOCKSZ*(y+3), BLOCKSZ, BLOCKSZ, 0);
            
        }
    }//DIBUJO PIEZA SIGUIENTE

    text_score_drawer(score, game_get_data().score);
	al_flip_display(); //despues de esribir toda la matriz muestro lo que escribi
	printf("SCORE:\n%u\n", game_get_data().score);
    text_score_drawer(nivel, game_get_data().game_level);

}


int initialize_alleg(void) {
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
    tetris_cartel = al_load_bitmap("./frontend/images/tetris_cartel.png");
    if (!tetris_cartel) {
        printf("failed to load tetris_cartel !\n");
        al_destroy_bitmap(image);
        al_destroy_bitmap(muroH);
        al_destroy_bitmap(muroV);
        al_destroy_bitmap(pieza_blanca);
        al_destroy_event_queue(event_queue);
        return -1;
    }
    diagrama_teclado = al_load_bitmap("./frontend/images/howtoplay3.png");
    if (!tetris_cartel) {
        printf("failed to load diagrama_cartel !\n");
        al_destroy_bitmap(image);
        al_destroy_bitmap(muroH);
        al_destroy_bitmap(muroV);
        al_destroy_bitmap(pieza_blanca);
        al_destroy_bitmap(tetris_cartel);
        al_destroy_event_queue(event_queue);
        return -1;
    }

    display = al_create_display(((ANCHO+8)*BLOCKSZ), ((ALTO+1)*BLOCKSZ));
    if (!display) {
        al_destroy_bitmap(image);
        al_destroy_bitmap(muroH);
        al_destroy_bitmap(muroV);
        al_destroy_bitmap(pieza_blanca);
        al_destroy_bitmap(diagrama_teclado);
        al_destroy_bitmap(tetris_cartel);
        al_destroy_event_queue(event_queue);
        fprintf(stderr,"failed to create display!\n");
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    return 0;
}
    
void  initialize_display_game (void){
    blocktext_t * pieza_sig = text_init_alleg(al_map_rgb(0,0,0), al_map_rgb(255,255,255), 25, "PIEZA  SIGUIENTE", PATH_TTF, BLOCKSZ*(ANCHO+2.5), BLOCKSZ, ALINEADO_IZQUIERDA );
    blocktext_t * puntaje = text_init_alleg(al_map_rgb(0,0,0), al_map_rgb(255,255,255), 30, "PUNTAJE:", PATH_TTF, BLOCKSZ*(ANCHO+2.5), BLOCKSZ*7, ALINEADO_IZQUIERDA );
    blocktext_t * lvl_game = text_init_alleg(al_map_rgb(0,0,0), al_map_rgb(255,255,255), 30, "NIVEL:", PATH_TTF, BLOCKSZ*(ANCHO+2.5), BLOCKSZ*11, ALINEADO_IZQUIERDA );
    
    score = text_init_alleg(al_map_rgb(0,0,0), al_map_rgb(255,255,255), 30, "", PATH_TTF, BLOCKSZ*(ANCHO+2.5), BLOCKSZ*8, ALINEADO_IZQUIERDA );
    nivel = text_init_alleg(al_map_rgb(0,0,0), al_map_rgb(255,255,255), 30, "", PATH_TTF, BLOCKSZ*(ANCHO+2.5), BLOCKSZ*12, ALINEADO_IZQUIERDA );

    if(puntaje==NULL || pieza_sig==NULL || lvl_game==NULL){
        printf("problema con titulo de initialize_display_game");   
    }

    if(text_global_font_changer(pieza_sig))
    {
        printf("error con text_global_font_changer");
    }

    al_clear_to_color(al_map_rgb(0,0,0));
    text_drawer(pieza_sig);

    if(text_global_font_changer(puntaje))
    {
        printf("error con text_global_font_changer");
    }
    text_drawer(puntaje);
    text_drawer(lvl_game);
    text_score_drawer(nivel, game_get_data().game_level);
    
   
    //ahora dibujo el muro horizontal
    al_draw_scaled_bitmap(muroH, 0, 0, al_get_bitmap_width(muroH), al_get_bitmap_height(muroH), 0, BLOCKSZ*(ALTO), ((ANCHO+2)*BLOCKSZ), BLOCKSZ, 0);
    //empiezo uso la ultima fila
    //dibujo las paredes
    al_draw_scaled_bitmap(muroV, 0, 0, al_get_bitmap_width(muroV), al_get_bitmap_height(muroV), 0, 0, BLOCKSZ, al_get_display_height(display)-BLOCKSZ, 0);
    al_draw_scaled_bitmap(muroV, 0, 0, al_get_bitmap_width(muroV), al_get_bitmap_height(muroV),((ANCHO+1)*BLOCKSZ) , 0, BLOCKSZ, al_get_display_height(display)-BLOCKSZ, 0);
        
    
    al_flip_display();
    
    //playAudio();
    text_destroy(pieza_sig);
    text_destroy(puntaje);      //libero la memoria dinamica

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

// DESTRUCCION DE BITMAPS
    al_destroy_bitmap(image);
    al_destroy_bitmap(muroH);
    al_destroy_bitmap(muroV);
    al_destroy_bitmap(pieza_blanca);    
    al_destroy_bitmap(tetris_cartel);
    al_destroy_bitmap(diagrama_teclado);

// DESTRUCCION DE UTILIDADES ALLEGRO
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_uninstall_keyboard();

//DESTRUCCION DE UTILIDADES EXTRA
    menu_destroy(principal_menu);
    menu_destroy(pausa_menu);
    text_destroy(nivel);
    text_destroy(score);
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

