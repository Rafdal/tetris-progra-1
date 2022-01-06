#ifndef TECLADO_TRUCHO_H
#define TECLADO_TRUCHO_H

#include <stdbool.h>

enum MYKEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT //arrow keys
};

typedef void (*callback_t)(void);

void set_on_pressed(callback_t f, int key_id);
void set_on_exit(callback_t f);

bool is_pressed(int key_id); // devuelve true si la tecla esta presionada

int teclado_begin(void);
void teclado_run(void);
void teclado_close(void);

#endif // TECLADO_TRUCHO_H