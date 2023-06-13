CC = gcc
COMPILER_FLAGS = -Wall
STD = -std=c99
OFLAGS = -fsanitize-undefined-trap-on-error -fsanitize=undefined
LFLAGS = -lmingw32 -lSDL2main -lSDL2
CFLAGS = -Iinclude/SDL2 
SRC_FILES = ./src/*.c
OBJ_NAME = -o renderer

build:
	$(CC) $(OFLAGS) $(COMPILER_FLAGS) $(STD) $(CFLAGS) $(SRC_FILES) $(OBJ_NAME) $(LFLAGS)

run:
	./renderer.exe

clean:
	del renderer.exe