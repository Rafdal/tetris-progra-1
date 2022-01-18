#ifndef _MENU_H_
#define _MENU_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {MENU_CLOSE, MENU_IDLE, MENU_UP, MENU_DOWN, MENU_EXIT, MENU_SELECT} menu_state_t;
typedef enum {MENU_ACTION_DO_NOTHING, MENU_ACTION_EXIT, MENU_ACTION_CALLBACK} menu_action_t;

typedef void (*menu_callback_t)(void);

typedef struct {
    char *text;                 // texto de la opcion
    menu_callback_t callback;   // Callback de la opcion
}menu_option_t;

typedef struct {
    uint8_t state;          // evento (estado) actual
    char *title;            // Titulo del menu
    menu_option_t *option;  // Arreglo de opciones de menu
    uint8_t n_options;      // cantidad de opciones
    uint8_t current_option; // opcion actual
    menu_action_t on_exit;  // accion de salida
    menu_callback_t exit_c; // callback de salida
} menu_t;

menu_t *menu_current_menu;

// funciones de creacion de menues
menu_t *menu_init(uint8_t options, char* title, menu_callback_t exit_callback, menu_action_t action);
// INICIALIZAR MENU: Se ejecuta una sola vez al principio
// - Options: cantidad de opciones que tendra el menu (numero entero)
// - title: titulo del menu (cadena de texto)
// - exit_callback: funcion que se ejecuta al salir del menu. Colocar NULL para NO usar.

void menu_set_event_listener_display(menu_callback_t ev_listener, menu_callback_t _update_display);
bool menu_initialized(menu_t *menu);
bool menu_set_option(menu_t *menu, uint8_t option_id, char* text, menu_callback_t callback);

void menu_destroy(menu_t *menu);

// menu control
void menu_run(menu_t *menu); // BLOQUEANTE - ejecuta un menu y queda atrapado aqui
void menu_close_current();

#endif // _MENU_H_