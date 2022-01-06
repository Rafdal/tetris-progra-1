back: main_back.o game.o easy_timer.o teclado_trucho.o
	gcc -Wall main_back.o game.o easy_timer.o teclado_trucho.o -o back `pkg-config --libs allegro_ttf-5`

front: main_front.o
	gcc -Wall main_front.o -o front `pkg-config --libs allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 allegro_primitives-5`


main_front.o: main_front.c
	gcc -c -Wall main_front.c `pkg-config --cflags allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 allegro_primitives-5`

main_back.o: main_back.c game.o easy_timer.o
	gcc -c -Wall main_back.c


game.o: ./backend/game.c ./backend/game.h
	gcc -c -Wall ./backend/game.c

easy_timer.o: ./libs/easy_timer.c ./libs/easy_timer.h
	gcc -c -Wall ./libs/easy_timer.c

teclado_trucho.o: ./libs/teclado_trucho.c ./libs/teclado_trucho.h
	gcc -Wall -c `pkg-config --cflags allegro_ttf-5` ./libs/teclado_trucho.c


clean:
	rm *.o