#include <stdio.h>
#include <assert.h>

#include "./libs/easy_timer.h"
#include "./libs/joystick.h"
#include "./backend/game.h"
#include "./libs/matrix_handler.h"
#include "./libs/rpi_display.h"

void update_display(void);
void key_press_callback(uint8_t key);

int main(void){

    game_init();
    rpi_init_display();
    rpi_set_display(3,3, 1);
    rpi_run_display();

    dpad_init();
    dpad_on_press(key_press_callback);

	// interval_t display_interval = set_interval(update_display, 200);

    while (1)
    {
        dpad_run();
    }
    return 0;
}



void key_press_callback(uint8_t key){
    switch (key)
    {
        case DPAD_UP:
            printf("UP\n");
            break;

        case DPAD_DOWN:
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

void update_display(void) { // TODO: Este codigo es ineficiente, habria que implementar matrix_handler.h en game.h
    matrix_hand_t aux_mat;
    assert(mat_init(&aux_mat, HEIGHT, WIDTH));


    MAT_COPY_FROM_2D_ARRAY(&aux_mat, game_public_matrix, HEIGHT, WIDTH)

    rpi_copyToDis(&aux_mat, 0, 0);
	rpi_run_display();

    mat_delete(&aux_mat);
}
