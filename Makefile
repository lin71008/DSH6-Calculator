CC = gcc
FLAGS = -g -std=gnu99 -O0

WFLAGS = -o debug -Wall -Werror

FILENAME = main.c

build:
	${CC} ${FILENAME} ${FLAGS} $(WFLAGS)
# 	./debug
test:
	time ./debug.exe < sample_input > debug_output
	diff -B -w --color debug_output sample_output
