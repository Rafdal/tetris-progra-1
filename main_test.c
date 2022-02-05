#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#include "./backend/game.h"
#include "./backend/menu.h"
#include "./testing/joystick.h"
#include "./testing/easy_timer.h"
#include "./testing/matrix_handler.h"
#include "./testing/rpi_display.h"
#include "./testing/rpi_text.h"
#include "./frontend/keyboard.h"

// LIBRERIAS ALEGRO
#include <allegro5/allegro.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_image.h> //NO OLVIDAR INCLUIR ALLEGRO_IMAGE EN LINKER


// DEBUG
#define USAR_DEBUG
#include "./debug/debug.h"


#define STR_SIZES 64

// ******************************
// *	P R O T O T I P O S		*
// ******************************
void main_game_start(void);	// Inicializa el juego

void update_game_display(void); // Actualiza la matriz del juego

void destroy_text (void); //Destruye los bloques de textos deslizantes

void run_display_effects(void);	// Ejecuta los efectos del display de la RPI

void update_menu_display(void);	//Actualiza el display del menu

void animation_row_compleate (void);  //Ejecuta la animacion de fila completa

void key_press_callback(uint8_t key); // Define los Callbacks de las teclas

// int init_audio(char); //Inicializa el audio

_Noreturn void * animation_deleate_row(); //Elimina las filas completas de forma animada

_Noreturn void * animation_text (); //Muestra un texto en pantalla

void update_game_animation(uint8_t x_init, uint8_t y_init);

void animation_game_finish(void);





// **************************************
// *	 V A R S . G L O B A L E S		*
// **************************************
menu_t *main_menu = NULL;
menu_t *pause_menu = NULL;

rpi_text_block_t* text_stat = NULL; // Puntero para los bloques de texto estaticos
rpi_text_block_t* text_anim = NULL; // Puntero para texto deslizante con anumacion

static uint8_t game_level = 1;

// ******************************
// *	C A L L B A C K S		*
// ******************************
//CALLBACK DE EXIT GAME
void exit_game(void){
    game_quit();                  // Finalizar juego
    menu_force_close(pause_menu); // Cerrar menu pausa
}

//CALLBACK DE REINICIO DE JUEGO
void restart_game(void){
	rpi_clear_display(); //limpio el display
    menu_force_close(pause_menu); // Cerrar menu pausa
	game_init();	//Inicio el juego
    game_start(); 	//Corre el juego
}

//CALLBACK DE RENAUDAR JUEGO
void resume_game(void)
{
	rpi_clear_display(); 	//Limpia el display
	menu_force_close(pause_menu); //Cierra el menu de pausa
	game_run();	//Corre el juego
}


// ******************
// *	M A I N		*
// ******************
int main(void){
    printf("Inicializando...\n");

    rpi_init_display(); //Inicializo el display

    #ifdef USAR_DEBUG
    debug_new_file("debug.txt");
    #endif

    dpad_init();	//Inicializo el pad (joystick usado como pad direccional de 4 botones)

    game_init();	//Inicializa el juego
    easytimer_set_realTimeLoop(dpad_read);

	// init_audio(1);  //Inicializo el audio


	//Defino los callback for longpress
    dpad_on_press(key_press_callback);
    dpad_use_press_callback_for_longpress(DPAD_DOWN);
    dpad_use_press_callback_for_longpress(DPAD_LEFT);
    dpad_use_press_callback_for_longpress(DPAD_RIGHT);

	//Inicializo los menu
    main_menu = menu_init(3, "MENU", NULL, MENU_ACTION_DO_NOTHING);
    pause_menu = menu_init(3, "PAUSA", NULL, MENU_ACTION_JUST_EXIT);

    if(main_menu == NULL || pause_menu == NULL){
        printf("Error NULL menu!\n");
        return -1;
    }

	// CALLBACKS DE OPCIONES DE MENU MAIN
    menu_set_option(main_menu, 0, "JUGAR", main_game_start);
    menu_set_option(main_menu, 1, "Prueba2", NULL);
    menu_set_option(main_menu, 2, "SALIR", menu_force_close_current);

	// CALLBACKS DE OPCIONES DEL MENU DE PAUSA
	menu_set_option(pause_menu, 0, "REANUDAR", resume_game);
    menu_set_option(pause_menu, 1, "REINICIAR", restart_game);
    menu_set_option(pause_menu, 2, "SALIR", exit_game);

	// RESERVA DE MEMORIA PARA EL BLOQUE DE TEXTOS DESLIZANTES Y LOS ESTATICOS
    text_stat = rpi_text_reserve(STR_SIZES);
    text_anim = rpi_text_reserve(STR_SIZES);

    // Setear los callbacks que controlaran el menu
    menu_set_event_listener_display(dpad_read, update_menu_display);
    menu_set_animation_callback(run_display_effects);

	//Setear callback de animacion de eliminar fila
	game_set_delrow_callback(animation_row_compleate);

    // Ejecutar menu principal
    DEBUG("Running main menu...");
    menu_run(main_menu);
    DEBUG("Exit main menu...");


	// **************************
	// *	D E S T R O Y		*
	// **************************

    // Liberar memoria usada por los menues
    menu_destroy(main_menu);
    menu_destroy(pause_menu);

	// Destruye el audio
	// init_audio(0);
	// Destruye los bloques de textos deslizantes
    printf("Lol\n");
    rpi_text_destroy(text_anim);
    rpi_text_destroy(text_stat);
    printf("boe\n");

    return 0;
}

