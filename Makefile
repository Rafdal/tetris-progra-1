back: main_back.o game.o easy_timer.o teclado_trucho.o
	gcc -Wall main_back.o game.o easy_timer.o teclado_trucho.o -o back `pkg-config --libs allegro-5`

front: main_front.o game_screen.o
	gcc -Wall main_front.o game_screen.o -o front `pkg-config --libs allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 allegro_primitives-5`


main_front.o: main_front.c
	gcc -c -Wall main_front.c `pkg-config --cflags allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 allegro_primitives-5`

main_back.o: main_back.c game.o easy_timer.o
	gcc -c -Wall main_back.c


game_screen.o: ./frontend/game_screen.h ./frontend/game_screen.c
	gcc -c -Wall ./frontend/game_screen.c `pkg-config --cflags allegro-5`

game.o: ./backend/game.c ./backend/game.h
	gcc -c -Wall ./backend/game.c

easy_timer.o: ./libs/easy_timer.c ./libs/easy_timer.h
	gcc -c -Wall ./libs/easy_timer.c

teclado_trucho.o: ./libs/teclado_trucho.c ./libs/teclado_trucho.h
	gcc -Wall -c `pkg-config --cflags allegro-5` ./libs/teclado_trucho.c


clean:
	rm *.o


### LIBRERIAS ALLEGRO PARA AGREGAR al comando

# allegro_ttf-5                  allegro_ttf - Allegro game programming library, TrueType fonts addon
# allegro_color-5                allegro_color - Allegro game programming library, colors addon
# allegro_dialog-5               allegro_dialog - Allegro game programming library, native dialog addon
# allegro_video-5                allegro_video - Allegro game programming library, video player addon
# allegro_acodec-5               allegro_acodec - Allegro game programming library, audio codec addon
# allegro_main-5                 allegro_main - Allegro game programming library, magic main addon
# allegro_font-5                 allegro_font - Allegro game programming library, font addon
# allegro_memfile-5              allegro_memfile - Allegro game programming library, memory files addon
# allegro_image-5                allegro_image - Allegro game programming library, image I/O addon
# allegro-5                      allegro - Allegro game programming library
# allegro_physfs-5               allegro_physfs - Allegro game programming library, PhysicsFS addon
# allegro_primitives-5           allegro_primitives - Allegro game programming library, primitives addon
# allegro_audio-5                allegro_audio - Allegro game programming library, audio addon
