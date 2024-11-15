/*
    Exercise 1: parallel sum

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
int buffer[N_VALUES]; // Shared buffer

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

    // Your code here

    return 0;
}
