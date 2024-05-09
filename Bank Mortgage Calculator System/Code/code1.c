#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int array[10];

// Find minimum
int findMinimum() {
    int minimum = array[0];
    for (int i = 1; i < 10; i++) {
        if (array[i] < minimum) {
            minimum = array[i];
        }
    }
    return minimum;
}

// Find average
float findAverage() {
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        sum += array[i];
    }
    return (float)sum / 10;
}

// Swap for bubble sort (using pointers for direct memory access)
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Bubble sort
void sortArray() {
    for (int i = 0; i < 10 - 1; i++) {
        for (int j = 0; j < 10 - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                swap(&array[j], &array[j + 1]);
            }
        }
    }
}

int main() {
    printf("Enter the 10 elements of the array:\n");
    for (int i = 0; i < 10; i++) {
        scanf("%d", &array[i]);
    }

    //child processes
    int pid_min, pid_avg, pid_sort;
    
    //variables to store answers
    int minimum;
    float average;
    int sorted_array[10]; //New array for sorted data

    // Pipes descriptors
    int min_pipe[2];
    int avg_pipe[2];
    int sort_pipe[2];

    //error
    if (pipe(min_pipe) == -1 || pipe(avg_pipe) == -1 || pipe(sort_pipe) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }

    // Minimum child process 
    if ((pid_min = fork()) == 0) {
        minimum = findMinimum();
        close(min_pipe[0]);//close read
        write(min_pipe[1], &minimum, sizeof(minimum));//write on pipe the min
        close(min_pipe[1]);//close write
        exit(0);
    }

    // Average child process 
    if ((pid_avg = fork()) == 0) {
        average = findAverage();
        close(avg_pipe[0]);//close read
        write(avg_pipe[1], &average, sizeof(average));//write on pipe the avg
        close(avg_pipe[1]);//close write
        exit(0);
    }

    // Sorting child process 
    if ((pid_sort = fork()) == 0) {
        sortArray();
        close(sort_pipe[0]);//close read
        write(sort_pipe[1], array, sizeof(array));//send sorted array
        close(sort_pipe[1]);//close write
        exit(0);
    }

    // wait for child processes to finish
    waitpid(pid_min, NULL, 0);
    waitpid(pid_avg, NULL, 0);
    waitpid(pid_sort, NULL, 0);

    // main reads values from pipes
    close(min_pipe[1]);//close write
    read(min_pipe[0], &minimum, sizeof(minimum));//read min
    close(min_pipe[0]);//close read

    close(avg_pipe[1]);//close write
    read(avg_pipe[0], &average, sizeof(average));//read avg
    close(avg_pipe[0]);//close read

    close(sort_pipe[1]);//close write
    read(sort_pipe[0], sorted_array, sizeof(sorted_array));//read sorted
    close(sort_pipe[0]);//close read

    // Print 
    printf("Minimum value: %d\n", minimum);
    printf("Average value: %.2f\n", average);
    printf("Sorted array: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", sorted_array[i]);
    }
    printf("\n");

    return 0;
}

