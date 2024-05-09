#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int pipefd[2]; // file descriptor for the pipe

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }

    // Child creation using fork
    pid = fork();
	
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } 
    
    // Child process
    else if (pid == 0) {    
        close(pipefd[0]); //close read end of pipe 

        //open the input file using file descriptor
        int fd_input = open("input.txt", O_RDONLY);
        if (fd_input == -1) {
            perror("Error opening input file");
            exit(1);
        }

        // the replaced sections in the file
        char nameFile[] = "[name]";
        char dateFile[] = "[date]";

        // the user info
        char name[100];
        char date[20];

        printf("Enter a name: ");
        scanf("%s", name); 

        printf("Enter a date (dd/mm/yyyy): ");
        scanf("%s", date); 

        char buffer[1000];

        ssize_t bytes_read;
        while ((bytes_read = read(fd_input, buffer, sizeof(buffer))) > 0) {
            char *token = strtok(buffer, " \t\n");
            // Use strtok (string token) to split the line into words based on spaces, tabs, and newlines

            while (token != NULL) {
                // Examine each word in the current buffer

                if (strcmp(token, nameFile) == 0) {
                    // Check if the word is the name to be replaced in the input file 
                    write(pipefd[1], name, strlen(name)); // Write the user's name to the pipe
                } 
                else if (strcmp(token, dateFile) == 0) {
                    // Check if the word is the date to be replaced in the input file 
                    write(pipefd[1], date, strlen(date)); // Write the user's date to the pipe
                } 
                else {
                    // If the word is not [name] or [date], it should be copied as is
                    write(pipefd[1], token, strlen(token)); // Write the word to the pipe
                }

                write(pipefd[1], " ", 1); // Write a space after each word
                token = strtok(NULL, " \t\n"); // Move to the next word (token)
            }
        }

        // Close file descriptors and the write end of the pipe
        close(fd_input);
        close(pipefd[1]);

        // Exit child process
        exit(0);
    } 
    
    // Parent
    else {
    printf("You will be asked to enter your name and the date to sign the terms and conditions form.\n");
        //close write end of pipe 
        close(pipefd[1]);

        // Open a new output file using file descriptor
        int fd_output = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd_output == -1) {
            perror("Error opening output file");
            exit(1);
        }

        char buffer[1000];
        ssize_t bytes_read;

        // Read from the pipe and write to the output file
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            write(fd_output, buffer, bytes_read);
        }

        // Close file descriptors
        close(fd_output);
        close(pipefd[0]);

        // Wait for child
        wait(NULL);
        printf("You agreed to the terms and conditions, thank you.\n");
        sleep(3);
        // Create a new process to execute the "clear" command
        pid_t clear_pid = fork();

        if (clear_pid < 0) {
            perror("Fork failed for clear");
            exit(1);
        } 

        if (clear_pid == 0) {
            // Execute the "clear" command
            execlp("clear", "clear", NULL);

            // If execlp fails, print an error message
            perror("execlp failed");
            exit(1);
        } 

        // Wait for the "clear" command process to finish
        wait(NULL);
    }
    
    return 0;
}
