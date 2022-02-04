#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_acodec.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include "audio_lib.h"
//Extensiones con acodec .wav, .flac, .ogg, .it, .mod, .s3m, .xm. 





void manage_music (char optn, char mode) {
	al_reserve_samples(2);
	static ALLEGRO_SAMPLE *g_music = NULL;
	static ALLEGRO_SAMPLE_INSTANCE *g_instance = NULL;
	if (mode) {
		switch (optn) {
			case game:
					g_music = al_load_sample("tetris.wav");
				break;
			case menu:
					g_music = al_load_sample("main_title.wav");
				break;
			case pausa:
					g_music = al_load_sample("pausa.wav");
		}
		g_instance = al_create_sample_instance(g_music);
		al_attach_sample_instance_to_mixer(g_instance, al_get_default_mixer());
		al_set_sample_instance_playmode(g_instance, ALLEGRO_PLAYMODE_LOOP);
		al_play_sample_instance(g_instance);
	}
	else
		destroy_music(g_instance);
}
void destroy_music (ALLEGRO_SAMPLE_INSTANCE * todestroy){


	al_destroy_sample_instance(todestroy);
	al_uninstall_audio();


}

char install_audio (void){

		return 0;

}
