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

	printf("music state = %d,  optn: %u, mode: %u\n", is_music_playing, optn, mode);

	if ((optn <= pausa ) && mode && !is_music_playing) {
		al_reserve_samples(3);
		printf("reserve samples\n");
		switch (optn) {
			case game:
					g_music = al_load_sample("tetris.wav");
				//	printf("la musica de juego fue creada\n");
					is_music_playing = 69;
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
		if(g_music == NULL){
			printf("MUSIC NULL!\n");
			exit(1);
		}
		g_instance = al_create_sample_instance(g_music);
		al_attach_sample_instance_to_mixer(g_instance, al_get_default_mixer());
		al_set_sample_instance_playmode(g_instance, ALLEGRO_PLAYMODE_LOOP);
		al_play_sample_instance(g_instance);
		if (is_music_playing == 69)
			al_set_sample_instance_gain(g_instance, 0.50);

	}
	else if ((!mode) && is_music_playing){
		printf("audio_lib: musica destru");
		destroy_music(g_instance);
		printf("ida\n");
	}

	 if (optn > pausa){
		 printf("pause music\n");
		al_reserve_samples(3);

		switch (optn) {
			case lose:
				g_music_effect = al_load_sample("lose.wav");
				is_music_playing = 1;
				break;
			case clr_lane_1:
				g_music_effect = al_load_sample("1_line_compl.wav");
				is_music_playing = 1;
			case clr_lane_2:
				g_music_effect = al_load_sample("2_line_compl");
				is_music_playing = 1;
			case clr_lane_3:
				g_music_effect = al_load_sample("3_line_compl.wav");
				is_music_playing = 1;
				break;
			case TETRIS:
				g_music_effect = al_load_sample("3_line_compl.wav");
				is_music_playing = 1;
				break;
			case chime:
				g_music_effect = al_load_sample("chime.wav");
				is_music_playing = 1;
				break;
		}
		g_instance_effect = al_create_sample_instance(g_music_effect);
		if(g_instance_effect == NULL){
			printf("music NULL!\n");
			exit(1);
		}
		al_attach_sample_instance_to_mixer(g_instance_effect, al_get_default_mixer());
		al_set_sample_instance_playmode(g_instance_effect, ALLEGRO_PLAYMODE_ONCE);
		al_play_sample_instance(g_instance_effect);
	}
}

void destroy_music (ALLEGRO_SAMPLE_INSTANCE * todestroy){
	if(todestroy == NULL){
		printf("destroy NULL\n");
		exit(1);
	}
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
