CC = clang
CFLAGS = -Wall -Wextra -Werror -pedantic -g
OBJS = httpserver.o parsing.o handler.o asgn2_helper_funcs.a

all: httpserver

httpserver: $(OBJS)
	$(CC) -o httpserver $(OBJS)

httpserver.o: httpserver.c
	$(CC) $(CFLAGS) -c httpserver.c

parsing.o: 
	$(CC) $(CFLAGS) -c parsing.c

handler.o: 
	$(CC) $(CFLAGS) -c handler.c

# response.o:
# 	$(CC) $(CFLAGS) -c response.c

clean:
	rm -f httpserver httpserver.o global.o parsing.o handler.o
