#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>

int main(void) {
    // Universal buffer for all
    char buf[4096];
    if (read(STDIN_FILENO, buf, sizeof(buf)) == -1) {
        fprintf(stderr, "Invalid Statement\n");
        return (EXIT_FAILURE);
    }

    // Get/Set and Location (First line)
    char delimit[] = " \n";
    char *clStatement = strtok(buf, delimit);

    char *command = clStatement;

    clStatement = strtok(NULL, delimit);
    char *location = clStatement;

    // Check if anything is left then returns invalid (incorrect formatting)
    if ((clStatement = strtok(NULL, delimit)) != NULL) {
        fprintf(stderr, "Invalid Statement\n");
        return (EXIT_FAILURE);
    }

    // Make strings to check validity of commands
    char *get = "get";
    char *set = "set";

    // If command is get
    if (strcmp(command, get) == 0) {
        // Open file for reading and check for validity
        int fd;
        if ((fd = open(location, O_RDONLY)) == -1) {
            fprintf(stderr, "Invalid Filepath\n");
            return (EXIT_FAILURE);
        }

        // Clear buffer for writing from file
        memset(buf, '\0', sizeof(buf));

        // Write file contents to stdout
        int readBytes;
        while ((readBytes = read(fd, buf, sizeof(readBytes))) > 0) {
            write(STDOUT_FILENO, buf, sizeof(readBytes));

            // Clear buffer for writing from file
            memset(buf, '\0', sizeof(buf));
        }

        // Write newline to finish
        write(STDOUT_FILENO, "\n", sizeof("\n"));

        // Close file and exit
        close(fd);
        return (EXIT_SUCCESS);
    }

    // If command is set
    else if (strcmp(command, set) == 0) {
        // Open file for writing and check for validity
        int fd;
        if ((fd = open(location, O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1) {
            fprintf(stderr, "Invalid Filepath\n");
            return (EXIT_FAILURE);
        }

        // Clear buffer for writing to file
        memset(buf, 0, sizeof(buf));
        char masterString[4096] = "";
        // Write file contents to stdout
        int readBytes;
        while ((readBytes = read(STDOUT_FILENO, buf, sizeof(buf))) > 0) {
            char line[sizeof(readBytes) + 1];
            strncpy(line, buf, sizeof(readBytes));
            line[sizeof(readBytes)] = '\0';
            fprintf(stdout, "%s\n", line);

            // Re-clear buffer for writing next line to file
            memset(buf, '\0', sizeof(buf));
        }
        fprintf(stdout, "Final string: %s", masterString);

        // Write OK to finish
        write(STDOUT_FILENO, "OK\n", sizeof("OK\n"));

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
