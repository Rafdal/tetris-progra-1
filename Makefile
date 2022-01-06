all: main.o game.o easytimer.o teclado_trucho.o
	gcc -Wall main.o game.o easytimer.o teclado_trucho.o -o main `pkg-config --libs allegro_ttf-5`

main.o: main.c game.o easytimer.o
	gcc -c -Wall main.c

game.o: game.c game.h
	gcc -c -Wall game.c

easy_timer.o: easy_timer.c easy_timer.h
	gcc -c -Wall easy_timer.c

teclado_trucho.o: teclado_trucho.c teclado_trucho.h
	gcc -Wall -c `pkg-config --cflags allegro_ttf-5` teclado_trucho.c