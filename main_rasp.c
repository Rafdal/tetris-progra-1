#include <stdio.h>


#include "./libs/joydrv.h"

int main(void){
    joy_init();
    joy_update();
    jcoord_t pos = joy_get_coord();

    printf("coord: %d,%d\n", pos.x, pos.y);

    return 0;
}
