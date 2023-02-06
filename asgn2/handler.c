#include "handler.h"

int get(Request *req) {
    int fd;
    if ((fd = open(req->targetPath, O_RDONLY | O_DIRECTORY)) != -1) {
        dprintf(req->infd, "HTTP/1.1 403 Forbidden\r\nContent-Length: %d\r\n\r\nForbidden\n", 10);
        return (EXIT_FAILURE);
    }
    if ((fd = open(req->targetPath, O_RDONLY)) == -1) {
        dprintf(req->infd, "HTTP/1.1 404 Not Found\r\nContent-Length: %d\r\n\r\nNot Found\n", 10);
        return (EXIT_FAILURE);
    }
    struct stat st;
    stat(req->targetPath, &st);
    off_t size = st.st_size;

    dprintf(req->infd, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\n\r\n", size);
    pass_bytes(fd, req->infd, size);
    close(fd);
    return (EXIT_SUCCESS);
}

int put(Request *req) {
    int fd;
    int statusCode = 200;
    if ((fd = open(req->targetPath, O_WRONLY | O_DIRECTORY, 0666)) != -1) {
        dprintf(req->infd, "HTTP/1.1 403 Forbidden\r\nContent-Length: %d\r\n\r\nForbidden\n", 10);
        return (EXIT_FAILURE);
    }
    if ((fd = open(req->targetPath, O_WRONLY | O_CREAT | O_EXCL, 0666)) != -1) {
        statusCode = 201;
    } else if ((fd = open(req->targetPath, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
        dprintf(req->infd, "HTTP/1.1 404 Not Found\r\nContent-Length: %d\r\n\r\nNot Found\n", 10);
        return (EXIT_FAILURE);
    }

    int bytesWritten;
    bytesWritten = write_all(fd, req->msgBody, req->remainingBytes);
    int totalWritten = req->contentLength - req->remainingBytes;
    while (totalWritten > 0) {
        bytesWritten = pass_bytes(req->infd, fd, totalWritten);
        totalWritten = totalWritten - bytesWritten;
    }
    if (statusCode == 201) {
        dprintf(req->infd, "HTTP/1.1 201 Created\r\nContent-Length: %d\r\n\r\nCreated\n", 8);
    } else {
        dprintf(req->infd, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\nOK\n", 3);
    }
    close(fd);
    return (EXIT_SUCCESS);
}

int handleRequest(Request *req) {
    if (strncmp(req->version, "HTTP/1.1", 8) != 0) {
        dprintf(req->infd,
            "HTTP/1.1 505 Version Not Supported\r\nContent-Length: %d\r\n\r\nVersion Not "
            "Supported\n",
            22);
        return (EXIT_FAILURE);
    } else if (strncmp(req->cmd, "GET", 3) == 0) {
        return (get(req));
    } else if (strncmp(req->cmd, "PUT", 3) == 0) {
        return (put(req));
    } else {
        dprintf(req->infd,
            "HTTP/1.1 501 Not Implemented\r\nContent-Length: %d\r\n\r\nNot Implemented\n", 16);
        return (EXIT_FAILURE);
    }
}
