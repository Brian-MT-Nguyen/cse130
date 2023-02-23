CC = clang
CFLAGS = -Wall -Wextra -Werror -pedantic
OBJS = queue.o

all: queue.o

queue.o: queue.c
	$(CC) $(CFLAGS) -c queue.c

clean:
	rm -f queue queue.o