// ******************************
// *	F U N C I O N E S		*
// ******************************
void main_game_start(void){

	game_init();	//Inicio el juego
	rpi_clear_display(); //Limpio el display

    game_data_t game_data;
    uint64_t lastMillis;

    game_start(); //Empiezo el juego

    while ((game_data = game_get_data()).state != GAME_QUIT)
    {
        dpad_read(); //Leo el pad

		//Funcion para que la pieza baje sola
        if(game_data.state == GAME_RUN && easytimer_get_millis()-lastMillis >= game_data.speed_interval){
            game_move_down();
            game_run();	//Corro el juego
			//animation_row_compleate();	//Analizo si se completo una fila y corro lo animacion
            update_game_display();  //Actualizo el display

            lastMillis = easytimer_get_millis();
        }

        if(game_data.state == GAME_LOSE){
			animation_game_finish();
            game_data.state = GAME_QUIT;
        }
    }
    printf("Leaving game...\n");
}

void update_game_display(void){

	game_data_t game_data = game_get_data();

    rpi_text_set_offset(text_stat,11,12,0,0);

	char buffer[32];
	sprintf(buffer, "%d", game_data.game_level);
	rpi_text_set(buffer, text_stat);
	rpi_text_print(text_stat);

	// Actualizo la matriz del juego y la cargo en la matriz a imprimir
	matrix_hand_t mat_handler;
	assert(mat_init(&mat_handler, HEIGHT, WIDTH));
	MAT_COPY_FROM_2D_ARRAY(&mat_handler, game_public_matrix, HEIGHT, WIDTH);
	rpi_copyToDis(&mat_handler, 0, 0);

	//Actualizo la matriz de la pieza siguiente y la cargo en la matriz a imprimir
	matrix_hand_t public_next_mat;
	assert(mat_init(&public_next_mat, 11, 4));
	MAT_COPY_FROM_2D_ARRAY(&public_next_mat, next_block_public_matrix, 11,4);
	rpi_copyToDis(&public_next_mat, 0, 11);

	rpi_run_display(); //Actualizo el display

	printf("SCORE: %u\n", game_get_data().score);
	printf("LEVEL: %d\n", game_get_data().game_level);
}

void update_game_animation(uint8_t x_init, uint8_t y_init)
{
	// Actualizo la matriz del juego y la cargo en la matriz a imprimir
	matrix_hand_t mat_handler;
	assert(mat_init(&mat_handler, HEIGHT, WIDTH));
	MAT_COPY_FROM_2D_ARRAY(&mat_handler, game_public_matrix, HEIGHT, WIDTH);
	rpi_copyToDis_area(&mat_handler, x_init, 0, y_init, 0);

	//Actualizo la matriz de la pieza siguiente y la cargo en la matriz a imprimir
	matrix_hand_t public_next_mat;
	assert(mat_init(&public_next_mat, 12, 4));
	MAT_COPY_FROM_2D_ARRAY(&public_next_mat, next_block_public_matrix, 12,4);
	rpi_copyToDis_area(&public_next_mat, x_init, 11, y_init, 0);

	rpi_run_display(); //Actualizo el display
}

void run_display_effects(void)
{
    menu_t menu_data = menu_get_current_menu_data();
    uint8_t id;

    for(id=0; id<menu_data.n_options; id++){
        if(menu_data.current_option == id)
		{
            rpi_text_run(text_anim);	// Deslizar el texto de la opcion focuseada
        }
    }
}

void update_menu_display(void)
{
    menu_t menu_data = menu_get_current_menu_data();
    uint8_t id;

	rpi_clear_display();

    for(id=0; id<menu_data.n_options; id++)
	{
        if(menu_data.current_option == id){
            rpi_text_set(menu_data.option_titles[id], text_anim); //Cargo los titulos en los bloques de textos
            rpi_text_set_offset(text_anim, 0 , 5*id , 0, 0); //Defino las posiciones de los bloques
            rpi_text_slide(text_anim,250);
        }
        else{
            rpi_text_set(menu_data.option_titles[id], text_stat); //Cargo los titulos en los bloques de textos
            rpi_text_set_offset(text_stat, 0 , 5*id , 0, 0); //Defino las posiciones de los bloques
            rpi_text_print(text_stat); //Imprimo las opciones
        }
    }
    rpi_run_display(); //Actualizo el display
}

