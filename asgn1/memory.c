#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>

int main(void) {
    // Universal buffer for all
    char buf[8192];

    // Max sizes for command and location
    int commandSize = 4;
    int locationSize = PATH_MAX + 1024;

    // Read in for command
    int readBytes = 1;
    int totalRead = 0;
    while ((totalRead < commandSize) && (readBytes > 0)) {
        readBytes = read(STDIN_FILENO, buf + totalRead, commandSize - totalRead);
        totalRead += readBytes;
    }

    // Place null char at the space char to feed to command into strcmp (error if space isnt there)
    if (buf[3] == ' ') {
        buf[3] = '\0';
    } else {
        fprintf(stderr, "Invalid Command\n");
        return (EXIT_FAILURE);
    }

    // If command is get
    if (strcmp(buf, "get") == 0) {
        // Read in for location
        readBytes = 1;
        totalRead = 0;
        while ((totalRead < (locationSize - 1)) && (readBytes > 0)) {
            readBytes = read(STDIN_FILENO, buf + totalRead, locationSize - totalRead - 1);
            totalRead += readBytes;
        }

        // Place null char where PATH_MAX ends for string functions
        buf[locationSize] = '\0';

        // Check for newlines and contents after buffer
        int i;
        for (i = 0; buf[i] != '\n'; i++) {
            if (buf[i] == ' ' || buf[i] == '\0') {
                fprintf(stderr, "Invalid Command\n");
                return (EXIT_FAILURE);
            }
        }

        // Place null char where newline is for filename
        buf[i] = '\0';

        // Check for contents after the buffer
        if ((totalRead - (i + 1)) > 0) {
            fprintf(stderr, "Invalid Command\n");
            return (EXIT_FAILURE);
        }

        // Open file for reading and check for validity
        int fd;
        if ((fd = open(buf, O_RDONLY)) == -1) {
            fprintf(stderr, "Invalid Command\n");
            return (EXIT_FAILURE);
        }

        // Read file contents to stdout
        int readBytes;
        while ((readBytes = read(fd, buf, sizeof(buf))) > 0) {
            // Write file readBytes amount of contents to stdout
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

        // Error if read is bad
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
        // Read in for location
        readBytes = 1;
        totalRead = 0;
        while ((totalRead < (locationSize - 1)) && (readBytes > 0)) {
            readBytes = read(STDIN_FILENO, buf + totalRead, locationSize - totalRead - 1);
            totalRead += readBytes;
        }

        // Place null char where PATH_MAX ends for string functions
        buf[locationSize] = '\0';

        // Check for newline after filename
        int i;
        for (i = 0; buf[i] != '\n'; i++) {
            // Return error if space/null appears before newline
            if (buf[i] == ' ' || buf[i] == '\0') {
                fprintf(stderr, "Invalid Command\n");
                return (EXIT_FAILURE);
            }
        }

        // Place null char where newline is for filename
        buf[i] = '\0';

        // Open file for writing and check for validity
        int fd;
        if ((fd = open(buf, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
            fprintf(stderr, "Invalid Command\n");
            return (EXIT_FAILURE);
        }

        // Write file readBytes amount of contents from initial read() call for first line.
        int writtenBytes = 0;
        int totalWritten = 0;
        totalRead = totalRead - i
                    - 1; // Only read contents of buffer (subtract from filename and newline)
        char *contents = buf + i + 1; // start pointer after newline of filenmae
        while (totalWritten < totalRead) {
            writtenBytes = write(fd, contents + totalWritten, totalRead - totalWritten);
            //Check for invalid write
            if (writtenBytes == -1) {
                fprintf(stderr, "Operation Failed\n");
                close(fd);
                return (EXIT_FAILURE);
            }
            totalWritten += writtenBytes;
        }

        // Continue to read all other unread input
        while ((readBytes = read(STDIN_FILENO, buf, 8192)) > 0) {
            // Write file readBytes amount of contents to file
            int writtenBytes, totalWritten = 0;
            while (totalWritten < readBytes) {
                writtenBytes = write(fd, buf + totalWritten, readBytes - totalWritten);
                //Check for invalid write
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
