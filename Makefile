CC = gcc
COMPILER_FLAGS = -Wall
STD = -std=c99
OFLAGS = -fsanitize-undefined-trap-on-error -fsanitize=undefined
LFLAGS = -lmingw32 -lSDL2main -lSDL2 -lm
CFLAGS = -Iinclude/SDL2 
SRC_FILES = ./src/*.c ./src/Display/*.c ./src/Vectors/*.c ./src/Mesh/*.c ./src/Triangle/*.c ./src/DArray/*.c ./src/Colors/*.c ./src/Matrix/*.c ./src/Light/*.c ./src/Texture/*.c ./src/Helper/Swap/*.c
OBJ_NAME = -o renderer

build:
	$(CC) $(OFLAGS) $(COMPILER_FLAGS) $(STD) $(CFLAGS) $(SRC_FILES) $(OBJ_NAME) $(LFLAGS)

run:
	./renderer.exe

clean:
	del renderer.exe
