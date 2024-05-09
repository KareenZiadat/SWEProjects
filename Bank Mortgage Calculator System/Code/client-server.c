#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h> 
#include <math.h> 
#include <pthread.h>

double calculateMortgage(double principal, double interestRate, int numPayments) {
    double monthlyInterest = interestRate / 12.0 / 100.0;
    double term = pow(1 + monthlyInterest, numPayments);
    double monthlyPayment = (principal * monthlyInterest * term) / (term - 1);
    return monthlyPayment;
}

void * serverTH(void * soc){
	     //cast the socket to int (file descriptor)
	     int n;
	     int sfd = (intptr_t)(soc);
	
	
	    double principal, interestRate;
            int numPayments;
	    char clientName[256];
	    char clientLastName[256];
	    
	    n = read(sfd, clientName, sizeof(clientName));
	    n = read(sfd, clientLastName, sizeof(clientLastName));
            n = read(sfd, &principal, sizeof(double));
            n = read(sfd, &interestRate, sizeof(double));
            n = read(sfd, &numPayments, sizeof(int));

            double result = calculateMortgage(principal, interestRate, numPayments);
            n = write(sfd, &result, sizeof(double));
            
            // Save client's name and monthly payment to loans.txt
	    FILE *file = fopen("loans.txt", "a"); // Open the file in append mode
	    if (file != NULL) {
		fprintf(file, "Client: %s %s, Monthly Payment: %.2lf\n", clientName, clientLastName, result);
		fclose(file);
	    } else {
		printf("Error opening loans.txt for writing.\n");
	    }

            
            close(sfd);
}

int main(int argc, char *argv[]) {
    int clients=0;
    printf("How many clients do you want to allow to access the server? ");
    scanf("%d",&clients);
    
    pthread_t sth;
    pid_t pid;
    pid = fork();
    if (pid != 0) {
        int sockfd, newsockfd, portno, clilen, n;
        char buffer[256];

        struct sockaddr_in serv_addr, cli_addr;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd < 0)
            printf("ERROR opening socket");

        bzero((char *)&serv_addr, sizeof(serv_addr));
        portno = atoi(argv[2]);

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(portno);
        serv_addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
            printf("ERROR on binding");

        while (1) {
            printf("server is listening...");
            listen(sockfd, 5);
            
            clilen = sizeof(cli_addr);
            newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen); //creates socket for connection
            if (newsockfd < 0){
                printf("ERROR on accept");
                exit (0);
            }
           pthread_create(&sth,NULL,serverTH,(void *)(intptr_t)newsockfd);
           pthread_join(sth,NULL);
        }
    } else {
        int sockfd, portno, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        char buffer[256];
        for (int i = 0; i < clients; ++i) {
            server = gethostbyname(argv[1]);
            if (server == NULL) {
                fprintf(stderr, "ERROR, no such host");
                exit(0);
            }
            portno = atoi(argv[2]);
            bzero((char *)&serv_addr, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
            serv_addr.sin_port = htons(portno);
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                printf("ERROR connecting");
	    
	    
            double principal, interestRate;
            int numPayments;
	    char clientName[256];
	    char clientLastName[256];

	    printf("Enter the client's first name: ");
            scanf("%s", clientName);
            printf("Enter the client's last name: ");
            scanf("%s", clientLastName);
            printf("Please enter the principal amount: ");
            scanf("%lf", &principal);
            printf("Please enter the annual interest rate: ");
            scanf("%lf", &interestRate);
            printf("Please enter the number of payments: ");
            scanf("%d", &numPayments);

            n = write(sockfd, clientName, sizeof(clientName));
            n = write(sockfd, clientLastName, sizeof(clientLastName));
            n = write(sockfd, &principal, sizeof(double));
            n = write(sockfd, &interestRate, sizeof(double));
            n = write(sockfd, &numPayments, sizeof(int));

            double monthlyPayment;
            n = read(sockfd, &monthlyPayment, sizeof(double));
            printf("(Client) Monthly Payment: %.2lf\n\n", monthlyPayment);
        } //end of for
    }   //end for fork
    
   //exec
   // Use execl to run the statistics.o program
    int status;
    pid_t child_pid = fork();

    if (child_pid == 0) {
      // This code will be executed by the child process
      printf("Running statistics for the day...\n");

      // Use execl to run statistics.o with the argument "loans.txt"
      execl("./statistics.o", "./statistics.o", "loans.txt", NULL);

      // If execl fails, it will reach here
      perror("execl");
      exit(1);
    } else if (child_pid < 0) {
      // Fork failed
      perror("fork");
      exit(1);
    } else {
      // This code will be executed by the parent process
      waitpid(child_pid, &status, 0);
      printf("statistics.o program completed.\n");
    }
    return 0;
}

