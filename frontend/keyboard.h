#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define LONG_PRESS_INTERVAL  500

#include <allegro5/allegro.h>
#include <stdbool.h>
#include <stdint.h>

typedef void (*keyb_callback_t)(uint8_t);

#define KEYB_KEYS 8
#define KEYB_LONGPRESS_DELAY 100
enum {KEYB_UP, KEYB_RIGHT, KEYB_DOWN, KEYB_LEFT, KEYB_Q, KEYB_E, KEYB_SPACE, KEYB_ESC};


bool keyb_init(ALLEGRO_EVENT_QUEUE *queue); // iniciar libreria - true = OK | false = error
void keyb_run(ALLEGRO_EVENT* pev);  // leer botones y ejecutar callbacks (debe llamarse en Tiempo Real)

// al llamarse habilita que durante un LongPress, se llame al callback "on_press" cada <KEYB_LONGPRESS_DELAY> milisegundos
void keyb_use_press_callback_for_longpress(uint8_t key); 

bool keyb_is_pressed(uint8_t key); // devuelve true si el boton esta presionado
bool keyb_is_longpressed(uint8_t key); // devuelve true si el boton fue presionado por KEYB_LONG_PRESS milisegundos

void keyb_on_press(keyb_callback_t callback);  // Setear callback click de tecla

#endif // _KEYBOARD_H_