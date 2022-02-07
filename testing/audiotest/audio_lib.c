#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_acodec.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include "audio_lib.h"
//Extensiones con acodec .wav, .flac, .ogg, .it, .mod, .s3m, .xm. 


static char is_music_playing;
static ALLEGRO_SAMPLE *g_music = NULL;
static ALLEGRO_SAMPLE_INSTANCE *g_instance = NULL;
static ALLEGRO_SAMPLE *g_music_effect = NULL;
static ALLEGRO_SAMPLE_INSTANCE *g_instance_effect = NULL;

void manage_music (char optn, char mode) {

	printf("music state = %d\n", is_music_playing);

	if ((optn <= pausa ) && mode && !is_music_playing) {
		al_reserve_samples(3);

		switch (optn) {
			case game:
					g_music = al_load_sample("tetris.wav");
				//	printf("la musica de juego fue creada\n");
					is_music_playing = 1;
				break;
			case menu:
					g_music = al_load_sample("main_title.wav");
			//	printf("la musica de menu fue creada\n");
					is_music_playing = 1;
				break;

			case pausa:
					g_music = al_load_sample("pausa.wav");
					is_music_playing = 1;
		}
		g_instance = al_create_sample_instance(g_music);
		al_attach_sample_instance_to_mixer(g_instance, al_get_default_mixer());
		al_set_sample_instance_playmode(g_instance, ALLEGRO_PLAYMODE_LOOP);
		al_play_sample_instance(g_instance);
	}
	else if ((!mode) && is_music_playing){
		printf("audio_lib: musica destruida\n");
		destroy_music(g_instance);
	}

	 if (optn > pausa){

		al_reserve_samples(3);

		switch (optn) {
			case win:
				g_music = al_load_sample("tetris.wav");

				is_music_playing = 1;
				break;
			case lose:
				g_music = al_load_sample("main_title.wav");

				is_music_playing = 1;
				break;
			case clr_lane:
				g_music = al_load_sample("pausa.wav");
				is_music_playing = 1;
				break;
			case TETRIS:
				g_music = al_load_sample("main_title.wav");

				is_music_playing = 1;
				break;
			case chime:
				g_music_effect = al_load_sample("chime.wav");

				is_music_playing = 1;
				break;
		}
		g_instance_effect = al_create_sample_instance(g_music_effect);
		al_attach_sample_instance_to_mixer(g_instance_effect, al_get_default_mixer());
		al_set_sample_instance_playmode(g_instance_effect, ALLEGRO_PLAYMODE_ONCE);
		al_play_sample_instance(g_instance_effect);
		// printf("esto si se ejecuta\n");
	}
}

void destroy_music (ALLEGRO_SAMPLE_INSTANCE * todestroy){
	al_destroy_sample_instance(todestroy);
	is_music_playing = 0;
}

char install_audio (void){

	if (!al_install_audio()) {
		fprintf(stderr, "failed to initialize audio!\n");
		return -1;
	}

	if (!al_init_acodec_addon()) {
		fprintf(stderr, "failed to initialize audio codecs!\n");
		return -1;
	}

	if (!al_reserve_samples(10)) {
		fprintf(stderr, "failed to reserve samples!\n");
		return -1;
	}
//	printf("esta funcion fue llamada"); //debug
	return 0;
}
