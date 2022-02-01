#include <stdio.h>
#include <assert.h>

#include "./backend/game.h"
#include "./backend/menu.h"
#include "./testing/joystick.h"
#include "./testing/easy_timer.h"
#include "./testing/matrix_handler.h"
#include "./testing/rpi_display.h"
#include "./testing/rpi_text.h"

///////////////////////LIBRERIAS DE SANTI's SANDBOX//////////////////////
#include <allegro5/allegro.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_image.h> //NO OLVIDAR INCLUIR ALLEGRO_IMAGE EN LINKER
#include <allegro5/allegro_audio.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_acodec.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include "./frontend/keyboard.h"
//////////////////////////////////////////////////////////////////////////

void key_press_callback(uint8_t key);
void update_game_display(void);
void main_game_start(void);
void update_menu_display(void);
void run_display_effects(void);
void animation_row_compleate (void);
int init_audio(char); //FUNCION PERTENECIENTE A SANTI's SANDBOX
void destroy_text (void);


menu_t *main_menu = NULL;
menu_t *pause_menu = NULL;

rpi_text_block_t* text[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

void exit_game(void){
    game_quit();                // Finalizar juego
    menu_force_close(pause_menu); // Cerrar menu pausa
}

void restart_game(void){
    menu_force_close(pause_menu); // Cerrar menu pausa
	game_init();
	rpi_clear_display();
    game_start();
}

void resume_game(void)
{
	menu_force_close_current();
	rpi_clear_display();
	game_run();
}

int main(void){
    printf("Inicializando...\n");

    rpi_init_display();

    game_init();
    easytimer_set_realTimeLoop(dpad_read);

	init_audio(1);  // FUNCION PERTENECIENTE A SANTI's SANDBOX

    dpad_init();
    dpad_on_press(key_press_callback);
    dpad_use_press_callback_for_longpress(DPAD_DOWN);
    dpad_use_press_callback_for_longpress(DPAD_LEFT);
    dpad_use_press_callback_for_longpress(DPAD_RIGHT);

    main_menu = menu_init(3, "MENU", NULL, MENU_ACTION_DO_NOTHING);
    pause_menu = menu_init(3, "PAUSA", NULL, MENU_ACTION_JUST_EXIT);


    if(main_menu == NULL || pause_menu == NULL){
        printf("Error NULL menu!\n");
        return -1;
    }
    menu_set_option(main_menu, 0, "JUGAR", main_game_start);
    menu_set_option(main_menu, 1, "Prueba2", NULL);
    menu_set_option(main_menu, 2, "SALIR", menu_force_close_current);

	text[0] = rpi_text_create(16, 0, 0);
    text[1] = rpi_text_create(16, 0, 0);
    text[2] = rpi_text_create(16, 0, 0);


	menu_set_option(pause_menu, 0, "REANUDAR", resume_game);
    menu_set_option(pause_menu, 1, "REINICIAR", restart_game);
    menu_set_option(pause_menu, 2, "SALIR", exit_game);

	text[3] = rpi_text_create(16, 0, 0);
	text[4] = rpi_text_create(16, 0, 0);
	text[5] = rpi_text_create(16, 0, 0);


    // Setear los callbacks que controlaran el menu
    menu_set_event_listener_display(dpad_read, update_menu_display);
    menu_set_animation_callback(run_display_effects);

    // Ejecutar menu principal
    menu_run(main_menu);

    // Liberar memoria usada por los menues
    menu_destroy(main_menu);
    menu_destroy(pause_menu);
    
	init_audio(0); // wtf?

    destroy_text();

    rpi_clear_display();

    return 0;
}

void destroy_text (void)
{
	menu_t menu_data = menu_get_current_menu_data();
//Limpia los textos del menu
	int i;
	for (i=0; i< 6 ; i++)
	{
		rpi_text_destroy(text[i]);
	}
}

void run_display_effects(void){
    menu_t menu_data = menu_get_current_menu_data();
    uint8_t id;
    for(id=0; id<menu_data.n_options; id++){
        if(menu_data.current_option == id){
            rpi_text_slide(text[id], 600); //La opcion seleccionada la coloca en moviemiento
            rpi_text_run(text[id]);	//Luego imprime la opcion
        }
		rpi_text_print(text[id]);
    }
}

void update_menu_display(void){
    menu_t menu_data = menu_get_current_menu_data();

    uint8_t id;
    for(id=0; id<menu_data.n_options; id++){
		rpi_text_parse(menu_data.option_titles[id], text[id]) ;
		set_offset(text[id], 0 , 5*id , 0, 0);
        if(text[id] != NULL){
            rpi_text_print(text[id]);
        }
    }

    /* printf("\n\n\n\n\n\n\n");
    printf("%s",menu_data.title);
    putchar('\n');*/
    rpi_run_display();
}

void main_game_start(void){

	game_init();
	rpi_clear_display();

    game_data_t game_data;
    uint64_t lastMillis;

    game_start();

    while ((game_data = game_get_data()).state != GAME_QUIT)
    {
        dpad_read();
        
        if(game_data.state == GAME_RUN && easytimer_get_millis()-lastMillis >= game_data.speed_interval){
            game_move_down();
            game_run();
			animation_row_compleate();
            update_game_display();
            lastMillis = easytimer_get_millis();
        }

        if(game_data.state == GAME_LOSE){
            printf("Perdiste! The Game\n");
            break;
            #warning BREAK HARDCODEADO
        }
    }
    printf("Leaving game...\n");
}

void animation_row_compleate (void)
{
	//PAUSO EL JUEGO
	if(row_compleate[0] != 0)
	{
		int i, j;
		for(j=0; j < WIDTH; j++)
		{
			for( i=0; row_compleate[i] != 0 && i< WIDTH ; i++)
			{
				delete_pixel(row_compleate[i], j);
			}

			easytimer_delay(20);
			update_game_display();
		}
		for(i=0; i < 4 ; i++)
		{
			delete_row(row_compleate[i]);
			row_compleate[i]= 0;
		}
	}
}

void update_game_display(void){
    matrix_hand_t mat_handler;
    assert(mat_init(&mat_handler, HEIGHT, WIDTH));
    MAT_COPY_FROM_2D_ARRAY(&mat_handler, game_public_matrix, HEIGHT, WIDTH);
    rpi_copyToDis(&mat_handler, 0, 0);

	matrix_hand_t public_next_mat;
	assert(mat_init(&public_next_mat, 12, 4));
	MAT_COPY_FROM_2D_ARRAY(&public_next_mat, next_block_public_matrix, 12,4);
	rpi_copyToDis(&public_next_mat, 0, 11);

    rpi_run_display();
    printf("SCORE: %u\n", game_get_data().score);
	printf("LEVEL: %d\n", game_get_data().game_level );
}

void key_press_callback(uint8_t key){
    
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
            menu_t data = menu_get_current_menu_data();
            printf("menu: %s, option: %u %s\n", data.title, data.current_option, data.option_titles[data.current_option]);
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
		animation_row_compleate();
		update_game_display();
    }else{
        printf("Error state.\n");
    }
}

/////////////SANTI's SANDBOX////////////////////////
int init_audio(char destroy) {
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

	if(!destroy) {

		al_uninstall_audio();
		al_destroy_display(display);
		al_destroy_event_queue(event_queue);
		al_destroy_sample(sample);
	}
	return 0;
}

////////////END OF SANDBOX//////////////////////////