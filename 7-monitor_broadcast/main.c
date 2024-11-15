/*
    There are situations in which the unlocking a single thread is not enough.

    Let's suppose that we want to unlock *all* threads that are waiting on a condition variable.

    Pthread (and other libraries) usually provide a way to unlock all waiting threads.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define N_RUNNERS 10

bool started = false;
pthread_mutex_t mutex;
pthread_cond_t condition;

void* sprint_starter(void* param)
{
    sleep(rand() % 5);  // Random wait time
    pthread_mutex_lock(&mutex);
    started = true;
    pthread_cond_broadcast(&condition); // Signal that all threads can start
    pthread_mutex_unlock(&mutex);
}

void* sprinter(void* param) {
    pthread_mutex_lock(&mutex);
    while (!started) {
        pthread_cond_wait(&condition, &mutex); // Wait for the signal to start
    }
    printf("Sprinter %ld started running at time %ld\n", pthread_self(), time(NULL));
    pthread_mutex_unlock(&mutex);
}

int main()
{
    pthread_t starter_thread;
    pthread_t sprinter_threads[N_RUNNERS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);

    pthread_create(&starter_thread, NULL, sprint_starter, NULL);

    for (int i = 0; i < N_RUNNERS; i++) {
        pthread_create(&sprinter_threads[i], NULL, sprinter, NULL);
    }

    pthread_join(starter_thread, NULL);
    for (int i = 0; i < N_RUNNERS; i++) {
        pthread_join(sprinter_threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);

    return 0;
}
