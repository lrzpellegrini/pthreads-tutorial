/*
    Let's consider the last high-level synchronizazion primitive: barriers.

    A barrier is a synchronization mechanism that allows a set of threads to wait 
    for each other to reach a common point. When all threads have reached the barrier,
    they are all released and can continue executing.

    They can be easily implemented using condition variables+mutexes, but pthreads
    provides a barrier implementation that is more efficient and easier to use.
*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS 4

pthread_barrier_t barrier;

void *thread_function(void *arg)
{
    int thread_id = *((int *)arg);
    printf("Thread %d is initializing its variable.\n", thread_id);
    
    sleep(1); // Simulate work

    printf("Thread %d has reached the barrier.\n", thread_id);

    // Wait at the barrier
    pthread_barrier_wait(&barrier);

    printf("Thread %d is proceeding after the barrier.\n", thread_id);
    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    /*
        Initialize the barrier for NUM_THREADS

        It is very important to initialize the barrier with the correct number of threads
        that will wait on it. If the number of threads is not correct, the program will
        hang indefinitely (or the barrier will be released too early).
    */
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy the barrier
    pthread_barrier_destroy(&barrier);

    return 0;
}
