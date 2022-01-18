#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "menu.h"

static menu_callback_t real_time_loop;
static menu_callback_t event_listener;
static menu_callback_t update_display;

menu_t* menu_init(uint8_t options, char* title){
    menu_t *menu = (menu_t*) calloc(1,sizeof(menu_t));
    assert(menu != NULL);

    menu->option = (menu_option_t*) calloc(options, sizeof(menu_option_t));
    if(menu->option == NULL){
        free(menu);
        assert(0); // throw error
    }

    menu->title = title;
    menu->current_option = 0;
    menu->n_options = options;

    return menu;
}

bool menu_initialized(menu_t *menu){ 
#warning HARDCODED
    return true; 
}

void menu_set_event_listener_display(menu_callback_t ev_listener, menu_callback_t _update_display){
    event_listener = ev_listener;
    update_display = _update_display;
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
    assert(menu->option != NULL);
    assert(event_listener != NULL);
    assert(update_display != NULL);

    menu->state = MENU_IDLE;
    menu_current_menu = menu;
    while (menu->state != MENU_INACTIVE)
    {
        if(real_time_loop != NULL)
            real_time_loop();

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
                menu_current_menu = menu; // retomo el control (en caso de que se haya perdido)
            }
            else{
                printf("null Callback!\n");
            }
            break;

        case MENU_EXIT:
            menu->state = MENU_INACTIVE; // cierro el menu
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
        //     menu->state = MENU_INACTIVE; // cierro el menu
        //     printf("Error event_listener == NULL\n");
        // }

    }
}