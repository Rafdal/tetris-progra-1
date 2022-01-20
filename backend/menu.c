#include <stdio.h>
#include <stdlib.h>
// #include <assert.h>
#include "menu.h"

static menu_callback_t event_listener; // Callback lector de nuevos eventos
static menu_callback_t update_display; // Callback para actualizar datos de interfaz de usuario

static menu_t *current_menu = NULL;    // Puntero al menu del contexto actual (en ejecucion)


// Inicializar menu (retorna NULL en caso de error)
menu_t* menu_init(uint8_t options, char* title, menu_callback_t on_exit, menu_action_t action){
    menu_t *menu = (menu_t*) calloc(1, sizeof(menu_t)); // Asignar memoria a la estructura
    if(menu != NULL && options > 0){
        // Asignar memoria a los arreglos de informacion de las opciones
        menu->option_titles = (char**) calloc(options, sizeof(char*));
        menu->option_callbacks = (menu_callback_t*) calloc(options, sizeof(menu_callback_t));

        // Si ocurre un error liberar todo
        if(menu->option_titles == NULL || menu->option_callbacks == NULL){
            free(menu->option_titles);
            free(menu->option_callbacks);
            free(menu);
            menu = NULL; // (error)
        }

        // Asignar los datos
        menu->title = title;
        menu->current_option = 0;
        menu->n_options = options;
        menu->on_exit = on_exit;
        menu->exit_action = action;
    }
    return menu;
}

void menu_set_event_listener_display(menu_callback_t ev_listener, menu_callback_t _update_display){
    event_listener = ev_listener;
    update_display = _update_display;
}

void menu_set_option(menu_t *menu, uint8_t option_id, char* title, menu_callback_t callback){
    if(menu != NULL && option_id < menu->n_options){
        menu->option_titles[option_id] = title;
        menu->option_callbacks[option_id] = callback;
    }
}

void menu_destroy(menu_t *menu){ // Liberar memoria de un menu
    if(menu != NULL){
        free(menu->option_titles);
        free(menu->option_callbacks);
        free(menu);
    }
}


// ABRIR Y EJECUTAR UN MENU
void menu_run(menu_t *menu){
    if(menu != NULL && event_listener != NULL && update_display != NULL){

        menu->state = MENU_STATE_AVAILABLE; // Activar el menu
        menu->current_option=0;

        current_menu = menu;
        update_display(); // Mostrar el menu

        while (menu->state != MENU_STATE_CLOSE) // Mientras el menu no se haya cerrado o este cerrado
        {
            menu->state = MENU_STATE_AVAILABLE; // reset state
        
            event_listener(); // escuchar cambios de estado

            switch (menu->state)
            {
            case MENU_STATE_AVAILABLE:
                /* do nothing */
                break;

            case MENU_STATE_DOWN: // Ir a la opcion inferior
                if(menu->current_option < menu->n_options-1)
                    menu->current_option++;
                break;

            case MENU_STATE_UP: // Ir a la opcion superior
                if(menu->current_option >0)
                    menu->current_option--;
                break;

            case MENU_STATE_SELECT: // Seleccionar la opcion actual
                if(menu->option_callbacks[menu->current_option] != NULL){
                    menu->option_callbacks[menu->current_option]();
                    current_menu = menu; // retomo el control (en caso de que se haya perdido)
                }
                else{
                    printf("null Callback!\n");
                }
                break;

            case MENU_STATE_BACK:
                switch (menu->exit_action)
                {
                case MENU_ACTION_CALLBACK:
                    if(menu->on_exit != NULL)
                        menu->on_exit();                 // ejecuto la funcion de salida
                    break;
                
                case MENU_ACTION_DO_NOTHING:
                    break; // do nothing

                case MENU_ACTION_JUST_EXIT:
                default:
                    menu->state = MENU_STATE_CLOSE;    // cierro el menu (por defecto)
                    break;
                }
                break;

            default:
                printf("Menu Event Error\n");
                break;
            }

            // Si estamos en un estado de transicion, actualizamos el display
            if(menu->state != MENU_STATE_AVAILABLE && menu->state != MENU_STATE_CLOSE){
                update_display();
            }
        }
    }
}

void menu_go_up(void){
    current_menu->state = MENU_STATE_UP;
}

void menu_go_down(void){
    current_menu->state = MENU_STATE_DOWN;
}

void menu_go_select(void){
    current_menu->state = MENU_STATE_SELECT;
}

void menu_go_back(void){
    current_menu->state = MENU_STATE_BACK;
}


menu_t menu_get_current_menu_data(void){
    menu_t data = {};
    if(current_menu != NULL){
        data = *current_menu;
    }
    return data;
}

bool menu_is_available(menu_t *menu){
    if(menu != NULL)
        return menu->state == MENU_STATE_AVAILABLE; // En los estados intermedios (SELECT, UP, etc) se considera que el menu esta ocupado
    else
        return false;
}

bool menu_is_current_available(void){
    return menu_is_available(current_menu);
}

void menu_force_close(menu_t *menu){
    if(menu != NULL)
        menu->state = MENU_STATE_CLOSE;
}

void menu_force_close_current(void){
    menu_force_close(current_menu);
}