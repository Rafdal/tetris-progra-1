test: main_test.o menu.o dispEmu.o t_rpi_display.o t_easy_timer.o t_matrix_handler.o teclado_trucho.o game.o
	gcc -Wall main_test.o menu.o dispEmu.o easy_timer.o rpi_display.o teclado_trucho.o game.o matrix_handler.o -o test `pkg-config --libs allegro-5 allegro_audio-5 allegro_acodec-5`

back: main_back.o game.o easy_timer.o teclado_trucho.o
	gcc -Wall main_back.o game.o easy_timer.o teclado_trucho.o -o back `pkg-config --libs allegro-5`

linux: main_linux.o game.o l_easy_timer.o keyboard.o
	gcc -Wall main_linux.o easy_timer.o keyboard.o game.o -o linux `pkg-config --libs allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 allegro_primitives-5 allegro_font-5` 

rasp: main_rasp.o easy_timer.o joystick.o rpi_display.o game.o matrix_handler.o
	gcc -Wall main_rasp.o joystick.o easy_timer.o rpi_display.o game.o matrix_handler.o ./libs/joydrv.o ./libs/disdrv.o -o rasp


#################
# 	   MAINs	#
#################

main_test.o: main_test.c
	gcc -c -Wall main_test.c `pkg-config --cflags allegro-5`

main_rasp.o: main_rasp.c
	gcc -c -Wall main_rasp.c

main_linux.o: main_linux.c
	gcc -c -Wall main_linux.c `pkg-config --cflags allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 allegro_primitives-5 allegro_font-5` 

main_back.o: main_back.c
	gcc -c -Wall main_back.c

#################
# 	LIBRERIAS 	#
#################

game.o: ./backend/game.c ./backend/game.h
	gcc -c -Wall ./backend/game.c

menu.o:	./backend/menu.c ./backend/menu.h
	gcc -c -Wall ./backend/menu.c


easy_timer.o: ./testing/easy_timer.c ./testing/easy_timer.h
	gcc -c -Wall ./testing/easy_timer.c

l_easy_timer.o: ./frontend/easy_timer.c ./frontend/easy_timer.h
	gcc -c -Wall ./frontend/easy_timer.c

keyboard.o: ./frontend/keyboard.c ./frontend/keyboard.h
	gcc -c -Wall ./frontend/keyboard.c `pkg-config --cflags allegro-5`

joystick.o: ./libs/joystick.c ./libs/joystick.h
	gcc -c -Wall ./libs/joystick.c

rpi_display.o: ./libs/rpi_display.c ./libs/rpi_display.h
	gcc -c -Wall ./libs/rpi_display.c

matrix_handler.o: ./libs/matrix_handler.c ./libs/matrix_handler.h
	gcc -c -Wall ./libs/matrix_handler.c

#################
# TESTING LIBS 	#
#################

t_matrix_handler.o: ./testing/matrix_handler.c ./testing/matrix_handler.h
	gcc -c -Wall ./testing/matrix_handler.c

t_easy_timer.o: ./testing/easy_timer.c ./testing/easy_timer.h
	gcc -c -Wall ./testing/easy_timer.c

t_rpi_display.o: ./testing/rpi_display.c ./testing/rpi_display.h
	gcc -c -Wall ./testing/rpi_display.c

teclado_trucho.o: ./testing/joystick.c ./testing/joystick.h
	gcc -Wall -c ./testing/joystick.c -o teclado_trucho.o `pkg-config --cflags allegro-5`

dispEmu.o: ./testing/disdrv.c ./testing/disdrv.h
	gcc -c -Wall ./testing/disdrv.c -o dispEmu.o

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
