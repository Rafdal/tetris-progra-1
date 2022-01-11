#include <stdio.h>
#include <assert.h>

#include "./libs/rpi_display.h"
#include "./libs/easy_timer.h"
#include "./libs/joystick.h"
#include "./libs/matrix_handler.h"

void key_press_callback(uint8_t key);

int run = 1;
matrix_hand_t aux_mat;

int main(void){

    printf("Init rpi_display.h\n");
    rpi_init_display();
    rpi_set_display(3,3, 1);
    rpi_run_display();

    return 0;
}

void key_press_callback(uint8_t key){
    switch (key)
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
    }
}