#include "asgn2_helper_funcs.h"
#include "global.h"
#include "parsing.h"
#include "handler.h"
#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFERSIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return (EXIT_FAILURE);
    }

    char buf[BUFFERSIZE + 1] = { '\0' };

    Listener_Socket socket;
    int socketStatus = listener_init(&socket, atoi(argv[1]));
    if (socketStatus == -1) {
        fprintf(stderr, "Invalid Port\n");
        return (EXIT_FAILURE);
    }

    while (1) {
        int sockfd = listener_accept(&socket);
        if (sockfd == -1) {
            return (EXIT_FAILURE);
        }
        Request req;
        req.infd = sockfd;
        ssize_t bytesRead = read_until(sockfd, buf, BUFFERSIZE, "\r\n\r\n");
        if (parseRequest(&req, buf, bytesRead) != 1) {
            handleRequest(&req);
        }
        close(sockfd);
        memset(buf, '\0', sizeof(buf));
    }
    return (EXIT_SUCCESS);
}
