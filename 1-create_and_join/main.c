/*
This program creates a new thread. 
That thread will print a message to the console and then exit.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5

/*
    First change: the function signature of PrintHello() 
    is changed to return void* and take a void* argument.
*/
void *PrintHello(void *);

int main()
{
    /*
        pthread_t is the data type used to uniquely identify a thread.
        We can use that identifer to manage the lifecycle of the thread.
    */
    pthread_t hello_thread;
    int rc;

    /*
        First thing, let's create the thread.

        To do this, we will use pthread_create, that needs 4 arguments:
        - a pointer to a pthread_t variable, that will be used to identify the thread
        - a pointer to a pthread_attr_t variable, that will be used to set the thread attributes (we don't use it now)
        - a pointer to the function that will be executed by the thread (its entry point)
        - a pointer to the argument that will be passed to the function (we don't use it now)
    */
    printf("Creating a thread\n");
    rc = pthread_create(&hello_thread, NULL, PrintHello, NULL);

    /*
        pthread_create returns an integer that is 0 if the thread was created successfully
        (or any other value if an error occurred).
    */
    if (rc)
    {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        return 1;
    }

    /*
        Wait for the thread to complete.

        To do this, we use pthread_join by passing the thread ID as the first argument.
        The second argument is a pointer to a variable that will receive the return value of the thread (if any).
        We can pass NULL if we don't care about the return value.
    */
    pthread_join(hello_thread, NULL);
    
    printf("[Main] Hello thread completed!\n");

    /*
        What happens if we don't call pthread_join?

        Comment it out and check for differences in the output.
    */
    return 0;
}

void *PrintHello(void *unused_parameter)
{
    printf("[PrintHello] Hello from thread. Going to sleep for a second.\n");
    sleep(1);
    printf("[PrintHello] Waking up after sleep.\n");
}
