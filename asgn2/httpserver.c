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
#include <errno.h>

#define BUFFERSIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return (EXIT_FAILURE);
    }

    char buf[BUFFERSIZE + 1] = { '\0' };

    Listener_Socket socket;
    int port = strtol(argv[1], NULL, 10);
    if (errno == EINVAL) {
        fprintf(stderr, "Invalid Port\n");
        return (EXIT_FAILURE);
    }
    int socketStatus = listener_init(&socket, port);
    if (socketStatus == -1) {
        fprintf(stderr, "Invalid Port\n");
        return (EXIT_FAILURE);
    }

    while (1) {
        int sockfd = listener_accept(&socket);
        if (sockfd == -1) {
            fprintf(stderr, "Unable to Establish Connection\n");
            return (EXIT_FAILURE);
        }
        Request req;
        req.infd = sockfd;
        ssize_t bytesRead = read_until(sockfd, buf, BUFFERSIZE, "\r\n\r\n");
        if (bytesRead == -1) {
            dprintf(req.infd, "HTTP/1.1 400 Bad Request\r\nContent-Length: %d\r\n\r\nBad Request\n",
                12);
            return (EXIT_FAILURE);
        }
        if (parseRequest(&req, buf, bytesRead) != EXIT_FAILURE) {
            handleRequest(&req);
        }
        close(sockfd);
        memset(buf, '\0', sizeof(buf));
    }
    return (EXIT_SUCCESS);
}
