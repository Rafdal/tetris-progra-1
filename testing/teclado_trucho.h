#ifndef TECLADO_TRUCHO_H
#define TECLADO_TRUCHO_H

#define LONG_PRESS_TIMEOUT  500

#include <stdbool.h>

typedef void (*dpad_callback_t)(uint8_t);

enum {DPAD_UP, DPAD_RIGHT, DPAD_DOWN, DPAD_LEFT, DPAD_UPLEFT, DPAD_UPRIGHT, DPAD_BTN};

// extern const char dpad_key_names[][DPAD_NAMELEN];

void dpad_init(void); // iniciar libreria
void dpad_run(void);  // leer botones y ejecutar callbacks (debe llamarse en Tiempo Real)
void dpad_close(void);

bool dpad_is_pressed(uint8_t key); // devuelve true si el boton esta presionado
bool dpad_is_longpressed(uint8_t key); // devuelve true si el boton fue presionado por DPAD_LONG_PRESS milisegundos
void dpad_on_press(dpad_callback_t callback); // Setear callback click de tecla

#endif // TECLADO_TRUCHO_H