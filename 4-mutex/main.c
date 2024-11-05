/*
    This program counts to 500000 using 5 threads, each incrementing the counter by 100000.
    The threads share a global counter variable, and a mutex is used to synchronize access to the counter.

    Try it with and without mutex!
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5
#define INCREMENTS_PER_THREAD 100000

void* increment_counter(void* thread_id);

/*
    Note that we are using 2 global variables:
    - counter: the shared counter variable that all threads will increment
    - counter_mutex: a mutex that will be used to synchronize access to the counter

    pthread_mutex_t is a type that represents a mutex in the pthreads library.
*/
int counter = 0;
pthread_mutex_t counter_mutex;


int main()
{
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&counter_mutex, NULL);

    // Create threads
    for (long i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, increment_counter, (void*)i);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Print the final value of the counter
    printf("Final counter value: %d\n", counter);

    // Clean up and exit
    pthread_mutex_destroy(&counter_mutex);
    return 0;
}

void* increment_counter(void* thread_id)
{
    long tid = (long)thread_id;
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++)
    {
        // Lock the mutex before modifying the counter
        pthread_mutex_lock(&counter_mutex);

        // Increment the counter by 1 (the mutex ensures that only one thread can do this at a time)
        counter++;

        // Unlock the mutex after modification
        pthread_mutex_unlock(&counter_mutex);
    }
    printf("Thread %ld finished incrementing.\n", tid);
}
