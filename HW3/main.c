#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 10
#define NUM_NUMBERS 1000
#define TOTAL_NUMBERS (NUM_THREADS * NUM_NUMBERS)

// Function that each thread will execute
void *sum(void *arg) {
    int thread_id = *((int *)arg);
    int start = thread_id * NUM_NUMBERS;
    int end = start + NUM_NUMBERS;
    long long sum = 0;

    for (int i = start; i < end; i++) {
        sum += i;
    }

    printf("Thread %d: Sum = %lld\n", thread_id, sum);

    // Return the sum as a void pointer
    return (void *)(sum);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int targs[NUM_THREADS];
    long long total = 0;

    // Create and start the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        targs[i] = i;
        if (pthread_create(&threads[i], NULL, sum, &targs[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    // Wait for the threads to finish and collect their results
    for (int i = 0; i < NUM_THREADS; i++) {
        void *tresult;
        if (pthread_join(threads[i], &tresult) != 0) {
            perror("pthread_join");
            return 1;
        }
        total += (long long)tresult;
    }

    printf("Total Sum = %lld\n", total);

    return 0;
}