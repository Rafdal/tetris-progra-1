#include <stdio.h>
#include <assert.h>

#include "./backend/game.h"
#include "./backend/menu.h"
#include "./libs/joystick.h"
#include "./libs/easy_timer.h"
#include "./libs/matrix_handler.h"
#include "./libs/rpi_display.h"
#include "./libs/rpi_text.h"

//LIBRERIA AUDIO SDL2
#include <SDL2/SDL.h>
#include "./libs/audio.h"

// DEBUG
// #define USAR_DEBUG
#include "./debug/debug.h"


#define STR_SIZES 64
#define S2WAIT 180

#define MENU_AUDIO "./audios/main_title.wav"
#define PAUSE_AUDIO "./audios/pausa.wav"
#define GAME_AUDIO "./audios/tetris.wav"
#define LOSE_AUDIO "./audios/game_over.wav"
#define MOVE_AUDIO "./audios/chime.wav"

// ******************************
// *	P R O T O T I P O S		*
// ******************************
void main_game_start(void);	// Inicializa el juego

void update_game_display(void); // Actualiza la matriz del juego

void destroy_text (void); //Destruye los bloques de textos deslizantes

void run_menu_effects(void); // Ejecuta los efectos del display de la RPI

void update_menu_display(void);	//Actualiza el display del menu

void animation_row_complete (void);  //Ejecuta la animacion de fila completa

void key_press_callback(uint8_t key); // Define los Callbacks de las teclas

void update_game_animation(uint8_t x_init, uint8_t y_init);

void animation_game_finish(void);

void animation_game_start (void);

void exit_game(void);

void restart_game(void);

void resume_game(void);



// **************************************
// *	 V A R S . G L O B A L E S		*
// **************************************
menu_t *main_menu = NULL;
menu_t *pause_menu = NULL;

rpi_text_block_t* text_stat = NULL; // Puntero para los bloques de texto estaticos
rpi_text_block_t* text_anim = NULL; // Puntero para texto deslizante con anumacion

static uint8_t last_game_level = 1;

static uint8_t line[16][1]={{1},{1},{1},{1}, {1},{1},{1},{1} , {1},{1},{1},{1} , {1},{1},{1},{1}};



// ******************
// *	M A I N		*
// ******************
int main(void){
    printf("Inicializando...\n");

    rpi_init_display(); //Inicializo el display

	// RESERVA DE MEMORIA PARA EL BLOQUE DE TEXTOS DESLIZANTES Y LOS ESTATICOS
    text_stat = rpi_text_reserve(STR_SIZES);
    text_anim = rpi_text_reserve(STR_SIZES);


	dpad_init();	//Inicializo el pad (joystick usado como pad direccional de 4 botones)
	initAudio();
	easytimer_set_realTimeLoop(dpad_read);


    #ifdef USAR_DEBUG
    debug_new_file("debug.txt");
    #endif


	//Defino los callback for longpress
    dpad_on_press(key_press_callback);
    dpad_use_press_callback_for_longpress(DPAD_DOWN);
    dpad_use_press_callback_for_longpress(DPAD_LEFT);
    dpad_use_press_callback_for_longpress(DPAD_RIGHT);

	//Inicializo los menu
    main_menu = menu_init(2, "MENU", NULL, MENU_ACTION_DO_NOTHING);
    pause_menu = menu_init(3, "PAUSA", NULL, MENU_ACTION_JUST_EXIT);

    if(main_menu == NULL || pause_menu == NULL){
        printf("Error NULL menu!\n");
        return -1;
    }

	// CALLBACKS DE OPCIONES DE MENU MAIN
    menu_set_option(main_menu, 0, "PLAY", main_game_start);
    menu_set_option(main_menu, 1, "EXIT", menu_force_close_current);

	// CALLBACKS DE OPCIONES DEL MENU DE PAUSA
	menu_set_option(pause_menu, 0, "RESUME", resume_game);
    menu_set_option(pause_menu, 1, "RESTART", restart_game);
    menu_set_option(pause_menu, 2, "EXIT", exit_game);


    // Setear los callbacks que controlaran el menu
    menu_set_event_listener_display(dpad_read, update_menu_display);
    menu_set_animation_callback(run_menu_effects);

	//Setear callback de animacion de eliminar fila
	game_set_delrow_callback(animation_row_complete);

	playMusic(MENU_AUDIO, SDL_MIX_MAXVOLUME);
	animation_game_start();

    // Ejecutar menu principal
    menu_run(main_menu);

	// **************************
	// *	D E S T R O Y		*
	// **************************

    // Liberar memoria usada por los menues
    menu_destroy(main_menu);
    menu_destroy(pause_menu);

	// Destruye los bloques de textos deslizantes
	rpi_text_destroy(text_anim);
    rpi_text_destroy(text_stat);

	//Finalizo el sistema de audio
	endAudio();

	rpi_clear_display();

    return 0;
}


