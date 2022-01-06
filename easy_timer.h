#ifndef EASY_TIMER_H
#define EASY_TIMER_H

#include <stdint.h>

typedef struct {
    void (*callback)(void);
    uint64_t time;
    uint64_t __last_ms__; // private
} interval_t;

uint64_t get_millis(void);

interval_t set_interval(void (*callback)(void), uint64_t time);
void run_interval(interval_t* interval);

void delay(uint64_t ms);

#endif // EASY_TIMER_H