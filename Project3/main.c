// Lucas Ho
// CS 3113 Intro to Operating System
// Project 3
// 29 April 2024

// Define constants for thread safety and buffer management
#define _REENTRANT
#define BUFFER_SIZE 15
#define END_OF_FILE_MARKER '*'

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>

// Define Semaphores
sem_t empty;
sem_t full;
sem_t mutex;

int count = 0; // number of characters in circular bufffer
int start = 0; // reading index
int end = 0; // writing index

// circular buffer declaration
char c_buffer[BUFFER_SIZE];

// Producer thread function
void* thread_1(void *arg) 
{
    // Variables for file handling
    char Char;
    FILE* fp;

    fp = fopen("mytest.dat", "r"); // Open the file for reading
    
    // Read characters from the file until EOF and store them in the buffer
    while (fscanf(fp, "%c", &Char) != EOF) 
    {
        sem_wait(&empty); // Wait until there is space
        sem_wait(&mutex); // Lock the buffer

        c_buffer[end] = Char; // Write character to the buffer
        end = (end + 1) % BUFFER_SIZE; // Move the write index circularly
        count++; // Increment count of charactes in the buffer

        sem_post(&mutex); // Release the buffer
        sem_post(&full); // Signal that the buffer has new data
    }

    // Handle the end of file by marking the buffer appropriately
    sem_wait(&empty);
    sem_wait(&mutex);

    c_buffer[end] = END_OF_FILE_MARKER; // Mark the end of file in the buffer
    end = (end + 1) % BUFFER_SIZE;
    count++;

    sem_post(&mutex);
    sem_post(&full);

    fclose(fp); // Close the file
    return NULL; // End of thread function
}

// Consumer thread function
void* thread_2(void *arg) 
{
    printf("Characters read by consumer: \n");

    // Continuously read characters from the buffer
    while(1) 
    {
        sem_wait(&full); // Wait for data to be available
        sem_wait(&mutex); // Lock the buffer

        // Check for end of file marker and exit if found
        if (c_buffer[start] == END_OF_FILE_MARKER) 
        {
            sem_post(&mutex);// Release the lock
            sem_post(&empty); // Signal empty space
            sem_post(&full);  // Signal to handle any remaining consumers
            pthread_exit(NULL); // Exit thread
        }
        
        printf("%c", c_buffer[start]); //Print character
        fflush(stdout); // Ensure it's displayed immediately 

        start = (start + 1) % BUFFER_SIZE; // Circularly increment the read index
        count--; // Decrement count of character in the buffer

        sem_post(&mutex); // Release the buffer
        sem_post(&empty); // Signal space is available
        sleep(1); // Simulate processing time
    }
    return NULL;
}

int main()
{
    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_t tid_1[1]; // Thread ID for producer
    pthread_t tid_2[1]; // Thread ID for consumer
    pthread_attr_t attr[1]; // Thread attribute

    fflush(stdout); // Clear any buffered data

    // Set thread scheduling to be managed by the system
    pthread_attr_init(&attr[0]);
    pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM);

    // Create producer and consumer threads
    pthread_create(&tid_1[0], &attr[0], thread_1, NULL);
    pthread_create(&tid_2[0], &attr[0], thread_2, NULL);

    // Wait for both threads to complete
    pthread_join(tid_1[0], NULL);
    pthread_join(tid_2[0], NULL);

    // Destroy semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    // Exit program
    printf("End of Program. \n");
    pthread_exit(NULL);
    
    return 0;

}