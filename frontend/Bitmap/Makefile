all: main.o
	gcc -Wall main.o -o main `pkg-config --libs allegro_ttf-5`

main.o: main.c
	gcc -Wall -c main.c `pkg-config --cflags allegro_ttf-5`