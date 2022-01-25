#include <ctype.h>

#include "rpi_chars.h"
#include "disdrv.h"

void rpi_putchar(char c, uint8_t y, uint8_t x){
    dcoord_t pos;
    c = (char)toupper(c);
    if(c >= '0' && c <= 'Z'){
        c = c - '0';
        int i,j;
        for(i=0; i<5; i++) // y_axis
        {
            for(j=4; j>1; j--) // x_axis
            {
                pos.x = x-j+4;
                pos.y = y+i;
                if ((ascii_0_z[(int)c][i] & (0x1 << j)) >> j)
                    disp_write(pos, D_ON);
                else
                    disp_write(pos, D_OFF);
            }
        }
    }
    else if (c == ' '){
        int i,j;
        for(i=0; i<8; i++) // y_axis
        {
            for(j=4; j>=0; j--) // x_axis
            {
                pos.x = x+j;
                pos.y = y+i;
                disp_write(pos, D_OFF);
            }
        }
    }
    else{
        int i,j;
        for(i=0; i<8; i++) // y_axis
        {
            for(j=4; j>=0; j--) // x_axis
            {
                pos.x = x+j;
                pos.y = y+i;
                disp_write(pos, D_ON); // unknown symbol
            }
        }
    }
}