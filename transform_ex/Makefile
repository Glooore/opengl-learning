CC=g++

CFLAGS=-lGL -lX11 -lglfw -lXi -ldl -Wall -I ../include/ -I /usr/include/

main: main.cpp
	$(CC) $(CFLAGS) -o transform_ex transform_ex.cpp ../src/glad.c
	./transform_ex
