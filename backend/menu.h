#ifndef _MENU_H_
#define _MENU_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {MENU_INACTIVE, MENU_IDLE, MENU_UP, MENU_DOWN, MENU_EXIT, MENU_SELECT} menu_state_t;

typedef void (*menu_callback_t)(void);
typedef menu_state_t (*ev_listener_t)(menu_state_t);

typedef struct {
    char *text;
    menu_callback_t callback;   // Callback que se ejecuta
}menu_option_t;

typedef struct {
    uint8_t state;          // evento actual
    char *title;            // Titulo del menu
    menu_option_t *option;  // Arreglo de opciones de menu
    uint8_t n_options;      // cantidad de opciones
    uint8_t current_option; // opcion actual
} menu_t;

menu_t *menu_current_menu;

// funciones de creacion de menues
menu_t *menu_init(uint8_t options, char* title); 
// INICIALIZAR MENU: Se ejecuta una sola vez al principio
// - Options: cantidad de opciones que tendra el menu
// - title: titulo del menu
void menu_set_event_listener_display(menu_callback_t ev_listener, menu_callback_t _update_display);
bool menu_initialized(menu_t *menu);
bool menu_set_option(menu_t *menu, uint8_t option_id, char* text, menu_callback_t callback);
// void menu_set_event_listener(menu_t *menu, );
void menu_destroy(menu_t *menu);

// menu control
void menu_run(menu_t *menu); // BLOQUEANTE - ejecuta un menu y queda atrapado aqui

#endif // _MENU_H_