#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_acodec.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include "audio_lib.h"
//Extensiones con acodec .wav, .flac, .ogg, .it, .mod, .s3m, .xm. 


// static char is_music_playing;
// static ALLEGRO_SAMPLE *g_music = NULL;            			// Inicia el vessel para la musica
// static ALLEGRO_SAMPLE_INSTANCE *g_instance = NULL;			// Inicia el entorno para la musica			
// static ALLEGRO_SAMPLE *g_music_effect = NULL;				// Inicia el vessel para los sonidos
// static ALLEGRO_SAMPLE_INSTANCE *g_instance_effect = NULL;	// Inicia el entorno para los sonidos

#define AUDIO_SAMPLES 32

static uint8_t audio_counter=0;
static audio_t* audio_list[AUDIO_SAMPLES];
static audio_t* current_music=NULL;

// Instala las herramientas de audio necesarias para que funcione la libreria
// retorno:  False = ERROR | True = OK
bool audio_install (void){
	int i;
	for(i=0; i<AUDIO_SAMPLES; i++)
		audio_list[i] = NULL;

	if (!al_install_audio()) {	// Instalar el sistema de audio de allegro
		printf("failed to initialize audio!\n");
		return false;
	}

	if (!al_init_acodec_addon()) { // Iniciar el codec de audio de allegro
		printf("failed to initialize audio codecs!\n");
		return false;
	}

	if (!al_reserve_samples(AUDIO_SAMPLES)) { // Reservar "pistas" de musica
		printf("failed to reserve samples!\n");
		return false;
	}
	return true;
}

// Inicializa un objeto de audio (puntero a estructura audio_t)
// Retorna NULL en caso de error.
// Parametros: 
// - filename: nombre/path del archivo de audio (ej: "beep.wav")
// - gain: ganancia de la pista de audio (1.0 por defecto)
// - type: tipo de archivo de audio (AUDIO_MUSIC o AUDIO_EFFECT, el primero reproduce la pista en loop, el segundo una sola vez)
// Nota: En modo AUDIO_MUSIC solo se puede reproducir una pista a la vez, pero 
// 		pero en modo AUDIO_EFFECT se pueden reproducir varios efectos de sonido al mismo tiempo.
audio_t* audio_init(const char* filename, float gain, audio_type_t type){
	audio_t *audio = NULL;
	if(audio_counter<AUDIO_SAMPLES)					// si hay espacio para agregar una pista de audio
		audio = (audio_t*) malloc(sizeof(audio_t)); // Asignamos memoria al objeto de audio

	if(audio != NULL){
		audio->sample = al_load_sample(filename);	// Creamos una muestra de musica
		if(audio->sample != NULL){					// Si se inicializo correctamente
			audio->instance = al_create_sample_instance(audio->sample); // Creamos una instancia de la muestra
			if(audio->instance != NULL){
				audio->gain = gain;
				audio->status = AUDIO_STOP;			// Seteamos parametros de configuracion del objeto
				audio->type = type;
				audio->speed = 1; // velocidad normal

				if(audio_counter<AUDIO_SAMPLES-1)
					audio_list[audio_counter++] = audio; // Guardo el puntero en un arreglo (usado en la destruccion)
			}else{
				al_destroy_sample(audio->sample);	// Si la instancia no se inicializo, liberamos la muestra y la memoria de la estructura
				free(audio);
				audio = NULL;
				printf("instance of %s is NULL!\n", filename);
			}
		}else{
			printf("sample of %s is NULL! File error!\n", filename);
			free(audio);  // Si la muestra de audio no se inicio, liberamos la memoria de la  estructura
			audio = NULL;
		}
	}
	return audio;
}

