#include <stdio.h>
#include <assert.h>

#include "./backend/game.h"
#include "./libs/joystick.h"
#include "./libs/easy_timer.h"
#include "./libs/matrix_handler.h"
#include "./libs/rpi_display.h"

void key_press_callback(uint8_t key);

void update_display(void);

int main(void){

    printf("Inicializando...\n");

    rpi_init_display();
    rpi_run_display();


    dpad_init();
    dpad_on_press(key_press_callback);
    dpad_use_press_callback_for_longpress(DPAD_DOWN);
    dpad_use_press_callback_for_longpress(DPAD_LEFT);
    dpad_use_press_callback_for_longpress(DPAD_RIGHT);

    uint64_t lastMillis;
    game_data_t game_data = game_get_data();

    while (game_get_data().state != GAME_QUIT)
    {
        dpad_read();
        
        game_data = game_get_data();

        if(game_data.state == GAME_RUN && easytimer_get_millis()-lastMillis >= game_data.speed_interval){
            if(game_data.id == 0)
                game_insert_block();

            else
                game_move_down();
            game_run();
            update_display();
            lastMillis = easytimer_get_millis();
        }

        if(dpad_is_longpressed(DPAD_BTN)){
            ;// ;
        }
    }
    
    return 0;
}

void update_display(void){
    matrix_hand_t mat_handler;
    assert(mat_init(&mat_handler, GAME_HEIGHT, GAME_WIDTH));
    MAT_COPY_FROM_2D_ARRAY(&mat_handler, game_public_matrix, GAME_HEIGHT, GAME_WIDTH);
    printf("MAT_HANDLER:\n");
    rpi_copyToDis(&mat_handler, 0, 0);

	matrix_hand_t public_next_mat;
	assert(mat_init(&public_next_mat, 12, 4));
	MAT_COPY_FROM_2D_ARRAY(&public_next_mat, game_next_block_public_matrix, 12,4);
	rpi_copyToDis(&public_next_mat, 0, 11);
	mat_print(&public_next_mat);

    rpi_run_display();
    printf("SCORE:\n%u\n", game_get_data().score);
}

void key_press_callback(uint8_t key){
    switch (key)
    {
        case DPAD_UP:
            printf("UP\n");
            break;

        case DPAD_DOWN:
            if(game_get_data().id == 0)
                game_insert_block();
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
    update_display();
}
