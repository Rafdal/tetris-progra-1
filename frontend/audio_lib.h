 //
// Created by santia on 30/1/22.
//

#ifndef AUDIO_LIB_H
#define AUDIO_LIB_H

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_acodec.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include "audio_lib.h"

#define start 1
#define stop 0

 enum {
	 game,
	 menu,
	 pausa,
	 lose,
	 clr_lane_1,
	 clr_lane_2,
	 clr_lane_3,
	 TETRIS,
	 chime,
	 chime_select,
 };


////////////Prototipos//////////////////

void manage_music (char, char);
void destroy_music (ALLEGRO_SAMPLE_INSTANCE*);
char install_audio (void);



#endif //AUDIO_LIB_H
