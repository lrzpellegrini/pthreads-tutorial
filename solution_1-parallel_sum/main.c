/*
    Exercise 1: parallel sum (solution)

    An array of integers is given. The goal is to obtain the sum of the elements of the array.
    
    The sum of N integers can be calculated by dividing the array into T parts.
    The sum of the parts can be calculated in parallel and the final sum can be obtained by adding the sums of the parts.

    Write a program that calculates the sum of the elements of the array in parallel using threads.

    Rules:
    - The array must be divided into T parts (T = number of threads). Use a number of threads that is a dividend of the size of the array.
    - Each thread must calculate the sum of the elements of the part assigned to it.
    - You can only use pthread_create and pthread_join functions from the pthread library. No semaphores or other synchronization mechanisms are allowed.
    - You can create global variables, functions, define macros, etc. as needed.
    
    The array will be created randomly and the excpected output will be printed when starting. Do not modify the array creation part.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N_VALUES 1000000 // Size of the array of ints
#define N_THREADS 4
int buffer[N_VALUES]; // Shared buffer

// Define the array of sums of the parts
long sums[N_THREADS];

// Define the sum function
void *sum(void *arg)
{
    // Get the part number
    long part = (long) arg;

    // Calculate the start and end indexes of the part
    long start = part * (N_VALUES / 4);
    long end = (part + 1) * (N_VALUES / 4);

    // Calculate the sum of the part
    long result = 0;
    for (int i = start; i < end; i++)
    {
        result += buffer[i];
    }

    // Store the sum of the part in the global variable (an array of size T)
    sums[part] = result;
}

int main()
{
    long i, expected_result;
    // Create the array of integers
    srand(time(NULL));
    for (i = 0; i < N_VALUES; i++)
    {
        buffer[i] = rand() % 10;
    }

    // Print the expected output
    expected_result = 0;
    for (i = 0; i < N_VALUES; i++)
    {
        expected_result += buffer[i];
    }

    printf("Expected output: %ld\n", expected_result);

    // One of the possible solutions is to
    // 1. create a thread for each part of the array by defining a sum function to calculate the sum of the part
    // 2. make each thread store the sum of the part in a global variable (an array of size T)
    // 3. join the threads and calculate the final sum
    // Here is a possible implementation of the solution:

    // Define the size of the part
    int part_size = N_VALUES / N_THREADS;

    // Define the thread ids
    pthread_t threads[N_THREADS];

    // Create the threads
    for (i = 0; i < N_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, sum, (void *) i);
    }

    // Join the threads
    for (i = 0; i < N_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Calculate the final sum
    long final_sum = 0;
    for (i = 0; i < N_THREADS; i++)
    {
        final_sum += sums[i];
    }

    // Print the final sum
    printf("Final sum: %ld\n", final_sum);

    return 0;
}
