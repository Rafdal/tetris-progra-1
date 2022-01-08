#include <stdio.h>
#include <string.h>

#include "./libs/joydrv.h"
#include "./libs/easy_timer.h"

jcoord_t pos, lastPos;

void callback(void){
    joy_update();
    jcoord_t pos = joy_get_coord();
    if(pos.x != lastPos.x || pos.y != lastPos.y){
        char buf[32];
        sprintf(buf, "coord: %d,%d\n\0", pos.x, pos.y);
        printf(buf);
        for(int i=0; i< strlen(buf); i++){
            putchar((char)8);
        }
        lastPos = pos;
    }

}


int main(void){

    interval_t inter = set_interval(callback, 200);
    joy_init();

    while (1)
    {
        run_interval(&inter);
    }

    return 0;
}