void animation_row_compleate (void)
{
	if(row_compleate[0] != 0 ) //Si existe fila completa entro a la animacion
	{

		pthread_t tid1,tid2;

		pthread_create(&tid1,NULL,animation_text,NULL);
		pthread_create(&tid2,NULL,animation_deleate_row,NULL);


		pthread_join(tid1,NULL);
		pthread_join(tid2,NULL);

	}
}

_Noreturn void * animation_text ()
{
	game_data_t game_data = game_get_data();

	if (game_level != game_data.game_level)
	{
		printf("LEVEL UP\n");
		game_level = game_data.game_level;

        rpi_text_set_offset(text_anim, 0, 0, 0, 0);
		rpi_text_set("LEVEL UP", text_anim);

		rpi_clear_area(0, 0, 5, RPI_WIDTH);

		// rpi_text_slide(text_anim, 100);
		while(text_anim->state == RPI_TEXT_STATE_SLIDE)
			rpi_text_one_slide(text_anim);
	}
}

_Noreturn void * animation_deleate_row()
{
	int i, j;
	for(j=0; j < WIDTH; j++) // Me muevo por columnas
	{
		for( i=0; row_compleate[i] != 0 && i< WIDTH ; i++) //Me muevo por filas
		{
			delete_pixel(row_compleate[i], j);
		}
		easytimer_delay(100); //Delay
		rpi_run_display();
		update_game_animation(0, 5); //Actualizo el display
	}

	for(i=0; i < 4 ; i++) //Elimino las filas completas
	{
		if(row_compleate[i] != 0)
		{
			delete_row(row_compleate[i]);
			row_compleate[i]= 0; //Coloco en cero el arreglo
		}
	}
}

void key_press_callback(uint8_t key){
    printf("keypress cback\n");
    DEBUG("key_press_callback");
    if(easytimer_delay_active()){ // si el delay esta activo
        return; // no hacer nada
    }

    if(menu_is_current_available()){
        switch (key)
        {
            case DPAD_UP:
                menu_go_up();
                printf("menu UP\n");
                break;

            case DPAD_DOWN:
                menu_go_down();
                printf("menu DOWN\n");
                break;

            case DPAD_LEFT:
                menu_go_back();
                printf("menu LEFT\n");
                break;

            case DPAD_BTN:
                menu_go_select();
                printf("menu BTN\n");
                break;

            default:
                break;
        }
            // DEBUG
            // menu_t data = menu_get_current_menu_data();
            // printf("menu: %s, option: %u %s\n", data.title, data.current_option, data.option_titles[data.current_option]);
    }else if(game_get_data().state == GAME_RUN){
        switch (key)
        {
            case DPAD_UP:
                printf("game UP\n");
                break;

            case DPAD_DOWN:
                game_move_down();
                printf("game DOWN\n");
                break;

            case DPAD_LEFT:
                game_move_horizontal(0);
                printf("game LEFT\n");
                break;

            case DPAD_RIGHT:
                game_move_horizontal(1);
                printf("game RIGHT\n");
                break;

            case DPAD_UPRIGHT:
                game_rotate(1);
                printf("game UPRIGHT\n");
                break;

            case DPAD_UPLEFT:
                game_rotate(0);
                printf("game UPLEFT\n");
                break;

            case DPAD_BTN:
                menu_run(pause_menu);
                printf("game BTN\n");
                break;

            default:
                break;
        }
        game_run();
		//animation_row_compleate();
		update_game_display();
    }else{
        printf("Error state.\n");
    }
}


void animation_game_finish(void)
{
	game_data_t game_data = game_get_data();
	
	rpi_clear_display();
	char score[16];
	sprintf(score, "%d", game_data.score);
	
    rpi_text_set_offset(text_anim, 2, RPI_WIDTH, 0,0);
    rpi_text_set_offset(text_stat, 10, 0, 0,0);
	
	rpi_text_set("PERDISTE", text_anim);
	rpi_text_set(score, text_stat);

	rpi_text_slide(text_anim, 200);

	printf("STRING SIZE: %lu\n", strlen(score));

	if(strlen(score) > 3)
	{
		rpi_text_slide(text_stat, 200);
		rpi_text_set_offset(text_stat, RPI_WIDTH + 2, 10, 0, 0);

		while(text_anim->state == RPI_TEXT_STATE_SLIDE)
		{
			rpi_text_one_slide(text_anim);
			rpi_text_one_slide(text_stat);
		}
	}
	else
	{
		rpi_text_print(text_stat);
		while (text_anim->state == RPI_TEXT_STATE_SLIDE)
			rpi_text_one_slide(text_anim);
	}
}
