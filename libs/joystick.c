#include <stdio.h>
#include "joystick.h"
#include "easy_timer.h"
#include "joydrv.h"


void callback(void){
    printf("callback\n");
}


void dpad_init(void){
    joy_update();
    jcoord_t joy = joy_get_coord();
    jswitch_t btn = joy_get_switch();
    printf("Coords: (%d,%d)\nBtn: %d\n", joy.x, joy.y, btn);
}

void dpad_run(void){
    interval_t inter = set_interval(callback, 1000);
    joy_init();

    while (1)
    {
        run_interval(&inter);
    }    

}
