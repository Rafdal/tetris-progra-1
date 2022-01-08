#include <stdio.h>

#include "./libs/easy_timer.h"
#include "./libs/joystick.h"

int main(void){

    dpad_init();
    interval_t inter = set_interval(dpad_run, 30);

    while (1)
    {
        run_interval(&inter);
    }

    return 0;
}
