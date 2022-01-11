#include <stdio.h>
#include "dispEmu.h"

static char mat[16][16];

void disp_init(void){
    disp_clear();
}

void disp_clear(void){
    for(int i=0; i<16; i++)
    {
        for(int j=0; j<16; j++)
            mat[i][j]=0;
    }
}

void disp_write(dcoord_t coord, dlevel_t val){
    mat[coord.y][coord.x] = val;
}

void disp_update(void){
    for (int i = 0; i < 34; i++)
        putchar('-');
    putchar('\n');
    for(int y=0; y<16; y++)
    {
        putchar('|');
        for(int x=0; x<16; x++)
        {
            if (mat[y][x] == 0)
            {
                printf("  ");
            }
            else{
                putchar('0'+mat[y][x]);
                putchar(' ');
            }
        }
        putchar('|');
        putchar('\n');
    }
    for (int i = 0; i < 34; i++)
        putchar('-');
    putchar('\n');
}