CC := gcc
CFLAGS := -std=gnu99 -O0 -g -Wall -Werror

OBJECT := $(patsubst %.c,%.o,$(wildcard *.c))

all: $(OBJECT)
	$(CC) $(CFLAGS) $(OBJECT) -o main

$(OBJECT): %o:%c
	$(CC) -c $(CFLAGS) $< -o $@

test:
	time ./main.exe < sample_input > test_output
	diff -B -w --color test_output sample_output

clean:
	rm $(OBJECT) *.exe
	rm test_output
