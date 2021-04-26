CC=g++
# CC=clang++

CFLAGS=-lGL -lX11 -lglfw -lXi -ldl -Wall -I ../include

main:
	$(CC) $(CFLAGS) -o camera_ex camera_ex.cpp ../src/glad.c
	./camera_ex
