# Assignment 4 directory

This directory contains source code and other files for Assignment 4.

## Functionality
A multi-threaded version of the HTTP Server in Asgn 2
- Uses a dispatcher thread (the main thread) and a thread pool of workers of size n specified by the user of the server (default: 4), giving a total of n+1 threads.
- Prints to client via socket the contents of specified file if client requests GET.
- Sets contents of specified file to given msgbody and prints to client "OK" or "Created" via socket if client requests PUT.
- Prints an atomic audit log of the requests the server proccesses in order of when they are proccessed by the worker threads to stderr.

## Developer Notes:
- Followed the psuedo-code of mutex file creation lock provided by Vince in section about preventing PUT race conditions
- Failure rate is about 1/100 for conflict_stress_mix test and 100% for all others (decided on the tradeoff for more throughput)
- May try to implement another variant of PUT race conditions prevention by creating another thread that is specifically made for creating files pushed/popped from a dedicated queue in the future.
