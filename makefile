CC = g++
CFLAGS = -Wall -g

pixelbrot: src/main.o
	$(CC) $(CFLAGS) -o pixelbrot src/main.o -lSDL2
