/*
    This program creates 5 threads. 
    Each thread prints a message to the console.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

/*
    This time we'll use the argument of PrintHello to get an input value.
*/
void *PrintHello(void *threadid);

int main()
{
    /*
        This time we need to keep track of multiple threads (using an array of pthread_t).
    */
    pthread_t threads[NUM_THREADS];
    int rc, t;

    /*
        Let's create the threads.
    */
    for (t = 0; t < NUM_THREADS; t++)
    {
        printf("Creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // Wait for all threads to complete
    for (t = 0; t < NUM_THREADS; t++)
    {
        pthread_join(threads[t], NULL);
    }

    printf("Main(): All threads completed.\n");

    /*
        - Do all threads print in the same order?
        - Do all threads print after the main finished creating all of them?

        Try running the program multiple times and see if the output changes.
    */
    return 0;
}

void *PrintHello(void *threadid)
{
    /*
        Here we cast the input argument to a long and print it.

        This way we can see which thread is printing the message.

        We could also use the input argument to pass a custom message to the thread.
        In fact, the input is a void pointer, so we can pass any kind of data.
    */

    long tid;
    tid = (long)threadid;
    printf("Hello from thread %ld\n", tid);
}
