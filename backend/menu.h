#ifndef _MENU_H_
#define _MENU_H_

#include <stdint.h>
#include <stdbool.h>

/********************************************************************************
 *                              LIBERIA MENU                                    *
 * Esta libreria permite crear diferentes menues y poder controlarlos           *
 * facilmente, navegar entre sus opciones, crear sub-menues y estructuras       *
 * complejas de menues dentro de otros menues con total libertad                *
 *                                                                              *
 * Tambien permite acceder con facilidad al los datos del menu actual para poder*
 * mostrarlos por pantalla (lista de opciones, opcion seleccionada, titulo, etc)*
 *                                                                              *
 * Esta libreria NO cumple funciones de frontend, el programador decide como    *
 * conectar esta libreria con el frontend y que interfaz usara                  *
 ********************************************************************************
 */

// Acciones del menu al ejecutar menu_go_back()
typedef enum {
    MENU_ACTION_DO_NOTHING, // No hacer nada al ir hacia atras (no salir)
    MENU_ACTION_JUST_EXIT,  // Salir del menu al ir hacia atras
    MENU_ACTION_CALLBACK,   // Ejecutar un callback al ir hacia atras
} menu_action_t;

// Estados del menu
typedef enum {
    MENU_STATE_CLOSE,       // Cerrar menu | menu cerrado
    MENU_STATE_AVAILABLE,   // Menu activo en espera
    MENU_STATE_UP,      // Ir a la opcion superior (estado de transicion)
    MENU_STATE_DOWN,    // Ir a la opcion inferior (estado de transicion)
    MENU_STATE_BACK,    // Ir hacia atras (estado de transicion)
    MENU_STATE_SELECT   // Seleccionar opcion (estado de transicion)
} menu_state_t;

typedef void (*menu_callback_t)(void); // Void Callback

// Estructura de datos del menu
typedef struct {
    char **option_titles;   // titulos de cada opcion
    menu_callback_t *option_callbacks; // callbacks de cada opcion
    uint8_t current_option; // opcion actual
    uint8_t state;          // evento (estado) actual
    char *title;            // Titulo del menu
    uint8_t n_options;      // cantidad de opciones
    menu_action_t exit_action;  // accion de salida
    void (*on_exit)(void); // callback de salida
} menu_t; // Objeto Menu


// (1) FUNCIONES DE CREACION Y CONFIGURACION DE MENUES
menu_t* menu_init(uint8_t options, char* title, menu_callback_t on_exit, menu_action_t action);
// DESCRIPCION Y PARAMETROS: Inicializar menu. Se ejecuta una sola vez al principio del programa, en la configuracion
// - [options]: cantidad de opciones que tendra el menu (numero entero > 0)
// - [title]:   titulo del menu (c-string)
// - [on_exit]: funcion que se ejecuta al salir del menu. Colocar NULL para NO usar.
// - [action]:  accion que realizara el menu cuando se ejecuta la funcion "menu_back()"
// RETORNA: NULL en caso de error | puntero a un menu en caso contrario
// EJEMPLO: menu_t *cfg_menu = menu_init(3, "Config", NULL, MENU_ACTION_JUST_EXIT);
void menu_destroy(menu_t *menu); 
// libera memoria usada por el menu (llamar al finalizar el programa)

void menu_set_option(menu_t *menu, uint8_t option_id, char* title, menu_callback_t callback);
// DESCRIPCION Y PARAMETROS: Configurar una opcion de un menu
// - [menu]:        puntero a menu_t (objeto menu)
// - [option_id]:   numero de opcion que desea configurar (entre 0 y la cant. de opciones que tiene dicho menu)
// - [title]:       titulo de la opcion (c-string)
// - [callback]:    funcion que se ejecuta al seleccionar la opcion
// EJEMPLO:     menu_set_option(cfg_menu, 0, "Save data", config_save_data);
void menu_set_event_listener_display(menu_callback_t ev_listener, menu_callback_t update_display);
// DESCRIPCION Y PARAMETROS: Configurar escucha de eventos de control y funciones de visualizacion del menu
// - [ev_listener]: funcion que se ejecuta en TIEMPO REAL para escuchar nuevos eventos (ej: clicks de teclado)
//                  Colocar por ejemplo, un callback de lectura del joystick
// - [update_display]:  funcion que se ejecuta cada vez que se deberia actualizar el display que muestra el menu.
//                      Dentro de esta funcion se deberia llamar menu_get_current_menu_data() para obtener
//                      los datos del menu actual y asi poder mostrarlos al usuario.
// EJEMPLO:     menu_set_event_listener_display(keyboard_read, display_update);

void menu_set_animation_callback(menu_callback_t animation_f); // OPCIONAL. Loop en tiempo real


// (2) FUNCIONES DE MENU
void menu_run(menu_t *menu); // BLOQUEANTE - ejecuta y abre un menu. El programa queda atrapado dentro del mismo hasta que se cierre
void menu_force_close(menu_t *menu);    // Cerrar el menu indicado
bool menu_is_available(menu_t* menu);   // Retorna true si el menu esta activo


// (3) FUNCIONES PARA NAVEGAR EN EL MENU (colocar en un callback asociado a un evento de control. ej: Teclado, Joystick)
void menu_go_up(void);     // Ir a opcion superior
void menu_go_down(void);   // Ir a opcion inferior
void menu_go_select(void); // Seleccionar opcion
void menu_go_back(void);   // Ir hacia atras


// (4) FUNCIONES PARA MANIPULAR EL MENU EN EJECUCION ACTUAL
bool menu_is_current_available(void);      // Retorna true si el menu actual esta activo
void menu_force_close_current(void);       // fuerza el cierre del menu actual
menu_t menu_get_current_menu_data(void);   // Retorna una estructura con datos del menu actual (para mostrar en una UI, etc)

#endif // _MENU_H_