/*
    A producer-consumer example using monitors.

    Just like the previous example:
    - The producer threads produce items and adds them to a shared buffer.
    - The consumer threads consume items from the shared buffer.

    However, this time we use monitors to synchronize the producer and consumer threads.

    Another difference with the previous example: here buffer items are random numbers between 0 and 99.

    Exercise: 
    Add a way for the producer to say "I'm done producing" to the consumer. That is, the consumer should stop 
    consuming when the producer is done (do not use MAX_ITEMS in the consumer).
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define MAX_ITEMS 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0;
pthread_mutex_t mutex;
pthread_cond_t not_empty, not_full;

void* producer(void* param)
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        int item = rand() % 100; // Produce a random item
        
        pthread_mutex_lock(&mutex);

        /*
            When a thread is woken up, it must re-check the condition that caused it to wait.
            This is because the condition may have changed while the thread was waiting.

            This is known as "spurious wakeups".

            Solution: use a while loop to re-check the condition!

            DO NOT FORGET THE WHILE LOOP WHEN USING CONDITION VARIABLES!
        */
        while ((in + 1) % BUFFER_SIZE == out)
        {
            pthread_cond_wait(&not_full, &mutex);
        }
        
        buffer[in] = item; // Add item to buffer
        printf("Produced: %d\n", item);
        in = (in + 1) % BUFFER_SIZE;
        
        pthread_cond_signal(&not_empty); // Signal that buffer is not empty
        pthread_mutex_unlock(&mutex);
        sleep(1); // Simulate time taken to produce an item
    }
}

void* consumer(void* param) {
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        pthread_mutex_lock(&mutex);
        while (in == out)
        {
            pthread_cond_wait(&not_empty, &mutex);
        }
        
        int item = buffer[out]; // Remove item from buffer
        printf("Consumed: %d\n", item);
        out = (out + 1) % BUFFER_SIZE;
        
        pthread_cond_signal(&not_full); // Signal that buffer is not full
        pthread_mutex_unlock(&mutex);
        sleep(2); // Simulate time taken to consume an item
    }
}

int main()
{
    pthread_t prod, cons;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);

    return 0;
}
