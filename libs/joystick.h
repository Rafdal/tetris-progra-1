#ifndef JOYSTICK_H
#define JOYSTICK_H

/************************************
 *      JOYSTICK TO DPAD RASPBERRY  *
 ************************************/

#include <stdint.h>

typedef void (*dpad_callback_t)(uint8_t);

#define DPAD_LONG_PRESS 500 // milisegundos
#define DPAD_UMBRAL 80      // umbral de activacion (joystick)
#define DPAD_DEBOUNCE 30    // umbral de desactivacion (joystick)

enum {DPAD_UP, DPAD_RIGHT, DPAD_DOWN, DPAD_LEFT, DPAD_UPLEFT, DPAD_UPRIGHT};

void dpad_init(void);
void dpad_run(void);

void dpad_on_press(dpad_callback_t callback); // Setear callback click de tecla (joystick)
void dpad_on_longpress(dpad_callback_t callback); // Setear callback para Long Press

#endif // JOYSTICK_H