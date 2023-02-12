#Assignment 2 directory

This directory contains source code and other files for Assignment 2.

## Functionality
Creates an http server to listen/accept connections from clients for communication
- Prints to client via socket the contents of specified file if client requests GET
- Sets contents of a specified file to another specified file and prints OK/Created if client requests PUT

## Developer Notes:
- Initially had some troubles figuring out logic for parsing and REGEX
- Went through a couple of iterations on top down design and ended with parsing and handling module on top of main
- Had some silly errors like checking for wrong conditionals and wrong variables that added unecessary debug time
- Ended up not using response for now, but saving module in case future use is warranted
