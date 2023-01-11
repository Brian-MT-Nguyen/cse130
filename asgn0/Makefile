CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -O2

all: hello

clean:
	rm -f hello hello.o

hello:
	$(CC) $(CFLAGS) -c hello.c
	$(CC) -o hello hello.o