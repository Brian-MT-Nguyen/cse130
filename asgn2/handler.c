#include "handler.h"

int get(Request *req) {
    // Check if GET has Content Length and/or Msg body and responds with bad request
    if (req->contentLength != -1) {
        dprintf(
            req->infd, "HTTP/1.1 400 Bad Request\r\nContent-Length: %d\r\n\r\nBad Request\n", 12);
        return (EXIT_FAILURE);
    }
    if (req->remainingBytes > 0) {
        dprintf(
            req->infd, "HTTP/1.1 400 Bad Request\r\nContent-Length: %d\r\n\r\nBad Request\n", 12);
        return (EXIT_FAILURE);
    }

    int fd;
    if ((fd = open(req->targetPath, O_RDONLY | O_DIRECTORY)) != -1) {
        dprintf(req->infd, "HTTP/1.1 403 Forbidden\r\nContent-Length: %d\r\n\r\nForbidden\n", 10);
        return (EXIT_FAILURE);
    }
    if ((fd = open(req->targetPath, O_RDONLY)) == -1) {
        if (errno == ENOENT) {
            dprintf(
                req->infd, "HTTP/1.1 404 Not Found\r\nContent-Length: %d\r\n\r\nNot Found\n", 10);
        } else if (errno == EACCES) {
            dprintf(
                req->infd, "HTTP/1.1 403 Forbidden\r\nContent-Length: %d\r\n\r\nForbidden\n", 10);
        } else {
            dprintf(req->infd,
                "HTTP/1.1 500 Internal Server Error\r\nContent-Length: %d\r\n\r\nInternal Server "
                "Error\n",
                22);
        }
        return (EXIT_FAILURE);
    }
    struct stat st;
    fstat(fd, &st);
    off_t size = st.st_size;

    dprintf(req->infd, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\n\r\n", size);
    int bytesWritten = pass_bytes(fd, req->infd, size);
    if (bytesWritten == -1) {
        dprintf(req->infd,
            "HTTP/1.1 500 Internal Server Error\r\nContent-Length: %d\r\n\r\nInternal Server "
            "Error\n",
            22);
        return (EXIT_FAILURE);
    }
    close(fd);
    return (EXIT_SUCCESS);
}

int put(Request *req) {
    if (req->contentLength == -1) {
        dprintf(
            req->infd, "HTTP/1.1 400 Bad Request\r\nContent-Length: %d\r\n\r\nBad Request\n", 12);
        return (EXIT_FAILURE);
    }
    int fd;
    int statusCode = 0;
    // Checks for Directory
    if ((fd = open(req->targetPath, O_WRONLY | O_DIRECTORY, 0666)) != -1) {
        dprintf(req->infd, "HTTP/1.1 403 Forbidden\r\nContent-Length: %d\r\n\r\nForbidden\n", 10);
        return (EXIT_FAILURE);
    }

    // Checks if File exists if not create it
    if ((fd = open(req->targetPath, O_WRONLY | O_CREAT | O_EXCL, 0666)) == -1) {
        if (errno == EEXIST) {
            statusCode = 200;
        } else if (errno == EACCES) {
            dprintf(
                req->infd, "HTTP/1.1 403 Forbidden\r\nContent-Length: %d\r\n\r\nForbidden\n", 10);
            return (EXIT_FAILURE);
        } else {
            dprintf(req->infd,
                "HTTP/1.1 500 Internal Server Error\r\nContent-Length: %d\r\n\r\nInternal Server "
                "Error\n",
                22);
            return (EXIT_FAILURE);
        }
    } else if (fd != -1) {
        statusCode = 201;
    }

    // If file exists
    if (statusCode == 200) {
        if ((fd = open(req->targetPath, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
            if (errno == EACCES) {
                dprintf(req->infd,
                    "HTTP/1.1 403 Forbidden\r\nContent-Length: %d\r\n\r\nForbidden\n", 10);
                return (EXIT_FAILURE);
            } else {
                dprintf(req->infd,
                    "HTTP/1.1 500 Internal Server Error\r\nContent-Length: %d\r\n\r\nInternal "
                    "Server Error\n",
                    22);
                return (EXIT_FAILURE);
            }
        }
    }

    int bytesWritten = write_all(fd, req->msgBody, req->remainingBytes);
    if (bytesWritten == -1) {
        dprintf(req->infd,
            "HTTP/1.1 500 Internal Server Error\r\nContent-Length: %d\r\n\r\nInternal Server "
            "Error\n",
            22);
        return (EXIT_FAILURE);
    }

    int totalWritten = req->contentLength - req->remainingBytes;
    bytesWritten = pass_bytes(req->infd, fd, totalWritten);
    if (bytesWritten == -1) {
        dprintf(req->infd,
            "HTTP/1.1 500 Internal Server Error\r\nContent-Length: %d\r\n\r\nInternal Server "
            "Error\n",
            22);
        return (EXIT_FAILURE);
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
