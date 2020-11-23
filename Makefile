CC = gcc
FLAGS = -g -std=gnu99 -O0

WFLAGS = #-Wall -Werror


build:
	${CC} -c main.c -o main.o ${FLAGS} $(WFLAGS)
	${CC} -c functional.c -o functional.o ${FLAGS} $(WFLAGS)
	${CC} functional.o main.o -o debug.exe ${FLAGS} $(WFLAGS)
test:
	time ./debug.exe < sample_input
