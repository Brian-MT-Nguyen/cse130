# Assignment 3 directory

This directory contains source code and other files for Assignment 3.

## Functionality
A thread safe Circular Queue ADT that supports up to multiple threads (thread safe) pushing and popping of (void *) elements
- Used semaphors (3 total) for mutual exclusion and thread safety as well as checking for if the queue is full or empty without busy waiting, but blocking instead
- Dynamically allocates/creates the queue and the shared bounded buffer of items using malloc and frees, using queue_new and queue_delete respectively
- Pushing (queue_push) and Popping (queue_pop) follow circiular queue logic by tracking the front and rear of queue and using the modulo of its size:
    - queue_push sets the buffer at index i to the desired element and increments in around modulo size
    - queue_pop sets the (void **) elem to the desired element and increments out around modulo size

## Developer Notes:
- Based implementation of required queue.h file provided
- Followed the psuedo-code provided by Quinn in lecture
- Still unsure if there is a more efficient/ better performing method
- Also unsure whether to account for more errors and what messages to print out (I used assert() where applicable)
