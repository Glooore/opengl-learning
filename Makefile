CC=g++

CFLAGS=-lGL -lX11 -lglfw -lXi -ldl -Wall -I ../include/ -I /usr/include/

main: main.cpp
	$(CC) $(CFLAGS) -o main main.cpp ../src/glad.c
	./main

triangle: triangle.cpp
	$(CC) $(CFLAGS) -o triangle triangle.cpp ../src/glad.c
	./triangle

shader_ex: shader_ex.cpp
	$(CC) $(CFLAGS) -o shader_ex shader_ex.cpp ../src/glad.c
	./shader_ex

texture_ex: texture_ex.cpp
	$(CC) $(CFLAGS) -o texture_ex texture_ex.cpp ../src/glad.c
	./texture_ex

transform_ex: transform_ex.cpp
	$(CC) $(CFLAGS) -o transform_ex transform_ex.cpp ../src/glad.c
	./transform_ex

coordinate_ex: coordinate_ex.cpp
	$(CC) $(CFLAGS) -o coordinate_ex coordinate_ex.cpp ../src/glad.c
	./coordinate_ex

camera_ex: camera_ex.cpp
	$(CC) $(CFLAGS) -o camera_ex camera_ex.cpp ../src/glad.c
	./camera_ex

lighting_ex: lighting_ex.cpp
	$(CC) $(CFLAGS) -o lighting_ex lighting_ex.cpp ../src/glad.c
	./lighting_ex
