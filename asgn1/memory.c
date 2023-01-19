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
    int readBytes;
    if ((readBytes = read(STDIN_FILENO, buf, sizeof(buf) - 1)) == -1) {
        fprintf(stderr, "Invalid Statement\n");
        return (EXIT_FAILURE);
    }

    // Adds \0 to end of string for saveptr calls later
    buf[readBytes > 0 ? readBytes : 0] = '\0';

    // Variables to parse first line
    char *saveptr;

    // Get Command
    char *clStatement = strtok_r(buf, " ", &saveptr);
    char *command = clStatement;

    // Make strings to check validity of commands
    char *get = "get";
    char *set = "set";

    // If command is get
    if (strcmp(command, get) == 0) {
        // Check for newline to proceed
        for(int i = 0; saveptr[i] != '\n'; i++) {
            if(saveptr[i] == ' ' || saveptr[i] == '\0') {
                fprintf(stderr, "Invalid Command\n");
                return(EXIT_FAILURE);
            }
        }
        // Get Location
        clStatement = strtok_r(NULL, "\n", &saveptr);
        char *location = clStatement;

        // Check for invalid extra junk input
        char *clStatement = strtok_r(NULL, "", &saveptr);
        char *junk = clStatement;
        if(junk != NULL) {
            fprintf(stderr, "Invalid Command\n");
            return(EXIT_FAILURE);
        }

        // Open file for reading and check for validity
        int fd;
        if ((fd = open(location, O_RDONLY, 0444)) == -1) {
            fprintf(stderr, "Invalid Command\n");
            return (EXIT_FAILURE);
        }

        // Write file contents to stdout
        int readBytes;
        while ((readBytes = read(fd, buf, sizeof(buf))) > 0) {
            int writtenBytes, totalWritten = 0;
            while (totalWritten < readBytes) {
                writtenBytes = write(STDOUT_FILENO, buf + totalWritten, readBytes);
                totalWritten += writtenBytes;
            }
        }
        if(readBytes  == -1) {
            fprintf(stderr, "Invalid Command\n");
            return(EXIT_FAILURE);
        }

        // Close file and exitg
        close(fd);
        return (EXIT_SUCCESS);
    }

    // If command is set
    else if (strcmp(command, set) == 0) {
        for(int i = 0; clStatement[i] != '\n'; i++) {
            if(clStatement[i] == ' ') {
                fprintf(stderr, "Invalid Command\n");
                return(EXIT_FAILURE);
            }
        }

        // Get Location
        clStatement = strtok_r(NULL, "\n", &saveptr);
        char *location = clStatement;
        
        // Open file for writing and check for validity
        int fd;
        if ((fd = open(location, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
            fprintf(stderr, "Invalid Command\n");
            return (EXIT_FAILURE);
        }

        // Write contents to buffer from initial read() call for first line.
        int writtenBytes = 0;
        unsigned long totalWritten = 0;
            while (totalWritten < strlen(saveptr)) {
                writtenBytes = write(fd, saveptr + totalWritten, strlen(saveptr));
                totalWritten += writtenBytes;
            }

        // Continue to read if user would like to input more than initial input
        int readBytes;
        while ((readBytes = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
            int writtenBytes, totalWritten = 0;
            while (totalWritten < readBytes) {
                writtenBytes = write(fd, buf + totalWritten, readBytes);
                totalWritten += writtenBytes;
            }
        }
        if(readBytes  == -1) {
            fprintf(stderr, "Invalid Command\n");
            return(EXIT_FAILURE);
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
