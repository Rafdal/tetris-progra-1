#ifndef JOYSTICK_H
#define JOYSTICK_H

/************************************
 *      JOYSTICK TO DPAD RASPBERRY  *
 ************************************/

#include <stdint.h>
#include <stdbool.h>


typedef void (*dpad_callback_t)(uint8_t);

#define DPAD_LONG_PRESS 500         // milisegundos
#define DPAD_UMBRAL 80              // umbral de activacion (joystick)
#define DPAD_DEBOUNCE 30            // umbral de desactivacion (joystick)

#define DPAD_KEYS    7
#define DPAD_NAMELEN 8

enum {DPAD_UP, DPAD_RIGHT, DPAD_DOWN, DPAD_LEFT, DPAD_UPLEFT, DPAD_UPRIGHT, DPAD_BTN, DPAD_idle};

const char dpad_key_names[][DPAD_NAMELEN] = {"UP", "RIGHT", "DOWN", "LEFT", "UPLEFT", "UPRIGHT", "BTN", "idle"};

void dpad_init(void); // iniciar libreria
void dpad_run(void);  // leer botones y ejecutar callbacks (debe llamarse en Tiempo Real)

bool dpad_is_pressed(uint8_t key); // devuelve true si el boton esta presionado
bool dpad_is_longpressed(uint8_t key); // devuelve true si el boton fue presionado por DPAD_LONG_PRESS milisegundos
void dpad_on_press(dpad_callback_t callback); // Setear callback click de tecla

#endif // JOYSTICK_H