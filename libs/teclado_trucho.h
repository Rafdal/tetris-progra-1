#ifndef TECLADO_TRUCHO_H
#define TECLADO_TRUCHO_H

#define LONG_PRESS_TIMEOUT  600

#include <stdbool.h>

enum MYKEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_E, KEY_Q //arrow keys
};

typedef void (*callback_t)(void);

void set_on_pressed(callback_t f, int key_id);
void set_on_exit(callback_t f);

bool is_pressed(int key_id); // devuelve true si la tecla esta presionada
bool is_long_pressed(int key_id); // devuelve true si fue presionado por mas de 1000 

int teclado_begin(void);
void teclado_run(void);
void teclado_close(void);

#endif // TECLADO_TRUCHO_H