CC = clang
CFLAGS = -Wall -Werror -Wextra

all: hello

clean:
	rm -f hello hello.o

hello:
	$(CC) $(CFLAGS) -c hello.c
	$(CC) -o hello hello.o