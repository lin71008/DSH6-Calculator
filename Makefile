CC := gcc
FLAGS := -std=gnu99 -O0
WFLAGS := -g -Wall -Werror

FILENAME := main.c

build:
	${CC} ${FILENAME} ${FLAGS} $(WFLAGS)
# 	./debug
test:
	time ./debug.exe < sample_input > debug_output
	diff -B -w --color debug_output sample_output
