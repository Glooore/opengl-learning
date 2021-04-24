CC=g++

CFLAGS=-lGL -lX11 -lglfw -lXi -ldl -Wall -I ../include/ -I /usr/include/

main: 
	$(CC) $(CFLAGS) -o render lighting_ex.cpp ../src/glad.c
	./render

clean:
	rm ./render
