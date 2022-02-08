#ifndef LIBDANY_H
#define LIBDANY_H


enum status { 
			  NO_INIT,
			  READY,
			  STOPPED,
			  PLAYING,
			  PAUSED,
			  FINISHED
			};

void debug_status();

// ============ Colored Text ===========

#define BLACK_TEXT		   "\e[30;1m"
#define RED_TEXT		   "\e[31;1m"
#define GREEN_TEXT 		   "\e[32;1m"
#define YELLOW_TEXT                "\e[33;1m"
#define BLUE_TEXT 		   "\e[34;1m"
#define MAGENTA_TEXT 	           "\e[35;1m"
#define CYAN_TEXT 	   	   "\e[36;1m"
#define WHITE_TEXT 		   "\e[37m"

// ============ Colored Text End ===========


	
int play_sound(void);
int pause_sound(void);
int end_play (void);
int player_status(void);
int init_sound(void);
int set_file_to_play(const char *music_path);
int stop_sound(void);

#endif
