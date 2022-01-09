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
#define DPAD_BTN_DEBOUNCE_TIME 50   

#define DPAD_KEYS   7
enum {DPAD_UP, DPAD_RIGHT, DPAD_DOWN, DPAD_LEFT, DPAD_UPLEFT, DPAD_UPRIGHT, DPAD_BTN};

void dpad_init(void);
void dpad_run(void);

bool dpad_is_pressed(uint8_t key);
void dpad_on_press(dpad_callback_t callback); // Setear callback click de tecla (joystick)
void dpad_on_longpress(dpad_callback_t callback); // Setear callback para Long Press

#endif // JOYSTICK_H