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


typedef enum {AUDIO_PLAY, AUDIO_STOP} audio_status_t;
typedef enum {AUDIO_MUSIC, AUDIO_EFFECT} audio_type_t;

typedef struct {
	ALLEGRO_SAMPLE_INSTANCE *instance;
	ALLEGRO_SAMPLE *sample;
	audio_type_t type;
	audio_status_t status;
	float gain;
	float speed;
}audio_t;



bool audio_install (void);

audio_t* audio_init(const char* filename, float gain, audio_type_t type);

void audio_play(audio_t* audio);

void audio_destroy(void);

// Si se esta reproduciendo lo detiene
void audio_force_stop(audio_t* audio);

void audio_set_speed(audio_t* audio, float speed);

bool audio_is_playing(audio_t* audio);


#endif //AUDIO_LIB_H
