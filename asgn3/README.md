# Assignment 3 directory

This directory contains source code and other files for Assignment 3.

## Functionality
A thread safe Circular Queue ADT that supports up to multiple threads (thread safe) pushing and popping of (void *) elements
- Used semaphors (3 total) for mutual exclusion and thread safety as well as checking for if the queue is full or empty without busy waiting, but blocking instead
- Dynamically allocates the queue and the shared bounded buffer of items using malloc and frees using queue_new and queue_destroy
- Pushing (queue_push) and Popping (queue_pop) follow circiular queue logic by tracking the front and rear of queue and using the modulo of its size:
    - Push sets the buffer at index i to the element and increments in with modulo size
    - Pop sets the (void **) elem to the desired item and increments out with modulo size

## Developer Notes:
- Followed the psuedo-code provided by Quinn in lecture
- Still unsure if there is a more efficient/ better performing method
