#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "menu.h"

static menu_callback_t event_listener;
static menu_callback_t update_display;

menu_t* menu_init(uint8_t options, char* title, menu_callback_t exit_callback, menu_action_t action){
    menu_t *menu = (menu_t*) calloc(1, sizeof(menu_t));
    assert(menu != NULL);

    menu->option = (menu_option_t*) calloc(options, sizeof(menu_option_t));
    if(menu->option == NULL){
        free(menu);
        assert(0); // throw error
    }

    menu->title = title;
    menu->current_option = 0;
    menu->n_options = options;
    menu->exit_c = exit_callback;
    menu->on_exit = action;

    return menu;
}


bool menu_initialized(menu_t *menu){ 
#warning HARDCODEADO!!! :(
    return true; 
}

void menu_set_event_listener_display(menu_callback_t ev_listener, menu_callback_t _update_display){
    event_listener = ev_listener;
    update_display = _update_display;
}

bool menu_set_option(menu_t *menu, uint8_t option_id, char* text, menu_callback_t callback){
    assert(menu != NULL);
    assert(option_id < menu->n_options);

    menu->option[option_id].text = text;
    menu->option[option_id].callback = callback;

    return true;
}

void menu_destroy(menu_t *menu){
    free(menu->option);
    free(menu);
}


//  MENU CONTROL FUNCTIONS

void menu_run(menu_t *menu){
    assert(menu != NULL);
    assert(menu->option != NULL);
    assert(event_listener != NULL);
    assert(update_display != NULL);

    menu->state = MENU_IDLE;
    menu->current_option=0;
    menu_current_menu = menu;
    update_display();
    while (menu->state != MENU_CLOSE)
    {
        menu->state = MENU_IDLE;
        event_listener(); // get event state

        switch (menu->state)
        {
        case MENU_IDLE:
            /* do nothing */
            break;

        case MENU_DOWN:
            if(menu->current_option < menu->n_options-1)
                menu->current_option++;
            break;

        case MENU_UP:
            if(menu->current_option >0)
                menu->current_option--;
            break;

        case MENU_SELECT:
            if(menu->option[menu->current_option].callback != NULL){
                menu->option[menu->current_option].callback();

                if(menu->state != MENU_CLOSE) // si no hay que cerrar
                    menu_current_menu = menu; // retomo el control (en caso de que se haya perdido)
            }
            else{
                printf("null Callback!\n");
            }
            break;

        case MENU_EXIT:
            switch (menu->on_exit)
            {
            case MENU_ACTION_CALLBACK:
                if(menu->exit_c != NULL)
                    menu->exit_c();                 // ejecuto la funcion de salida
                break;
            
            case MENU_ACTION_EXIT:
                menu->state = MENU_CLOSE;    // cierro el menu
                break;
            }
            break;

        default:
            printf("Menu Event Error\n");
            break;
        }

        if(menu_current_menu->state != MENU_IDLE){
            update_display();
        }
        // }
        // else{
        //     menu->state = MENU_CLOSE; // cierro el menu
        //     printf("Error event_listener == NULL\n");
        // }

    }
}

void menu_close_current(void){
    menu_current_menu->state = MENU_CLOSE;
}