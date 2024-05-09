#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 10

// Global variables for all threads to access
int array[ARRAY_SIZE];
int minimum, average;

// Thread helper function (minimum) 
void *findMinimum(void *arg) {
    minimum = array[0];
    for (int i = 1; i < ARRAY_SIZE; i++) {
        if (array[i] < minimum) {
            minimum = array[i];
        }
    }
    pthread_exit(NULL);
}

// Thread helper function (average) 
void *findAverage(void *arg) {
    int sum = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += array[i];
    }
    average = sum / ARRAY_SIZE;
    pthread_exit(NULL);
}

// Thread helper function (bubble sorting) 
void *sortArray(void *arg) {
    for (int i = 0; i < ARRAY_SIZE - 1; i++) {
        for (int j = 0; j < ARRAY_SIZE - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    pthread_exit(NULL);
}

int main() {
    printf("Enter the 10 elements of the array:\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        scanf("%d", &array[i]);
    }

    //define threads
    pthread_t minThread, avgThread, sortThread;

    //create threads
    pthread_create(&minThread, NULL, findMinimum, NULL);
    pthread_create(&avgThread, NULL, findAverage, NULL);
    pthread_create(&sortThread, NULL, sortArray, NULL);

    //wait for threads to finish so they are displayed sequentially 
    pthread_join(minThread, NULL);
    pthread_join(avgThread, NULL);
    pthread_join(sortThread, NULL);

    //print results
    printf("Minimum value: %d\n", minimum);
    printf("Average value: %d\n", average);
    printf("Sorted array: ");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}
