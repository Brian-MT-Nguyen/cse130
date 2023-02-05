#include "handler.h"

int get(Request *req,  char *buffer) {
    int fd;
    if ((fd = open(req->targetPath, O_RDONLY)) == -1) {
        fprintf(stderr, "Invalid Command\n");
        return (EXIT_FAILURE);
    }
    int readBytes;
    int totalRead = 0;
    while ((readBytes = read(fd, buffer, sizeof(buffer))) > 0) {
        // Write file readBytes amount of contents to stdout
        totalRead = readBytes + totalRead;
    }

    // Error if read is bad
    if (readBytes == -1) {
        fprintf(stderr, "Invalid Command\n");
        close(fd);
        return (EXIT_FAILURE);
    }

    lseek(fd, 0, SEEK_SET);
    dprintf(req->infd, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", totalRead);
    pass_bytes(fd, req->infd, totalRead);
    return(0);
}

int put(Request *req, char *buffer) {
    int fd;
    if((fd = open(req->targetPath, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
        fprintf(stderr, "Invalid Command\n");
        return (EXIT_FAILURE);
    }
    write_all(fd, req->msgBody, strlen(req->msgBody));
    int readBytes;
    int totalRead = req->contentLength - strlen(req->msgBody);
    while (totalRead > 0) {
        // Write file readBytes amount of contents to stdout
        readBytes = read(req->infd, buffer, sizeof(buffer));
        write_all(fd, buffer, readBytes);
        totalRead = totalRead - readBytes;
    }
    dprintf(req->infd, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\nOK\n", 3);
    return(0);
}
int handleRequest(Request *req,  char *buffer) {
    if(strncmp(req->cmd, "GET", 3) == 0) {
        return (get(req, buffer));
    }
    else if (strncmp(req->cmd, "PUT", 3) == 0) {
        return (put(req, buffer));
    }
    else {
        return(1);
    }
}
