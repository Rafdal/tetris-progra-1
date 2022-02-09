linux: main_linux.o debug.o game.o l_easy_timer.o keyboard.o textblocks.o menu.o audio_lib.o
	gcc -Wall main_linux.o debug.o easy_timer.o keyboard.o game.o textblocks.o menu.o audio_lib.o -o linux `pkg-config --libs allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 allegro_primitives-5 allegro_font-5`

rasp: rpi_text.o menu.o main_rasp.o r_easy_timer.o joystick.o rpi_display.o game.o matrix_handler.o
	gcc -Wall rpi_text.o menu.o main_rasp.o joystick.o easy_timer.o rpi_display.o game.o matrix_handler.o ./libs/libAudioSDL1.o -lSDL ./libs/libAudioSDL2.o ./libs/joydrv.o ./libs/disdrv.o -L/usr/local/lib -lSDL2 -o rasp

push:
	git add . && git commit -m "make" && git push

db: main_db.o
	gcc -Wall main_db.o -o db

#################
# 	   MAINs	#
#################

main_rasp.o: main_rasp.c #RPI
	gcc -c -Wall main_rasp.c -lpthread

main_linux.o: main_linux.c	#LINUX
	gcc -c -Wall main_linux.c `pkg-config --cflags allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 allegro_primitives-5 allegro_font-5` 


main_db.o: main_db.c
	gcc -c -Wall main_db.c

#################
# 	LIBRERIAS 	#
#################

game.o: ./backend/game.c ./backend/game.h	#RPI Y LINUX
	gcc -c -Wall ./backend/game.c

menu.o:	./backend/menu.c ./backend/menu.h	#RPI Y LINUX
	gcc -c -Wall ./backend/menu.c

r_easy_timer.o: ./libs/easy_timer.c ./libs/easy_timer.h	#RPI
	gcc -c -Wall ./libs/easy_timer.c

l_easy_timer.o: ./frontend/easy_timer.c ./frontend/easy_timer.h	#LINUX
	gcc -c -Wall ./frontend/easy_timer.c

textblocks.o: ./frontend/textblocks.c ./frontend/textblocks.h	#LINUX
	gcc -c -Wall ./frontend/textblocks.c `pkg-config --cflags  allegro_ttf-5  allegro_primitives-5 allegro_font-5`

keyboard.o: ./frontend/keyboard.c ./frontend/keyboard.h		#LINUX
	gcc -c -Wall ./frontend/keyboard.c `pkg-config --cflags allegro-5`

joystick.o: ./libs/joystick.c ./libs/joystick.h		#RPI
	gcc -c -Wall ./libs/joystick.c

rpi_display.o: ./libs/rpi_display.c ./libs/rpi_display.h	#RPI
	gcc -c -Wall ./libs/rpi_display.c

matrix_handler.o: ./libs/matrix_handler.c ./libs/matrix_handler.h	#RPI
	gcc -c -Wall ./libs/matrix_handler.c

audio_lib.o: ./frontend/audio_lib.c ./frontend/audio_lib.h	#LINUX
	gcc -c -Wall ./frontend/audio_lib.c

#################
# libs LIBS 	#
#################

debug.o: debug/debug.c debug/debug.h	#
	gcc -c -Wall ./debug/debug.c

dispEmu.o: libs/disdrv.c libs/disdrv.h
	gcc -c -Wall ./libs/disdrv.c -o dispEmu.o

rpi_text.o: ./libs/rpi_text.c ./libs/rpi_text.h
	gcc -c -Wall ./libs/rpi_text.c

clean:
	rm *.o



### LIBRERIAS ALLEGRO PARA AGREGAR al comando pkg-config (si las necesita)

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
