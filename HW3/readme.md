Author: Lucas Ho
Date: [03/26/2024]

## Introduction

This C program demonstrates the use of Pthreads (POSIX Threads) to join ten threads for calculating 0-9999. Each thread calculates the sum of 1000 numbers.

## Prerequisites

Install the following prerequisite:

- **GCC Compiler**: C compiler to build and run the program.
- **Linux or Unix-like System**: Use of Unix-like systems to use the fork() and shared memory.

## Compilation

Follow these steps to compile the code:

1. Open terminal.

2. Navigate to the directory for the C source code (`main.c`).

3. Compile the code with `gcc`:

   gcc -o pthread_sum main.c -lpthread
   ./pthread_sum 