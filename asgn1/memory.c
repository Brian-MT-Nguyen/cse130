#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>

int main(void) {
    // Universal buffer for all
    char buf[8192] = { '\0' };

    int commandSize = 4;
    int locationSize = PATH_MAX + 1024;

    int readBytes = 1;
    int totalRead = 0;
    while ((totalRead < commandSize) && (readBytes > 0)) {
        readBytes = read(STDIN_FILENO, buf + totalRead, commandSize - totalRead);
        totalRead += readBytes;
    }

    if (buf[3] == ' ') {
        buf[3] = '\0';
    } else {
        fprintf(stderr, "Invalid Command\n");
        return (EXIT_FAILURE);
    }

    // If command is get
    if (strcmp(buf, "get") == 0) {
        readBytes = 1;
        totalRead = 0;
        while ((totalRead < (locationSize - 1)) && (readBytes > 0)) {
            readBytes = read(STDIN_FILENO, buf + totalRead, locationSize - totalRead - 1);
            totalRead += readBytes;
        }
        buf[locationSize] = '\0';

        // Check for newlines and contents after buffer
        int i;
        for (i = 0; buf[i] != '\n'; i++) {
            if (buf[i] == ' ' || buf[i] == '\0') {
                fprintf(stderr, "Invalid Command\n");
                return (EXIT_FAILURE);
            }
        }
        buf[i] = '\0';

        for (i = i + 1; i < locationSize; i++) {
            if (buf[i] != '\0') {
                fprintf(stderr, "Invalid Command\n");
                return (EXIT_FAILURE);
            }
        }

        // Open file for reading and check for validity
        int fd;
        if ((fd = open(buf, O_RDONLY)) == -1) {
            fprintf(stderr, "Invalid Command\n");
            return (EXIT_FAILURE);
        }

        // Write file contents to stdout
        int readBytes;
        while ((readBytes = read(fd, buf, sizeof(buf))) > 0) {
            int writtenBytes, totalWritten = 0;
            while (totalWritten < readBytes) {
                writtenBytes = write(STDOUT_FILENO, buf + totalWritten, readBytes - totalWritten);
                if (writtenBytes == -1) {
                    fprintf(stderr, "Operation Failed\n");
                    close(fd);
                    return (EXIT_FAILURE);
                }
                totalWritten += writtenBytes;
            }
        }
        if (readBytes == -1) {
            fprintf(stderr, "Invalid Command\n");
            close(fd);
            return (EXIT_FAILURE);
        }

        // Close file and exit
        close(fd);
        return (EXIT_SUCCESS);
    }

    // If command is set
    else if (strcmp(buf, "set") == 0) {
        readBytes = 1;
        totalRead = 0;
        while ((totalRead < (locationSize - 1)) && (readBytes > 0)) {
            readBytes = read(STDIN_FILENO, buf + totalRead, locationSize - totalRead - 1);
            totalRead += readBytes;
        }
        buf[locationSize] = '\0';

        // Check for newline
        int i;
        for (i = 0; buf[i] != '\n'; i++) {
            if (buf[i] == ' ' || buf[i] == '\0') {
                fprintf(stderr, "Invalid Command\n");
                return (EXIT_FAILURE);
            }
        }

        buf[i] = '\0';

        // Open file for writing and check for validity
        int fd;
        if ((fd = open(buf, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
            fprintf(stderr, "Invalid Command\n");
            return (EXIT_FAILURE);
        }

        // Write contents to buffer from initial read() call for first line.
        int writtenBytes = 0;
        int totalWritten = 0;
        totalRead = totalRead - i - 1;
        char *contents = buf + i + 1;
        while (totalWritten < totalRead) {
            writtenBytes = write(fd, contents + totalWritten, totalRead - totalWritten);
            if (writtenBytes == -1) {
                fprintf(stderr, "Operation Failed\n");
                close(fd);
                return (EXIT_FAILURE);
            }
            totalWritten += writtenBytes;
        }

        // Continue to read if user would like to input more than initial input
        while ((readBytes = read(STDIN_FILENO, buf, 8192)) > 0) {
            int writtenBytes, totalWritten = 0;
            while (totalWritten < readBytes) {
                writtenBytes = write(fd, buf + totalWritten, readBytes - totalWritten);
                if (writtenBytes == -1) {
                    fprintf(stderr, "Operation Failed\n");
                    close(fd);
                    return (EXIT_FAILURE);
                }
                totalWritten += writtenBytes;
            }
        }

        if (readBytes == -1) {
            fprintf(stderr, "Invalid Command\n");
            close(fd);
            return (EXIT_FAILURE);
        }

        // Write OK\n to stdout
        write(STDOUT_FILENO, "OK\n", sizeof(char) * 3);

        // Close file and exit
        close(fd);
        return (EXIT_SUCCESS);
    }

    // If command is invalid
    else {
        fprintf(stderr, "Invalid Command\n");
        return (EXIT_FAILURE);
    }
}
