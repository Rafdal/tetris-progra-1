#include <stdio.h>
#include <assert.h>

#include "./libs/rpi_display.h"
#include "./libs/easy_timer.h"
#include "./libs/joystick.h"
#include "./libs/matrix_handler.h"

void key_press_callback(uint8_t key);

int run = 1;
matrix_hand_t aux_mat;

uint8_t x,y;

uint8_t game_mat[3][3] = {
    {0,1,0},
    {1,1,1},
    {0,1,0},
};

int main(void){

    printf("Init rpi_display.h\n");
    rpi_init_display();
    rpi_set_display(y,x, 1);
    rpi_run_display();

    dpad_init();
    dpad_on_press(key_press_callback);

    while (run)
    {
        dpad_run();

        if(dpad_is_longpressed(DPAD_BTN)){
            run = 0;
        }
    }
    
    return 0;
}

void key_press_callback(uint8_t key){
    rpi_clear_display();
    switch (key)
    {
        case DPAD_UP:
            printf("UP\n");
            if(y>0){
                y--;
            }
            break;

        case DPAD_DOWN:
            if(y<RPI_HEIGHT-1){
                y++;
            }
            printf("DOWN\n");
            break;

        case DPAD_LEFT:
            if(x>0){
                x--;
            }
            printf("LEFT\n");
            break;

        case DPAD_RIGHT:
            if(x<RPI_WIDTH-1){
                x++;
            }
            printf("RIGHT\n");
            break;

        case DPAD_UPRIGHT:
            printf("UPRIGHT\n");
            break;

        case DPAD_UPLEFT:
            printf("UPLEFT\n");
            break;

        case DPAD_BTN:
            {
                matrix_hand_t mat;
                mat_init(&mat, 3, 3);
                MAT_COPY_FROM_2D_ARRAY(&mat, game_mat, 3, 3)
                rpi_copyToDis(&mat, y-1, x-1);
                mat_delete(&mat);
            }
            printf("BTN\n");
            break;

        default:
            break;
    }
    rpi_set_display(y,x,1);
    rpi_run_display();
}