// Reproduce una pista de audio. 
// OJO: Su comportamiento es diferente segun si el objeto de audio es del tipo:
//		AUDIO_EFFECT: Reproduce la pista de audio una sola vez
//		AUDIO_MUSIC: Reproduce una cancion en bucle. Detiene y reemplaza la cancion que se estaba reproduciendo anteriormente
void audio_play(audio_t* audio){
	if(audio != NULL){
		// if(audio->status == AUDIO_PLAY)
			// al_stop_sample_instance(audio->instance);
		
		// Si la instancia no esta anclada al mixer, la anclamos
		if(!al_get_sample_instance_attached(audio->instance)){
			al_attach_sample_instance_to_mixer(audio->instance, al_get_default_mixer());
			audio->status = AUDIO_STOP; // (reinicio la cancion por las dudas)
		}
		
		// Setear efecto
		if(audio->type == AUDIO_EFFECT){
			if(al_get_sample_instance_playing(audio->instance)) // Si el mismo efecto se estaba reproduciendo
				al_stop_sample_instance(audio->instance);	// Detenerlo para ejecutarlo otra vez

			// Ejecutar la pista de audio
			al_set_sample_instance_playmode(audio->instance, ALLEGRO_PLAYMODE_ONCE);
			al_set_sample_instance_gain(audio->instance, audio->gain);
			al_play_sample_instance(audio->instance);
		}

		// Setear musica
		if(audio->type == AUDIO_MUSIC){

			// Detener la musica actual en reproduccion (si es que hay)
			if(current_music != NULL && current_music != audio && current_music->status == AUDIO_PLAY){
				al_stop_sample_instance(current_music->instance);
				current_music->status = AUDIO_STOP;
			}

			// Si la musica no se esta reproduciendo, iniciar la reproduccion en modo bucle
			if(audio->status != AUDIO_PLAY || !al_get_sample_instance_playing(audio->instance)){
				audio->status = AUDIO_PLAY;
				current_music = audio;
				al_set_sample_instance_gain(audio->instance, audio->gain);
				al_set_sample_instance_playmode(audio->instance, ALLEGRO_PLAYMODE_LOOP);
				al_play_sample_instance(audio->instance);
			}
		}
	}
	else{
		printf(__FILE__": audio is null!\n");
	}
}

// Destruir todas las muestras e instancias de audio que hayan sido creadas, luego destruir los objetos de audio creados 
void audio_destroy(void){
	int i;
	for(i=0; i<AUDIO_SAMPLES; i++){
		if(audio_list[i] != NULL){
			al_stop_sample_instance(audio_list[i]->instance);
			al_detach_sample_instance(audio_list[i]->instance);
			al_destroy_sample_instance(audio_list[i]->instance);
			al_destroy_sample(audio_list[i]->sample);
			free(audio_list[i]);
			audio_list[i] = NULL;
		}
	}
	al_uninstall_audio();
}

// Retoran true si el audio se esta reproduciendo
bool audio_is_playing(audio_t* audio){
	if(audio != NULL){
		return al_get_sample_instance_playing(audio->instance);
	}
	return false;
}

// Setea la velocidad de reproduccion de la pista de audio
void audio_set_speed(audio_t* audio, float speed){
	if(audio != NULL && audio->speed != speed){
		printf("vel: %0.2f\n", speed);
		al_set_sample_instance_speed(audio->instance, speed);
		audio->speed = speed;
	}
}

// Si se esta reproduciendo el audio, lo detiene de manera forzada
void audio_force_stop(audio_t* audio){
	if(audio != NULL && al_get_sample_instance_playing(audio->instance)){
		al_stop_sample_instance(audio->instance);
		audio->status = AUDIO_STOP;
	}
}
/* void manage_music (char optn, char mode) {

	printf("music state = %d,  optn: %u, mode: %u\n", is_music_playing, optn, mode);

	if ((optn <= pausa ) && mode && !is_music_playing) {

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
			printf("MUSIC optn: %u NULL!\n", optn);
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
		switch (optn) {
			case lose:
				g_music_effect = al_load_sample("lose.wav");
				break;
			case clr_lane_1:
				g_music_effect = al_load_sample("1_line_compl.wav");
				break;
			case clr_lane_2:
				break;
				g_music_effect = al_load_sample("2_line_compl.wav");
			case clr_lane_3:
				g_music_effect = al_load_sample("3_line_compl.wav");
				break;
			case TETRIS:
				g_music_effect = al_load_sample("win.wav");
				break;
			case chime:
				g_music_effect = al_load_sample("chime.wav");
				break;
			case chime_select:
				g_music_effect = al_load_sample("chime_select.wav");
				break;
			default:
				printf("DEFAULT!\n");
				exit(1);
		}
		g_instance_effect = al_create_sample_instance(g_music_effect);
		if(g_music_effect == NULL || g_instance_effect == NULL){
			printf("music optn: %u NULL!\n", optn);
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
} */