// **************************************
// *	M E N U    C A L L B A C K S	*
// **************************************
//CALLBACK DE EXIT GAME
void exit_game(void){
    game_quit();                  // Finalizar juego
    menu_force_close(pause_menu); // Cerrar menu pausa
}

//CALLBACK DE REINICIO DE JUEGO
void restart_game(void){
	rpi_clear_display(); //limpio el display
    menu_force_close(pause_menu); // Cerrar menu pausa
    game_start(); 	//Corre el juego
}

//CALLBACK DE RENAUDAR JUEGO
void resume_game(void)
{
	rpi_clear_display(); 	//Limpia el display
	menu_force_close(pause_menu); //Cierra el menu de pausa
	game_run();	//Corre el juego
}

void run_menu_effects(void)
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



void key_press_callback(uint8_t key){
    DEBUG("key_press_callback");
    if(easytimer_delay_active()){ // si el delay esta activo
        return; // no hacer nada
    }

    if(menu_is_current_available()){
        switch (key)
        {
            case DPAD_UP:
				playSound(MOVE_AUDIO, SDL_MIX_MAXVOLUME / 2);
				menu_go_up();
                printf("menu UP\n");
                break;

            case DPAD_DOWN:
				playSound(MOVE_AUDIO, SDL_MIX_MAXVOLUME / 2);
				menu_go_down();
                printf("menu DOWN\n");
                break;

            case DPAD_LEFT:
                menu_go_back();
                printf("menu LEFT\n");
                break;

            case DPAD_BTN:
				pauseAudio();
				menu_go_select();
                printf("menu BTN\n");
                break;

            default:
                break;
        }


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
                easytimer_delay(200); // Delay para evitar salir del menu al entrar
				rpi_clear_display();
				printf("Pause menu status: %d\n", musicStatus());

				pauseAudio();
				printf("Pause menu status: %d\n", musicStatus());

				playMusic(PAUSE_AUDIO, SDL_MIX_MAXVOLUME);
				printf("Pause menu status: %d\n", musicStatus());


				menu_run(pause_menu);
				rpi_clear_display();
                printf("game BTN\n");
                break;

            default:
                break;
        }
        game_run();
		update_game_display();
    }else{
        printf("Error state.\n");
    }
}


// ***********************************************
// *	F U N C I O N E S	 D E L    J U E G O	 *
// ***********************************************
void main_game_start(void){
	printf("Menu status: %d\n", musicStatus());

	playMusic(GAME_AUDIO, SDL_MIX_MAXVOLUME);
	printf("New song/ Menu status: %d\n", musicStatus());


	rpi_clear_display(); //Limpio el display
	playMusic(GAME_AUDIO, SDL_MIX_MAXVOLUME); //Reproduzco el audio del juego

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
            update_game_display();  //Actualizo el display

            lastMillis = easytimer_get_millis();
        }

        if(game_data.state == GAME_LOSE){
			pauseAudio();
			playMusic(LOSE_AUDIO, SDL_MIX_MAXVOLUME);

			animation_game_finish();
			rpi_clear_display();
			game_quit();
        }
    }
    printf("Leaving game...\n");
	rpi_clear_display();
}

void update_game_display(void){
	game_data_t game_data = game_get_data();

	// Actualizo la matriz del juego y la cargo en la matriz a imprimir
	matrix_hand_t mat_handler;
	assert(mat_init(&mat_handler, GAME_HEIGHT, GAME_WIDTH));
	MAT_COPY_FROM_2D_ARRAY(&mat_handler, game_public_matrix, GAME_HEIGHT, GAME_WIDTH);
	rpi_copyToDis(&mat_handler, 0, 0);

	//Actualizo la matriz de la pieza siguiente y la cargo en la matriz a imprimir
	matrix_hand_t public_next_mat;
	assert(mat_init(&public_next_mat, 10, 5));
	MAT_COPY_FROM_2D_ARRAY(&public_next_mat, game_next_block_public_matrix, 10,5);
	rpi_copyToDis(&public_next_mat, 0, 11);

	//Cargo la linea divisora
	matrix_hand_t divisor_line;
	assert(mat_init(&divisor_line, 16, 1));
	MAT_COPY_FROM_2D_ARRAY(&divisor_line, line, 16, 1);
	rpi_copyToDis(&divisor_line, 0, 10);

    // MOSTRAR SCORE
    rpi_text_set_offset(text_stat,12,10,0,0);
	char buffer[32];
	sprintf(buffer, "%d", game_data.game_level);
	rpi_text_set(buffer, text_stat);
	rpi_text_print(text_stat);

	rpi_run_display(); //Actualizo el display

	printf("SCORE: %u\n", game_get_data().score);
	printf("LEVEL: %d\n", game_get_data().game_level);
}

