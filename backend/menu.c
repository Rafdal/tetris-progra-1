#include <stdlib.h>
#include <assert.h>
#include "menu.h"

static menu_callback_t real_time_loop;

menu_t* menu_init(uint8_t options, ev_listener_t ev_listener, char* title){
    menu_t *menu = (menu_t*) malloc(sizeof(menu_t));
    assert(menu != NULL);

    menu->option = (menu_option_t*) calloc(options, sizeof(menu_option_t));
    if(menu->option == NULL){
        free(menu);
        assert(0); // throw error
    }

    menu->title = title;
    menu->current_option = 0;
    menu->listener = ev_listener;
    menu->n_options = options;

    return menu;
}

bool menu_initialized(menu_t *menu){ 
#warning HARDCODED
    return true; 
}

bool menu_set_option(menu_t *menu, uint8_t option_id, char* text, menu_callback_t callback){
    assert(menu != NULL);
    if(option_id < menu->n_options){
        menu_option_t menu_option;
        menu_option.text = text;
        menu_option.callback = callback;
        menu->option[option_id] = menu_option;
    }
    else
        return false; // Out of range
    return true;
}

void menu_destroy(menu_t *menu){
    free(menu->option);
    free(menu);
}


//  MENU CONTROL FUNCTIONS

void menu_run(menu_t *menu){
    assert(menu != NULL);

    menu->state = MENU_IDLE;
    while (menu->state != MENU_INACTIVE)
    {
        if(real_time_loop != NULL)
            real_time_loop();

        if(menu->listener != NULL){
            menu_state_t ev = event_listener(); // get event

            switch (ev)
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

            case MENU_EXIT:
                menu->active = false;
                break;

            default:
                printf("Menu Event Error\n");
                break;
            }

        }
        else{
            menu->active = false;
            printf("Error event_listener == NULL\n");
        }

    }
}