#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <stdbool.h>

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} color_t;

bool run;

int game_screen_begin(void);
void game_screen_draw(float x, float y, color_t color);
void game_screen_clear(void);
void game_screen_show(void);
void game_screen_run(void);
void game_screen_end(void);

#endif