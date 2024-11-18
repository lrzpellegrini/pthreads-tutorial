/*
    Exercise 2: parallel sum

    A group of friends wants to verify if, by throwing a coins a million times, the probability of getting heads is actually 0.5.

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

#define N_PERSONS 5
#define N_COINS 4
#define N_THROWS 1000000
#define REST_INTERVAL 80  // In nanoseconds

bool coin_flip();
void *person(void *);

void *person(void *thread_id)
{
    long person_id = (long) thread_id;

    bool head = coin_flip();

    // Code needed to rest
    struct timespec req = {0, REST_INTERVAL};
    nanosleep(&req, NULL);
}

int main()
{
    return 0;
}

bool coin_flip()
{
    #ifdef _WIN32
        unsigned int random_value;
        // Generate a random number
        return (rand_s(&random_value) % 2) == 0;
    #elif defined(__linux__)
        double result;
        if (!initialized)
        {
            srand48_r(time(NULL), &buffer);
            initialized = 1;
        }

        drand48_r(&buffer, &result);
        return result;
    #endif
}