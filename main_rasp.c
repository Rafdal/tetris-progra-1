#include <stdio.h>

#include "./libs/easy_timer.h"
#include "./libs/joystick.h"
#include "./backend/game.h"
// #include "./libs/rpi_display.h"

uint8_t press_count[DPAD_KEYS];

void key_press_callback(uint8_t key){
    press_count[key]++;
    /* switch (key)
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

        case DPAD_BTN:
            printf("BTN\n");
            break;

        default:
            break;
    } */
}

void update_display(void) {
	// rpi_copyToDis((char**)&public_matrix[0][0], HEIGHT, WIDTH, 0,0);     //  SEGMENTATION FAULT
	// rpi_run_display();
}

int main(void){

    // rpi_init_display();
    // init_game();
    dpad_init();
    dpad_on_press(key_press_callback);

	// interval_t display_interval = set_interval(update_display, 200);

    while (1)
    {
        int key;
        for(key=0; key<DPAD_KEYS; key++){
            if(dpad_is_longpressed(key)){
                // printf("LONG Pressed %s\n", dpad_key_names[key]);
            }
        }
		// run_interval(&display_interval);
        dpad_run();
    }

    return 0;
}