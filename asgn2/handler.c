#include "handler.h"

int get(Request *req,  char *buffer) {
    int fd;
    if ((fd = open(req->targetPath, O_RDONLY)) == -1) {
        fprintf(stderr, "Invalid Command\n");
        return (EXIT_FAILURE);
    }
    int readBytes;
    while ((readBytes = read(fd, buffer, sizeof(buffer))) > 0) {
        // Write file readBytes amount of contents to stdout
        int writtenBytes, totalWritten = 0;
        while (totalWritten < readBytes) {
            writtenBytes = write(STDOUT_FILENO, buffer + totalWritten, readBytes - totalWritten);
            if (writtenBytes == -1) {
                fprintf(stderr, "Operation Failed\n");
                close(fd);
                return (EXIT_FAILURE);
            }
            totalWritten += writtenBytes;
        }
    }

    // Error if read is bad
    if (readBytes == -1) {
        fprintf(stderr, "Invalid Command\n");
        close(fd);
        return (EXIT_FAILURE);
    }
    return(0);
}

int put(Request *req) {
    fprintf(stdout, "We in B)%s", req->cmd);
    return(0);
}
int handleRequest(Request *req,  char *buffer) {
    if(strncmp(req->cmd, "GET", 3) == 0) {
        return (get(req, buffer));
    }
    else if (strncmp(req->cmd, "PUT", 3) == 0) {
        return (put(req));
    }
    else {
        return(1);
    }
}
