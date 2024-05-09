#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE_LENGTH 100
#define MAX_LOANS 100

// Structure to store loan information
struct Loan {
    char client[50];
    float monthlyPayment;
};

// Function to calculate and return the average monthly payment
float calculateAverage(struct Loan loans[], int numLoans) {
    float totalPayment = 0;
    for (int i = 0; i < numLoans; i++) {
        totalPayment += loans[i].monthlyPayment;
    }
    return totalPayment / numLoans;
}

// Function to calculate and return the maximum monthly payment
float calculateMax(struct Loan loans[], int numLoans) {
    float maxPayment = 0;
    for (int i = 0; i < numLoans; i++) {
        if (loans[i].monthlyPayment > maxPayment) {
            maxPayment = loans[i].monthlyPayment;
        }
    }
    return maxPayment;
}

// Function to calculate and return the minimum monthly payment
float calculateMin(struct Loan loans[], int numLoans) {
    float minPayment = loans[0].monthlyPayment;
    for (int i = 1; i < numLoans; i++) {
        if (loans[i].monthlyPayment < minPayment) {
            minPayment = loans[i].monthlyPayment;
        }
    }
    return minPayment;
}

int main() {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    struct Loan loans[MAX_LOANS]; // Assuming a maximum of 100 loans
    int numLoans = 0;

    // Open the file for reading
    file = fopen("loans.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read and process each line of the file
    while (fgets(line, sizeof(line), file)) {
        char client[50];
        float monthlyPayment;

        // Parse the line to extract loan information
        if (sscanf(line, "Client: %49[^,], Monthly Payment: %f", client, &monthlyPayment) == 2) {
            strcpy(loans[numLoans].client, client);
            loans[numLoans].monthlyPayment = monthlyPayment;
            numLoans++;
        }
    }

    // Close the file
    fclose(file);

    // Create child processes
    pid_t avg_pid, max_pid, min_pid;
    int avg_pipe[2], max_pipe[2], min_pipe[2];

    if (pipe(avg_pipe) == -1 || pipe(max_pipe) == -1 || pipe(min_pipe) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    avg_pid = fork();

    if (avg_pid == 0) {
        // Child process for average calculation
        close(avg_pipe[0]); // Close read end of the pipe
        float avgPayment = calculateAverage(loans, numLoans);
        write(avg_pipe[1], &avgPayment, sizeof(float)); // Write the average payment to the pipe
        close(avg_pipe[1]);
        exit(0);
    } else if (avg_pid < 0) {
        perror("Fork failed");
        return 1;
    }

    max_pid = fork();

    if (max_pid == 0) {
        // Child process for maximum calculation
        close(max_pipe[0]); // Close read end of the pipe
        float maxPayment = calculateMax(loans, numLoans);
        write(max_pipe[1], &maxPayment, sizeof(float)); // Write the max payment to the pipe
        close(max_pipe[1]);
        exit(0);
    } else if (max_pid < 0) {
        perror("Fork failed");
        return 1;
    }

    min_pid = fork();

    if (min_pid == 0) {
        // Child process for minimum calculation
        close(min_pipe[0]); // Close read end of the pipe
        float minPayment = calculateMin(loans, numLoans);
        write(min_pipe[1], &minPayment, sizeof(float)); // Write the min payment to the pipe
        close(min_pipe[1]);
        exit(0);
    } else if (min_pid < 0) {
        perror("Fork failed");
        return 1;
    }

    // Parent process

    // Close the write ends of the pipes
    close(avg_pipe[1]);
    close(max_pipe[1]);
    close(min_pipe[1]);

    float avgPayment, maxPayment, minPayment;

    // Read the values from the pipes
    read(avg_pipe[0], &avgPayment, sizeof(float));
    read(max_pipe[0], &maxPayment, sizeof(float));
    read(min_pipe[0], &minPayment, sizeof(float));

    // Close the read ends of the pipes
    close(avg_pipe[0]);
    close(max_pipe[0]);
    close(min_pipe[0]);

    // Print the results
    printf("\nHere are the statistical results:\n");
    printf("Average Monthly Payment: %.2f\n", avgPayment);
    printf("Max Monthly Payment: %.2f\n", maxPayment);
    printf("Min Monthly Payment: %.2f\n", minPayment);

    // Wait for all child processes to complete
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    return 0;
}

