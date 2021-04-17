CC=g++

CFLAGS=-lGL -lX11 -lglfw -lXi -ldl -Wall -I ./include

main: main.cpp
	$(CC) $(CFLAGS) -o main main.cpp src/glad.c
	./main

triangle: triangle.cpp
	$(CC) $(CFLAGS) -o triangle triangle.cpp src/glad.c
	./triangle

shaders: shaders.cpp
	$(CC) $(CFLAGS) -o shaders shaders.cpp src/glad.c
	./shaders
