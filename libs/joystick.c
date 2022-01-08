
#include <stdio.h>
#include "joystick.h"
#include "joydrv.h"

jcoord_t pos, lastPos;

void dpad_run(void){
    joy_update();
    jcoord_t pos = joy_get_coord();
    if(pos.x != lastPos.x || pos.y != lastPos.y){
        printf("coord: %d,%d\n", pos.x, pos.y);

        lastPos = pos;
    }

}

void dpad_init(void){
    joy_init();
}
