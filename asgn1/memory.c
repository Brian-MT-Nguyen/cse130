#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>

int main(void) {
    // Universal buffer for all
    char buf[4096];

    // Read in first line for command + location
    if ((read(STDIN_FILENO, buf, sizeof(buf))) == -1) {
        fprintf(stderr, "Invalid Statement\n");
        return (EXIT_FAILURE);
    }

    // Get/Set and Location (First line)
    char *delimit = " \n";
    char *saveptr;
    char *clStatement = strtok_r(buf, delimit, &saveptr);

    char *command = clStatement;

    clStatement = strtok_r(NULL, delimit, &saveptr);
    char *location = clStatement;

    // // Check if anything is left then returns invalid (incorrect formatting)
    // if ((clStatement = strtok(NULL, delimit)) != NULL) {
    //     fprintf(stderr, "Invalid Statement\n");
    //     return (EXIT_FAILURE);
    // }

    // Make strings to check validity of commands
    char *get = "get";
    char *set = "set";

    // If command is get
    if (strcmp(command, get) == 0) {
        // Open file for reading and check for validity
        int fd;
        if ((fd = open(location, O_RDONLY, 0444)) == -1) {
            fprintf(stderr, "Invalid Filepath\n");
            return (EXIT_FAILURE);
        }

        // Write file contents to stdout
        int readBytes;
        while ((readBytes = read(fd, buf, sizeof(buf))) > 0) {
            write(STDOUT_FILENO, buf, readBytes);
        }

        // Write newline to finish

        // Close file and exit
        close(fd);
        return (EXIT_SUCCESS);
    }

    // If command is set
    else if (strcmp(command, set) == 0) {
        // Open file for writing and check for validity
        int fd;
        if ((fd = open(location, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
            fprintf(stderr, "Invalid Filepath\n");
            return (EXIT_FAILURE);
        }

        // Write file contents to stdout
        write(fd, saveptr, strlen(saveptr));
        int readBytes;
        while ((readBytes = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
            write(fd, buf, readBytes);
        }

        // Write OK to finish
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
