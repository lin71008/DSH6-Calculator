CC = gcc
FLAGS = -g -std=gnu99 -O0

WFLAGS = -o debug -Wall -Werror

FILENAME = main.c

make:
	rm debug.exe
	${CC} ${FILENAME} ${FLAGS} $(WFLAGS)
	./debug
