#include <stdio.h>

#include "./libs/easy_timer.h"
#include "./libs/joystick.h"

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

int main(void){

    dpad_init();
    dpad_on_press(key_press_callback);

    while (1)
    {
        dpad_run();
    }

    return 0;
}
