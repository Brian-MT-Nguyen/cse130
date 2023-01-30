CC = clang
CFLAGS = -Wall -Wextra -Werror -pedantic
OBJS = httpserver.o asgn2_helper_funcs.a

all: httpserver

httpserver: $(OBJS)
	$(CC) -o httpserver $(OBJS)

httpserver.o: httpserver.c
	$(CC) $(CFLAGS) -c httpserver.c

clean:
	rm -f httpserver httpserver.o
