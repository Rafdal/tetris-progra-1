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
uint8_t main_menu_event_listener(void);

menu_t *main_menu;

int main(void){

    printf("Inicializando...\n");

    rpi_init_display();
    game_init();

    dpad_init();
    dpad_on_press(key_press_callback);
    dpad_use_press_callback_for_longpress(DPAD_DOWN);
    dpad_use_press_callback_for_longpress(DPAD_LEFT);
    dpad_use_press_callback_for_longpress(DPAD_RIGHT);

    main_menu = menu_init(3, main_menu_event_listener, "MENU");
    assert(menu_initialized(main_menu));
    menu_set_option(main_menu, 0, "JUGAR", main_game_start);
    menu_set_option(main_menu, 1, "OPT1", NULL);

    menu_run(main_menu);

    menu_destroy(main_menu);
    rpi_clear_display();

    return 0;
}

uint8_t main_menu_event_listener(void){
    dpad_read();
}

void main_game_start(void){

    game_data_t game_data;
    uint64_t lastMillis;

    while ((game_data = game_get_data()).state != GAME_QUIT)
    {
        dpad_read();
        
        if(game_data.state == GAME_RUN && get_millis()-lastMillis >= game_data.speed_interval){
            if(game_data.id == 0)
                game_insert_block(id_next_block[0]);

            else
                game_move_down();
            game_run();
            update_game_display();
            lastMillis = get_millis();
        }

        if(dpad_is_longpressed(DPAD_BTN)){
            if(game_data.state == GAME_IDLE){
                game_start();
            }
        }

        if(game_data.state == GAME_LOSE){
            printf("Perdiste! The Game\n");
            game_init();
        }
    }
}


void update_game_display(void){
    matrix_hand_t mat_handler;
    assert(mat_init(&mat_handler, HEIGHT, WIDTH));
    MAT_COPY_FROM_2D_ARRAY(&mat_handler, game_public_matrix, HEIGHT, WIDTH);
    printf("MAT_HANDLER:\n");
    rpi_copyToDis(&mat_handler, 0, 0);

	matrix_hand_t public_next_mat;
	assert(mat_init(&public_next_mat, 12, 4));
	MAT_COPY_FROM_2D_ARRAY(&public_next_mat, next_block_public_matrix, 12,4);
	rpi_copyToDis(&public_next_mat, 0, 11);
	mat_print(&public_next_mat);

    rpi_run_display();
    printf("SCORE:\n%u\n", game_get_data().score);
}

void key_press_callback(uint8_t key){
    menu_event(key);
    switch (key)
    {
        case DPAD_UP:
            printf("UP\n");
            break;

        case DPAD_DOWN:
            if(game_get_data().id == 0)
                game_insert_block(id_next_block[0]);
            else
                game_move_down();
            printf("DOWN\n");
            break;

        case DPAD_LEFT:
            game_move_horizontal(0);
            printf("LEFT\n");
            break;

        case DPAD_RIGHT:
            game_move_horizontal(1);
            printf("RIGHT\n");
            break;

        case DPAD_UPRIGHT:
            game_rotate(1);
            printf("UPRIGHT\n");
            break;

        case DPAD_UPLEFT:
            game_rotate(0);
            printf("UPLEFT\n");
            break;

        case DPAD_BTN:
            printf("BTN\n");
            break;

        default:
            break;
    }
    game_run();
    update_game_display();
}
