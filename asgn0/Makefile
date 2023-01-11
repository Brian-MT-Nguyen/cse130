CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -O2 -g

all: hello

clean:
	rm -f hello hello.o

hello: hello.o hello.c
	$(CC) $(CFLAGS) -c hello.c
	$(CC) -o hello hello.o