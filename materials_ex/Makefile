# CC=g++
CC=clang++

CFLAGS=-lGL -lX11 -lglfw -lXi -ldl -Wall -I ../include/ -I /usr/include/

main: 
	$(CC) $(CFLAGS) -o render materials_ex.cpp ../src/glad.c
	./render

clean:
	rm ./render
