#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_acodec.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include 'audio_lib.h'
//Extensiones con acodec .wav, .flac, .ogg, .it, .mod, .s3m, .xm. 



	enum {
	game;
	menu;
	win;
	nxt_lvl;
};



int main(void) {
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_SAMPLE *sample0 = NULL;
	ALLEGRO_SAMPLE *sample1 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance0 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance1 = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	bool display_close = false;

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	if (!al_install_audio()) {
		fprintf(stderr, "failed to initialize audio!\n");
		return -1;
	}

	if (!al_init_acodec_addon()) {
		fprintf(stderr, "failed to initialize audio codecs!\n");
		return -1;
	}

	if (!al_reserve_samples(1)) {
		fprintf(stderr, "failed to reserve samples!\n");
		return -1;
	}
///////////////////////////////////////////////
	al_reserve_samples(2);

	sample0 = al_load_sample("audio.wav");
	sample1 = al_load_sample("audio2.wav");

	instance0 = al_create_sample_instance(sample0);
	instance1 = al_create_sample_instance(sample1);
	al_attach_sample_instance_to_mixer(instance0, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instance1, al_get_default_mixer());

////////////////////////////////////////////////
	if (!sample0) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}

	display = al_create_display(640, 480);

	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	event_queue = al_create_event_queue();

	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));
	//Loop the sample until the display closes.


	al_play_sample_instance(instance0);
	al_play_sample_instance(instance1);

	while (!display_close) {
		ALLEGRO_EVENT ev;
		if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola, VER RETURN EN DOCUMENT.
		{
			if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				display_close = true;
		}

	}

	al_uninstall_audio();
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_sample(sample0);
	al_destroy_sample_instance(instance0);
	al_destroy_sample_instance(instance1);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////

void* play_music (char optn){
	al_reserve_samples(2);
	switch{
		case game:{
			ALLEGRO_SAMPLE *g_music = NULL;
			ALLEGRO_SAMPLE_INSTANCE *g_instance = NULL;
			g_instance = al_create_sample_instance(g_music);
			al_attach_sample_instance_to_mixer(instance0, al_get_default_mixer());
			al_play_sample_instance(g_instance);
			break;
		}
	}
	return g_instance;
}

void destroy_music (void* todestroy){

	ALLEGRO_SAMPLE_INSTANCE destroyed = (ALLEGRO_SAMPLE_INSTANCE)todestroy;
	al_destroy_sample_instance(destroyed);

}
