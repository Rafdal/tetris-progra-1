#include <stdio.h>

#include "./libs/easy_timer.h"
#include "./libs/joystick.h"
#include "./libs/rpi_display.h"
#include "./backend/game.h"

void key_press_callback(uint8_t key_id){
    switch (key_id)
    {
    case DPAD_UP:
        printf("UP\n");
        break;

    case DPAD_DOWN:
        printf("DOWN\n");
        break;

    case DPAD_LEFT:
        printf("LEFT\n");
        break;

    case DPAD_RIGHT:
        printf("RIGHT\n");
        break;

    case DPAD_UPRIGHT:
        printf("UPRIGHT\n");
        break;

    case DPAD_UPLEFT:
        printf("UPLEFT\n");
        break;

    default:
        break;
    }
}

void update_display(void) {
	rpi_copyToDis((char**)&public_matrix[0][0], HEIGHT, WIDTH, 0,0);
	run_rpi_display();
}

int main(void){

    init_game();
    dpad_init();
    dpad_on_press(key_press_callback);

	interval_t display_interval = set_interval(update_display, 200);

    while (1)
    {
		run_interval(&display_interval);
        dpad_run();
    }

    return 0;
}

