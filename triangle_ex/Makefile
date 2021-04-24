CC=g++

CFLAGS=-lGL -lX11 -lglfw -lXi -ldl -Wall -I ../include/ -I /usr/include/

main:
	$(CC) $(CFLAGS) -o triangle triangle.cpp ../src/glad.c
	./triangle
