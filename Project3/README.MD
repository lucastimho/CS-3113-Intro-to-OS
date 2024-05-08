## Project 3 README
CS 3113: Intro to Operating Systems
Lucas Ho
04/26/2024

## Introduction 
This C program features a producer-consumer system where the producer thread reads characters from a file and stores them in a circular buffer. The consumer thread then reads these characters from the buffer. Synchronization is managed using semaphores to ensure that the producer and consumer operate correctly without colliding or accessing empty or undefined sections of the buffer. The system handles synchronization of access to the buffer to maintain the integrity of the data.

## Compilation
Follow the included steps to compile the code:
1. Open your terminal.
2. Navigate to the src directory which contains the C code.
3. Compile the code by typing in the terminal:
    `gcc -o main main.c -pthread`
4. Once compiled, execute the program by typing:
    `./main`
