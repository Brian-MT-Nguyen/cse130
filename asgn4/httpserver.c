// Asgn 2: A simple HTTP server.
// By: Eugene Chou
//     Andrew Quinn
//     Brian Zhao

#include "asgn2_helper_funcs.h"
#include "connection.h"
#include "debug.h"
#include "response.h"
#include "request.h"
#include "queue.h"

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/stat.h>

#define OPTIONS "t:"
queue_t *q;
void handle_connection(int);

void handle_get(conn_t *);
void handle_put(conn_t *);
void handle_unsupported(conn_t *);
void *thread() {
    while (1) {
        int connfd = -1;
        queue_pop(q, (void **) (uintptr_t) &connfd);
        handle_connection(connfd);
        close(connfd);
    }
}

int main(int argc, char **argv) {

    // Parse Command Line Args
    int opt = 0;
    int nt = 4;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't':
            nt = atoi(optarg);
            if (nt < 0) {
                fprintf(stderr, "Invalid thread size.\n");
                exit(EXIT_FAILURE);
            }
            break;
        default: break;
        }
    }

    char *endptr = NULL;
    size_t port = (size_t) strtoull(argv[optind], &endptr, 10);
    if (endptr && *endptr != '\0') {
        warnx("invalid port number: %s", argv[optind]);
        return EXIT_FAILURE;
    }

    if (argc < 2) {
        warnx("wrong arguments: %s port_num", argv[optind]);
        fprintf(stderr, "usage: %s <port>\n", argv[optind]);
        return EXIT_FAILURE;
    }

    for (int i = optind + 1; i < argc; i++) {
        fprintf(stderr, "More extraneous arguments\n");
        return EXIT_FAILURE;
    }

    // Initialize socket for receiving connections
    signal(SIGPIPE, SIG_IGN);
    Listener_Socket sock;
    listener_init(&sock, port);

    // Make queue
    q = queue_new(nt);

    // Initialize threads
    pthread_t threads[nt];
    for (int i = 0; i < nt; i++) {
        pthread_create(&(threads[i]), NULL, thread, NULL);
    }

    // Dispatcher Main Thread
    while (1) {
        int connfd = listener_accept(&sock);
        queue_push(q, (void *) (uintptr_t) connfd);
    }

    // Join in case I make signal handler
    for (int i = 0; i < nt; i++) {
        pthread_join(threads[i], NULL);
    }
    return EXIT_SUCCESS;
}

void handle_connection(int connfd) {

    conn_t *conn = conn_new(connfd);

    const Response_t *res = conn_parse(conn);

    if (res != NULL) {
        conn_send_response(conn, res);
    } else {
        debug("%s", conn_str(conn));
        const Request_t *req = conn_get_request(conn);
        if (req == &REQUEST_GET) {
            handle_get(conn);
        } else if (req == &REQUEST_PUT) {
            handle_put(conn);
        } else {
            handle_unsupported(conn);
        }
    }

    conn_delete(&conn);
}

void handle_get(conn_t *conn) {

    char *uri = conn_get_uri(conn);
    const Response_t *res = NULL;

    // What are the steps in here?

    // 1. Open the file.
    // If  open it returns < 0, then use the result appropriately
    //   a. Cannot access -- use RESPONSE_FORBIDDEN
    //   b. Cannot find the file -- use RESPONSE_NOT_FOUND
    //   c. other error? -- use RESPONSE_INTERNAL_SERVER_ERROR
    // (hint: check errno for these cases)!
    int fd = open(uri, O_RDONLY);
    if (fd < 0) {
        debug("%s: %d", uri, errno);
        if (errno == EACCES) {
            res = &RESPONSE_FORBIDDEN;
            conn_send_response(conn, res);
            return;
        } else if (errno == ENOENT) {
            res = &RESPONSE_NOT_FOUND;
            conn_send_response(conn, res);
            return;
        } else {
            res = &RESPONSE_INTERNAL_SERVER_ERROR;
            conn_send_response(conn, res);
            return;
        }
    }

    // 2. Get the size of the file.
    // (hint: checkout the function fstat)!

    // Get the size of the file.
    struct stat finfo;
    fstat(fd, &finfo);
    off_t size = finfo.st_size;

    // 3. Check if the file is a directory, because directories *will*
    // open, but are not valid.
    // (hint: checkout the macro "S_IFDIR", which you can use after you call fstat!)
    if (S_ISDIR(finfo.st_mode) == 1) {
        res = &RESPONSE_FORBIDDEN;
        conn_send_response(conn, res);
        close(fd);
        return;
    }

    // 4. Send the file
    // (hint: checkout the conn_send_file function!)
    res = conn_send_file(conn, fd, size);
    if (res == NULL) {
        res = &RESPONSE_OK;
        conn_send_response(conn, res);
    }
    close(fd);
    return;
}

void handle_unsupported(conn_t *conn) {
    debug("handling unsupported request");

    // send responses
    conn_send_response(conn, &RESPONSE_NOT_IMPLEMENTED);
}

void handle_put(conn_t *conn) {

    char *uri = conn_get_uri(conn);
    const Response_t *res = NULL;
    debug("handling put request for %s", uri);

    // Check if file already exists before opening it.
    bool existed = access(uri, F_OK) == 0;
    debug("%s existed? %d", uri, existed);

    // Open the file..
    int fd = open(uri, O_CREAT | O_TRUNC | O_WRONLY, 0600);
    if (fd < 0) {
        debug("%s: %d", uri, errno);
        if (errno == EACCES || errno == EISDIR || errno == ENOENT) {
            res = &RESPONSE_FORBIDDEN;
            conn_send_response(conn, res);
            return;
        } else {
            res = &RESPONSE_INTERNAL_SERVER_ERROR;
            conn_send_response(conn, res);
            return;
        }
    }

    res = conn_recv_file(conn, fd);

    if (res == NULL && existed) {
        res = &RESPONSE_OK;
        conn_send_response(conn, res);
    } else if (res == NULL && !existed) {
        res = &RESPONSE_CREATED;
        conn_send_response(conn, res);
    }

    close(fd);
    return;
}
