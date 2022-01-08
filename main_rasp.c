#include <stdio.h>

#include "./libs/easy_timer.h"
#include "./libs/joystick.h"

void callback(void){
    printf("callback\n");
    joy_update();
    jcoord_t joy = joy_get_coord();
    jswitch_t btn = joy_get_switch();
    printf("Coords: (%d,%d)\nBtn: %d\n", joy.x, joy.y, btn);
}

int main(void){
    printf("Funciona\n");

    interval_t inter = set_interval(callback, 1000);
    joy_init();

    while (1)
    {
        run_interval(&inter);
    }    

    return 0;
}
