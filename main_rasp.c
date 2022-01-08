#include <stdio.h>

#include "./libs/easy_timer.h"
#include "./libs/joystick.h"

void callback(void){
    printf("callback\n");
}

int main(void){
    printf("Funciona\n");

    interval_t inter = set_interval(callback, 1000);

    while (1)
    {
        run_interval(&inter);
    }    

    return 0;
}
