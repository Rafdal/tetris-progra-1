#include <time.h>

#include "easytimer.h"

void easyDelay(unsigned long millis) 
{
    unsigned long lastTick=clock();
    while((unsigned) clock() <= lastTick+((CLOCKS_PER_SEC/1000)*millis)); // delay
} 