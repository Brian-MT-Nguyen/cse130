#Assignment 2 directory

This directory contains source code and other files for Assignment 2.

## Functionality
Creates an http server to listen/accept connections from clients for communication via 2 request commands: GET and PUT
- Prints to client via socket the contents of specified file if client requests GET
- Sets contents of specified file to given msgbody and prints to client "OK" or "Created" via socket if client requests PUT

## Developer Notes:
- Initially had some troubles figuring out logic for parsing and REGEX
- Went through a couple of iterations on top down design and ended with parsing and handling module on top of main
- Had some silly errors like checking for/using wrong conditionals and wrong variables that added unecessary debug time
- Added lots of error handling and edge case detection using errno to hopefully ensure correct use of server status code
- Ended up not using response for now, but saving module in case future use is warranted
