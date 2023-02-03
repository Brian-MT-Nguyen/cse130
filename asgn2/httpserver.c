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
    if(argc != 2) {
        return(EXIT_FAILURE);
    }

    char buf[BUFFERSIZE + 1] = { '\0' };

    Listener_Socket socket;
    listener_init(&socket, atoi(argv[1]));

    while(1) {
        int sockfd = listener_accept(&socket);
        if(sockfd == -1) {
            return(EXIT_FAILURE);
        }
        Request req;
        req.infd = sockfd;
        ssize_t bytes_read = read_until(sockfd, buf, BUFFERSIZE, "\r\n\r\n");
        if(bytes_read > 2048) {
            return(EXIT_FAILURE);
        }
        parseRequest(&req, buf);
        //fprintf(stdout, "cmd: %s\nPath: %s\nVersion: %s\nContent Length: %d\nMessage Body: %s\n", req.cmd, req.targetPath, req.version, req.contentLength, req.msgBody);
        handleRequest(&req, buf);
        close(sockfd);
        memset(buf, '\0', sizeof(buf));
    }
    return(EXIT_SUCCESS);
}
