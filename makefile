CC = g++
CFLAGS = -Wall -g

pixelbrot: src/main.o src/util.o src/util.h src/pixel.h
	$(CC) $(CFLAGS) -o pixelbrot src/main.o src/util.o -lSDL2
