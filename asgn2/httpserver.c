#include "asgn2_helper_funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFERSIZE 2048
struct Request {
    char *cmd;
    char *targetPath;
    int contentLength;
} Request;

int main(int argc, char *argv[]) {
    if(argc != 2) {
        return(EXIT_FAILURE);
    }

    char buf[BUFFERSIZE + 1];
    fprintf(stdout, "%s", argv[1]);

    Listener_Socket socket;
    listener_init(&socket, atoi(argv[1]));

    while(1) {
        int darren = listener_accept(&socket);
        ssize_t bytes_read = read_until(darren, buf, BUFFERSIZE, "\r\n\r\n");
        write(STDOUT_FILENO, buf, bytes_read);
        if(darren == -1) {
            printf("oh ok");
            return(EXIT_SUCCESS);
        }
        close(darren);
    }
    return(EXIT_SUCCESS);
}
