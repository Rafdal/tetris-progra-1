all: main.o game.o easytimer.o
	gcc -Wall main.o game.o easytimer.o -o main

main.o: main.c game.o easytimer.o
	gcc -c -Wall main.c

game.o: game.c game.h
	gcc -c -Wall game.c

easytimer.o: easytimer.c easytimer.h
	gcc -c -Wall easytimer.c