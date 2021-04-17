CC=g++

CFLAGS=-lGL -lX11 -lglfw -lXi -ldl -Wall -I ./include

triangle: main.cpp
	$(CC) $(CFLAGS) -o triangles main.cpp src/glad.c
	./triangles
