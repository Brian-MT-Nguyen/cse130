# Assignment 1 directory

This directory contains source code and other files for Assignment 1.

## Functionality
Takes in stdin user input and then:
- Prints out file contents to stdout if used get
- Writes stdin input to file if used set

## Developer Notes:
- Initially had some troubles finding exact formmatting for string inputs
- When testing, some problems came up about reading in contents for set command, but were fixed
- New more exhaustive tests caused a revamp of receiving buffer input and parsing through commands + location correctly (in cases for partial rw as well)
- Ditched strtok (even though it is valid) and worked with buffer directly instead via indexing and pointer arithmetic
- Did not modularize yet but can grab io functions from these for later use if needed
