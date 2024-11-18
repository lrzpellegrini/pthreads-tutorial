/*
    Exercise 2: parallel sum (solution)

    A group of friends wants to verify if, by throwing a coins 100000 times, the probability of getting heads is actually 0.5.

    To do this they will simulate the experiment by generating a million random numbers between 0 and 1. If the number is less than 0.5, they will consider it as a head, otherwise it will be a tail.
    However, throwing a million coins is a lot of work, so they decide to parallelize the task. However, each person (thread) after 1000 throws 
    will go resting for a while. In addition, only a limited number of coins is available (< number of persons), so the threads will have to share the coins.

    Your task is to implement a program that throws a million coins and calculates the number of heads obtained.
    Also keep track of the number of times each person throws a coin.

    Note: do not change the coin flip function and the whole management of the random number generator.
*/

#define _CRT_RAND_S
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#ifdef _WIN32

#elif defined(__linux__)
static __thread struct drand48_data buffer;
static __thread int initialized = 0;
#else
#error "OS not supported"
#endif

#define N_PERSONS 10
#define N_COINS 7
#define N_THROWS 1000000
#define REST_INTERVAL 80  // In nanoseconds

bool coin_flip();
void *person(void *);

int total_heads = 0;
int remaining_throws = N_THROWS;
int throws_per_person[N_PERSONS] = {0};

pthread_mutex_t count_mutex;
pthread_mutex_t head_mutex;
sem_t coin_semaphore;

void *person(void *thread_id)
{
    long person_id = (long) thread_id;
    bool i_have_the_coin = false;
    bool should_continue = true;
    int thrown_this_session = 0;
    bool head;

    while (should_continue) {
        // 1. Obtain the coin
        if (!i_have_the_coin) {
            sem_wait(&coin_semaphore);
            i_have_the_coin = true;
        }

        // Check if we should stop
        // (Note: if not, decrement the remaining throws)
        pthread_mutex_lock(&count_mutex);
        should_continue = remaining_throws > 0;
        if(should_continue) {
            remaining_throws--;
        }
        pthread_mutex_unlock(&count_mutex);

        if (should_continue) {
            // Throw the coin
            // also, keep track of the number of throws so far
            throws_per_person[person_id]++;
            thrown_this_session++;

            head = coin_flip();

            if (head) {
                pthread_mutex_lock(&head_mutex);
                total_heads++;
                pthread_mutex_unlock(&head_mutex);
            }
        }

        // Go to sleep if we have thrown 1000 coins
        // (remember to release the coin so that someone else can use it)
        if (thrown_this_session == 1000) {
            sem_post(&coin_semaphore);
            i_have_the_coin = false;
            thrown_this_session = 0;
            struct timespec req = {0, REST_INTERVAL};
            nanosleep(&req, NULL);
        }
    }

    if (i_have_the_coin) {
        sem_post(&coin_semaphore);
    }

}

int main()
{
    pthread_t threads[N_PERSONS];
    sem_init(&coin_semaphore, 0, N_COINS);
    pthread_mutex_init(&count_mutex, NULL);
    pthread_mutex_init(&head_mutex, NULL);

    for (long i = 0; i < N_PERSONS; i++) {
        pthread_create(&threads[i], NULL, person, (void *)i);
    }

    for (int i = 0; i < N_PERSONS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Total heads: %d\n", total_heads);
    for (int i = 0; i < N_PERSONS; i++) {
        printf("Person %d throws: %d\n", i, throws_per_person[i]);
    }

    return 0;
}

bool coin_flip()
{
    bool result;
    #ifdef _WIN32
        unsigned int random_value;
        // Generate a random number
        rand_s(&random_value);
        return (random_value % 2) == 0;
    #elif defined(__linux__)
        double rng_result;
        if (!initialized)
        {
            srand48_r(time(NULL), &buffer);
            initialized = 1;
        }

        drand48_r(&buffer, &rng_result);
        result = rng_result < 0.5;
    #endif
    return result;
}