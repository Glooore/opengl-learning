CC=g++

CFLAGS=-lGL -lX11 -lglfw -lXi -ldl -Wall -I ./include/

main: main.cpp
	$(CC) $(CFLAGS) -o main main.cpp src/glad.c
	./main

triangle: triangle.cpp
	$(CC) $(CFLAGS) -o triangle triangle.cpp src/glad.c
	./triangle

shader_ex: shader_ex.cpp
	$(CC) $(CFLAGS) -o shader_ex shader_ex.cpp src/glad.c
	./shader_ex

texture_ex: texture_ex.cpp
	$(CC) $(CFLAGS) -o texture_ex texture_ex.cpp src/glad.c
	./texture_ex
