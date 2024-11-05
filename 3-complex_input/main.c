/*
    This program creates 5 threads and passes complex data to each thread. 
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 5

void *PrintHello(void *threadid);

typedef struct {
    int thread_id;
    int some_value;
} my_custom_data;

int main()
{
    pthread_t threads[NUM_THREADS];
    my_custom_data thread_data_array[NUM_THREADS];
    int rc, t;

    /*
        First thing, let's create the custom data for each thread.
    */
    srand(time(NULL));
    for (int t = 0; t < NUM_THREADS; t++)
    {
        thread_data_array[t].thread_id = t;
        thread_data_array[t].some_value = rand() % 100;
    }

    /*
        Notice that this time we pass a pointer to the custom data as the last argument 
        of pthread_create.

        Note: we cast the pointer to void* because pthread_create expects a void* as the last argument.
        Note 2: this data resides in the stack of the main thread, so we need to make sure that the main thread
        does not exit before the other threads finish using this data.
    */
    for (t = 0; t < NUM_THREADS; t++)
    {
        printf("Creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)&thread_data_array[t]);
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
    return 0;
}

void *PrintHello(void *thread_data)
{
    /*
        Here we cast the input argument to a long and print it.

        This way we can see which thread is printing the message.

        We could also use the input argument to pass a custom message to the thread.
        In fact, the input is a void pointer, so we can pass any kind of data.
    */

    my_custom_data *data;
    data = (my_custom_data *)thread_data;
    printf("Hello from thread %d. Received some_value=%d\n", data->thread_id, data->some_value);
}