// (Para la animacion de borrar filas)
void update_game_animation(uint8_t x_init, uint8_t y_init)
{
	// Actualizo la matriz del juego y la cargo en la matriz a imprimir
	matrix_hand_t mat_handler;
	assert(mat_init(&mat_handler, GAME_HEIGHT, GAME_WIDTH));
	MAT_COPY_FROM_2D_ARRAY(&mat_handler, game_public_matrix, GAME_HEIGHT, GAME_WIDTH);
	rpi_copyToDis_area(&mat_handler, x_init, 0, y_init, 0);

	//Actualizo la matriz de la pieza siguiente y la cargo en la matriz a imprimir
	matrix_hand_t public_next_mat;
	assert(mat_init(&public_next_mat, 12, 4));
	MAT_COPY_FROM_2D_ARRAY(&public_next_mat, game_next_block_public_matrix, 12,4);
	rpi_copyToDis_area(&public_next_mat, x_init, 11, y_init, 0);

	rpi_run_display(); //Actualizo el display
}



void animation_row_complete (void)
{
	if(game_row_complete[0] != 0 ) //Si existe fila completa entro a la animacion
	{
		game_data_t game_data = game_get_data();

		char level_string[32];
		bool level_up = false;
		if (last_game_level != game_data.game_level)
		{
			level_up = true;
			last_game_level = game_data.game_level;
			sprintf(level_string, "LEVEL %d", last_game_level);

			rpi_text_set_offset(text_anim, 0, 0, 0, 0);
			rpi_text_slide(text_anim, 20);  // NO TOCAR
			rpi_text_set(level_string, text_anim);

			rpi_clear_area(0, 0, 5, RPI_WIDTH); // Limpio el area donde se muestra el texto
		}
		uint64_t lastMs;
		// rpi_text_slide(text_anim, 100);
		int i,j=0;
		while(j<GAME_WIDTH){
			if(level_up)
				rpi_text_one_slide(text_anim);

			if(easytimer_get_millis()-lastMs > 100){
				if(j<GAME_WIDTH){
					for(i=0; game_row_complete[i] != 0 && i < 4 ; i++) //Me muevo por filas
						delete_pixel(game_row_complete[i], j);
					
					update_game_animation(0, 5); //Actualizo el display
					rpi_run_display();

					j++;
				}
				lastMs = easytimer_get_millis();
			}
		}
		for(i=0; i < 4 ; i++) //Elimino las filas completas
		{
			if(game_row_complete[i] != 0)
			{
				delete_row(game_row_complete[i]);
				game_row_complete[i]= 0; //Coloco en cero el arreglo
			}
		}

	}
}



void animation_game_finish(void)
{
	game_data_t game_data = game_get_data();
	
	rpi_clear_display();
	char score[16];
	sprintf(score, "%d", game_data.score);
	
    rpi_text_set_offset(text_anim, RPI_WIDTH/2, 2, 0,0);

	rpi_text_set("PERDISTE", text_anim);
	rpi_text_set(score, text_stat);

	rpi_text_slide(text_anim, 200);

	if(strlen(score) > 3)
	{
		rpi_text_slide(text_stat, 200);
		rpi_text_set_offset(text_stat,RPI_WIDTH, 10, 0, 0);

		while(text_anim->state == RPI_TEXT_STATE_SLIDE)
		{
			rpi_text_one_slide(text_anim);
			rpi_text_one_slide(text_stat);
		}
	}
	else
	{
		rpi_text_set_offset(text_stat, 1, 10, 0,0);
		rpi_text_print(text_stat);
        rpi_text_slide(text_anim,100);
		while (text_anim->state == RPI_TEXT_STATE_SLIDE)
			rpi_text_one_slide(text_anim);
	}
}

void animation_game_start (void)
{
	rpi_clear_display();

	rpi_text_set("TETRIS", text_anim);
	rpi_text_slide(text_anim, 100);
	rpi_text_set_offset(text_anim, RPI_WIDTH, RPI_TEXT_SPACING + 1, 0, 0);

	while (text_anim->state == RPI_TEXT_STATE_SLIDE)
	{
		rpi_text_one_slide(text_anim);
	}
	rpi_clear_display();
	easytimer_delay(500);
}
