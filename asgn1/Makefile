CC = clang
CFLAGS = -Wall -Wextra -Werror -pedantic

all: memory

clean:
	rm -f memory memory.o

memory:
	$(CC) $(CFLAGS) -c memory.c
	$(CC) -o memory memory.o
