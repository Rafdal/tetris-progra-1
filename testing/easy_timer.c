#include <stdio.h> // NULL
#include <sys/time.h>
#include "easy_timer.h"

uint64_t easytimer_get_millis(void){
    uint64_t millis;

    struct timeval time;
    gettimeofday(&time, 0);
    millis = time.tv_sec*1000ULL + time.tv_usec/1000ULL;

    return millis;
}

interval_t easytimer_set_interval(void (*callback)(void), uint64_t time){
    interval_t interval;
    interval.callback = callback;
    interval.time = time;
    return interval;
}

void easytimer_run_interval(interval_t* interval){
    if (easytimer_get_millis() - interval->__last_ms__ >= interval->time)
    {
        if(interval->callback != NULL)
            interval->callback();
        interval->__last_ms__ = easytimer_get_millis();
    }
}

void easytimer_delay(uint64_t ms){
    uint64_t last_ms = easytimer_get_millis();
    while (easytimer_get_millis() - last_ms <= ms)
    {
        ;/* DO NOTHING */
    }
}

