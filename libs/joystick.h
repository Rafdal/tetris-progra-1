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

#define DPAD_KEYS   7
enum {DPAD_UP, DPAD_RIGHT, DPAD_DOWN, DPAD_LEFT, DPAD_UPLEFT, DPAD_UPRIGHT, DPAD_BTN};

void dpad_init(void);
void dpad_run(void);

bool dpad_is_pressed(uint8_t key);
bool dpad_is_longpressed(uint8_t key); // Setear callback para Long Press
void dpad_on_press(dpad_callback_t callback); // Setear callback click de tecla (joystick)

#endif // JOYSTICK_H