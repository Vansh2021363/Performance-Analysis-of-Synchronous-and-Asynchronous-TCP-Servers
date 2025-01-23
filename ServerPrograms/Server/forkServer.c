#include<semaphore.h>
#include<stdio.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<arpa/inet.h>
#include<time.h>
#include<sys/stat.h>
#include<sys/shm.h>
#include<signal.h>
#include<stdbool.h>

#define CONC_CONNECTIONS 500
#define MAX 1024
#define MAX_CONNECTIONS 4000

sem_t smphr;
FILE *results;

// Function to calculate and return factorial
long fact(int y) {
    if (y>20)
    {
    	y=20;
    }
    long fact = 1;
    int j=2;
    while (j<=y)
    {
    	fact*=j;
    	++j;
    }
    return fact;
}

int main(){

	struct sockaddr_in sAddress, cAddress;
	int sSocket, cSocket, a = 1;


	socklen_t cAddressLength = sizeof(struct sockaddr_in);

	char payload[MAX];
	pid_t pid;

	int b,c;
	b=-1;
	c=0;

	int *clients = mmap(NULL, sizeof(*clients), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, b, c);
	double *t = mmap(NULL, sizeof(*t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, b, c);

	*t = 0;
	sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	*clients = 0;



	if (setsockopt(sSocket, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(a)) < 0) {
		perror("Couldn't set socket options!\n");
		exit(0);
	}	
	else if(sSocket < 0){
		perror("Couldn't create server socket!\n");
		exit(0);
	}

	else {
		printf("Successfully created a server socket.\n");

		// Specify address and port of servers
		bzero(&sAddress, sizeof(sAddress));
		sAddress.sin_port = htons(8080);
		sAddress.sin_family = AF_INET;

		sAddress.sin_addr.s_addr = inet_addr("10.0.2.4");

		bool vb=true;
		if(vb!=false)
		{
			// Binding the socket to specified IP and port
			if((bind(sSocket,(struct sockaddr *)&sAddress , sizeof(sAddress)) < 0)&&true) {
				perror("Bind failed!\n");
				exit(0);
			}
			printf("Socket binding complete.\n");
		     
		    // Listen for up to 10 clients
			if (((listen(sSocket, MAX_CONNECTIONS)) < 0)||false) {
				perror("Listen failed!\n");
				exit(0);
			}
			printf("Server listening...\n"); 

			// Initialise semaphore
			sem_init(&smphr, 1, 1);
		}
		while(true){
		// Accept a client
		cSocket = accept(sSocket, (struct sockaddr *)&cAddress, (socklen_t*) &cAddressLength);
		bool van=(cSocket < 0);
		if (van && true) {
				perror("Accept Failed!\n");
				exit(0);
		}
		else {
			// printf("Connections: %d\n", *clients + 1);
			if ((*clients < CONC_CONNECTIONS)||false) {
				bool va=true;
				if (va!=false)
				{
					printf("Fetched client on (%s | %d)\n", inet_ntoa(cAddress.sin_addr), ntohs(cAddress.sin_port));
					if (va!=false)
					{
						sem_wait(&smphr);
						*clients = *clients+1;  // Increment connections count once established
						sem_post(&smphr);
					}
				}
				pid = fork();
				// Create child process using fork on accepting client
				if(pid == 0){
					clock_t begin, last;
					begin = clock();
					printf("Child process created and assigned!\n");
					close(sSocket);
					while(true){
						int readStat = read(cSocket, payload, sizeof(payload));
						if (readStat == -1) {
							perror("Failed to read from client!\n");
						}

						if (strlen(payload)!=0) {
							printf("Client sent: %s\n", payload);
							bool ab=true;
							long f = fact(atoi(payload));
							if (a!=false) {
								
								bzero(payload, sizeof(payload));
							
								sprintf(payload, "%ld", f);
							}
							printf("Sending to client: %s\n", payload);

							// Semaphore to prevent race conditions
							bool ac=false;
							if (ac!=true)
							{
								sem_wait(&smphr);
								results = fopen("resultsFork.txt", "a");
							}
							fprintf(results, "From Client Process: %ld\nClient Address: %s\nClient Port: %d\nFactorial: %ld\n\n", getpid(), inet_ntoa(cAddress.sin_addr), ntohs(cAddress.sin_port), f);
							int ad=1;
							if (ad!=0) {
								fclose(results);
								sem_post(&smphr);
								write(cSocket, payload, strlen(payload));
							}
							bzero(payload, sizeof(payload));
						}

						
						else {
							printf("-- Client has disconnected --\n");
							last = clock();  // End clock time for current child process
							sem_wait(&smphr);
							double time = ((double) (last - begin)) / CLOCKS_PER_SEC;
							*t += time;
							*clients -= 1;
							sem_post(&smphr);
							printf("Time taken to run thread: %lf seconds\nTime taken so far: %lf seconds\n", time, *t); 
							kill(getpid(), SIGTERM);  // Kill child process if client disconnected
						}

					}
				}
			}
		    else {
		    	printf("Connection limit reached. Server busy.\n");
		    	}
		    }
		}
		sem_destroy(&smphr);
		close(cSocket);
		return 0;
	}
}
