/*
    A producer-consumer example using semaphores and mutex.

    The producer threads produce items and adds them to a shared buffer.
    The consumer threads consume items from the shared buffer.

    Exercises:
    - Try changing the sleep times in the producer and consumer functions to see how it affects the output.
    - Try adding more producer and consumer threads to see how it affects the output.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5 // Size of the buffer
int buffer[BUFFER_SIZE]; // Shared buffer
int count = 0; // Count of items in the buffer

sem_t empty; // Semaphore to count empty slots
sem_t full;  // Semaphore to count full slots
pthread_mutex_t mutex; // Mutex for critical section

// Producer function
void* producer(void* arg)
{
    for (int i = 0; i < 10; i++)
    {
        sem_wait(&empty); // Decrement empty count
        pthread_mutex_lock(&mutex); // Enter critical section

        // Produce an item
        buffer[count] = i;
        printf("Produced: %d\n", buffer[count]);
        count++;

        pthread_mutex_unlock(&mutex); // Exit critical section
        sem_post(&full); // Increment full count
        sleep(1); // Simulate time taken to produce an item
    }
}

// Consumer function
void* consumer(void* arg)
{
    for (int i = 0; i < 10; i++)
    {
        sem_wait(&full); // Decrement full count
        pthread_mutex_lock(&mutex); // Enter critical section

        // Consume an item
        count--;
        printf("Consumed: %d\n", buffer[count]);

        pthread_mutex_unlock(&mutex); // Exit critical section
        sem_post(&empty); // Increment empty count
        sleep(2); // Simulate time taken to consume an item
    }
}

int main()
{
    pthread_t prod_thread, cons_thread;

    sem_init(&empty, 0, BUFFER_SIZE); // Initialize empty semaphore to BUFFER_SIZE
    sem_init(&full, 0, 0); // Initialize full semaphore to 0
    pthread_mutex_init(&mutex, NULL); // Initialize mutex

    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    sem_destroy(&empty); // Destroy empty semaphore
    sem_destroy(&full);  // Destroy full semaphore
    pthread_mutex_destroy(&mutex); // Destroy mutex

    return 0;
}
