#include <stdio.h>
#include <assert.h>

#include "./backend/game.h"
#include "./backend/menu.h"
#include "./testing/joystick.h"
#include "./testing/easy_timer.h"
#include "./testing/matrix_handler.h"
#include "./testing/rpi_display.h"

void key_press_callback(uint8_t key);
void update_game_display(void);
void main_game_start(void);
void update_menu_display(void);
void animation_row_compleate (void);


menu_t *main_menu = NULL;
menu_t *pause_menu = NULL;

void exit_game(void){
    game_quit();                // Finalizar juego
    menu_force_close(pause_menu); // Cerrar menu pausa
}

void restart_game(void){
    menu_force_close(pause_menu); // Cerrar menu pausa
	game_init();
    game_start();
}

void resume_game(void)
{
	menu_force_close_current();
	game_run();
}


int main(void){
    printf("Inicializando...\n");

    rpi_init_display();
    game_init();

    dpad_init();
    dpad_on_press(key_press_callback);
    dpad_use_press_callback_for_longpress(DPAD_DOWN);
    dpad_use_press_callback_for_longpress(DPAD_LEFT);
    dpad_use_press_callback_for_longpress(DPAD_RIGHT);

    main_menu = menu_init(5, "MENU", NULL, MENU_ACTION_DO_NOTHING);
    pause_menu = menu_init(4, "PAUSA", NULL, MENU_ACTION_JUST_EXIT);

    if(main_menu == NULL || pause_menu == NULL){
        printf("Error NULL menu!\n");
        return -1;
    }
    menu_set_option(main_menu, 0, "JUGAR", main_game_start);
    menu_set_option(main_menu, 1, "PARTIDAS GUARDADAS", NULL);
    menu_set_option(main_menu, 2, "TOP SCORES", NULL);
    menu_set_option(main_menu, 3, "CONFIGURACION", NULL);
    menu_set_option(main_menu, 4, "SALIR", menu_force_close_current);

    menu_set_option(pause_menu, 0, "REANUDAR", resume_game);
    menu_set_option(pause_menu, 1, "GUARDAR", NULL);
    menu_set_option(pause_menu, 2, "REINICIAR", restart_game);
    menu_set_option(pause_menu, 3, "SALIR", exit_game);

    // Setear los callbacks que controlaran el menu
    menu_set_event_listener_display(dpad_read, update_menu_display);

    // Ejecutar menu principal
    menu_run(main_menu);

    // Liberar memoria usada por los menues
    menu_destroy(main_menu);
    menu_destroy(pause_menu);

    rpi_clear_display();

    return 0;
}

void update_menu_display(void){
    printf("\n\n\n\n\n\n\n");

    menu_t menu_data = menu_get_current_menu_data();

    printf("%s",menu_data.title);
    putchar('\n');

    uint8_t id;
    for(id=0; id<menu_data.n_options; id++){
        if(menu_data.current_option == id)
            putchar('>');
        else
            putchar(' ');

        if(menu_data.option_titles[id] == NULL)
            printf("null");
        else
            printf("%s",menu_data.option_titles[id]);
        putchar('\n');
    }
}

void main_game_start(void){

	game_init();

    game_data_t game_data;
    uint64_t lastMillis;

    game_start();

    while ((game_data = game_get_data()).state != GAME_QUIT)
    {
        dpad_read();
        
        if(game_data.state == GAME_RUN && get_millis()-lastMillis >= game_data.speed_interval){
            game_move_down();
            game_run();
			animation_row_compleate();
            update_game_display();
            lastMillis = get_millis();
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
	int i, j;
	for( i=0; row_compleate[i] != 0 && i< WIDTH ; i++)
	{
		for(j=0; j < WIDTH; j++)
		{
				delete_pixel(row_compleate[i], j);
				delay(50);
				update_game_display();
		}

		delete_row(row_compleate[i]);
		row_compleate[i]= 0;

	}
}

void update_game_display(void){
    matrix_hand_t mat_handler;
    assert(mat_init(&mat_handler, HEIGHT, WIDTH));
    MAT_COPY_FROM_2D_ARRAY(&mat_handler, game_public_matrix, HEIGHT, WIDTH);
    // printf("MAT_HANDLER:\n");
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
