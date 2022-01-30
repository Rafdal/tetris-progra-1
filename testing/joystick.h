#ifndef TECLADO_TRUCHO_H
#define TECLADO_TRUCHO_H

#define LONG_PRESS_INTERVAL  500

#include <stdbool.h>
#include <stdint.h>

typedef void (*dpad_callback_t)(uint8_t);

enum {DPAD_UP, DPAD_RIGHT, DPAD_DOWN, DPAD_LEFT, DPAD_UPLEFT, DPAD_UPRIGHT, DPAD_BTN};

// extern const char dpad_key_names[][DPAD_NAMELEN];

void dpad_init(void); // iniciar libreria
void dpad_read(void);  // leer botones y ejecutar callbacks (debe llamarse en Tiempo Real)

// al llamarse habilita que durante un LongPress, se llame al callback "on_press" cada <interval> milisegundos
void dpad_use_press_callback_for_longpress(uint8_t key); 

bool dpad_is_pressed(uint8_t key); // devuelve true si el boton esta presionado
bool dpad_is_longpressed(uint8_t key); // devuelve true si el boton fue presionado por DPAD_LONG_PRESS milisegundos

void dpad_on_press(dpad_callback_t callback); // Setear callback click de tecla

#endif // TECLADO_TRUCHO_H