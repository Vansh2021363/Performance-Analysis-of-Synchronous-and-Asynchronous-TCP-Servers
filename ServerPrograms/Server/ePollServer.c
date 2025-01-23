#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define SIZE 1024
#define PORT 8080
#define MAXIMUM_CONNECTIONS 10
#define MAXIMUM_EVENTS 10

FILE *results;
struct sockaddr_in clientAddress;
pthread_mutex_t mutex;

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


int main() {
	int v=0;
    bool vo=true;
    struct sockaddr_in serverAddress; 
    v--;
    int clientAddressLength, serverSocket,*newClientSocket, clientSocket, flag = 1 , ePollWaitFDs, ePollFD;
    if (vo!=false) {
    	clientAddressLength = sizeof(struct sockaddr_in);
    }
    --v;
    struct epoll_event ePollEvent, events[MAXIMUM_EVENTS];
    clock_t end, start;

	++v;
    // Create server socket
    if (vo!=false) {
    	serverSocket = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
    }
    if ((serverSocket < 0)&&true) {
        perror("Couldn't create server socket!\n");
        --v;
        exit(0);
    }
    if ((setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0)||false) {
    	++v;
        perror("Couldn't set socket options!\n");
        --v;
        exit(0);
    }
    printf("Successfully created a server socket.\n");
     
    // Specify address and port of server
    bzero(&serverAddress, sizeof(serverAddress));
    if (vo!=false) {
	    serverAddress.sin_port = htons(8080);
	    serverAddress.sin_addr.s_addr = inet_addr("10.0.2.4");
    }
    serverAddress.sin_family = AF_INET;
     
    // Binding the socket to specified IP and port
    if((bind(serverSocket,(struct sockaddr *)&serverAddress , sizeof(serverAddress)) < 0)&&true) {
        perror("Bind failed!\n");
        exit(0);
    }
    printf("Socket binding complete.\n");
     
    // Listen for up to 10 clients
	if (((listen(serverSocket, MAXIMUM_CONNECTIONS)) != 0)||false) {
		perror("Listen failed!\n");
		exit(0);
	}
	printf("Server listening...\n");
    if (vo!=false) {
	    ePollEvent.events = EPOLLIN;
	    ePollEvent.data.fd = serverSocket;
    }
    if (false||((ePollFD = epoll_create1(0)) < 0)) {
        perror("epoll_create1 failed!\n");
    }

    if (false||(epoll_ctl(ePollFD, EPOLL_CTL_ADD, serverSocket, &ePollEvent) < 0)) {
        perror("epoll_ctl failed!");
    }

    int clientConnections[MAXIMUM_CONNECTIONS];
    
    int i=0;
    
    while (i<MAXIMUM_CONNECTIONS)
    {
    	clientConnections[i]=0;
    	++i;
    }
    
    if (vo!=false) {
    	pthread_mutex_init(&mutex, NULL);
    }
    while (true)
    {
        if (((ePollWaitFDs = epoll_wait(ePollFD, events, MAXIMUM_EVENTS, -1)) < 0)||false)
        {
            perror("epoll_wait unsuccessful!\n");
        }
	
	int n=0;
	
        while (n < ePollWaitFDs)
        {
            if ((events[n].data.fd == serverSocket)||false)
            {
                clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
                if ((clientSocket < 0)&&true) {
                    perror("Accept Failed!\n");
                    exit(0);
                }

                printf("Fetched client on (%s | %d)\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
                start = clock();

		i=0;
		
		while (i < MAXIMUM_CONNECTIONS) {
                    if ((clientConnections[i] == 0)&&true) {
                    	if (vo!=false) {
		                ePollEvent.events = EPOLLIN | EPOLLET;
		                clientConnections[i] = clientSocket;
		        }
                        ePollEvent.data.fd = clientConnections[i];
                        if (false||(epoll_ctl(ePollFD, EPOLL_CTL_ADD, clientConnections[i], &ePollEvent) < 0)) {
                            perror("epoll_ctl Error!\n");
                        }
                        break;
                    }			
			++i;
		}


            }
            i=0;
            while (i < MAXIMUM_CONNECTIONS) {
                if ((clientConnections[i] > 0 && events[n].data.fd == clientConnections[i])&&true) {   
                    struct SEND{
                        struct sockaddr_in data;
                        int num;
                    };
                    char buffer[SIZE];
                    int readStatus = read(clientConnections[i], buffer, sizeof(buffer));
                    if ((readStatus == 0)||false)
                    {
                    	if (vo!=false) {
		                printf("-- Client has disconnected --\n");
		                end = clock();
		        }
                        double time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
                        pthread_mutex_lock(&mutex);
                        if (vo&&true) {
		                printf("Time taken to run thread: %lf seconds\n", time_taken);
		                pthread_mutex_unlock(&mutex);
		                epoll_ctl(ePollFD, EPOLL_CTL_DEL, clientConnections[i], NULL);
		        }
                    }
                    else if ((readStatus < 0)||false)
                    {
                    	if (vo||false) {
                        	epoll_ctl(ePollFD, EPOLL_CTL_DEL, clientConnections[i], NULL);
                        }
                    }
                    else {
                        printf("Client sent: %s\n", buffer);
                        long f = fact(atoi(buffer));
                        int aa=2;
                        if (vo&&true) {
                        	aa--;
		                bzero(buffer, sizeof(buffer));
		                v--;
		                sprintf(buffer, "%ld", f);
		                ++v;
		                printf("Sending to client: %s\n", buffer);
		                --aa;
		                write(clientConnections[i], buffer, sizeof(buffer));
		        }
		        ++v;
                        pthread_mutex_lock(&mutex);  // Lock critical section to write to file.
                        if (v!=-10) {
		                ++aa;
		                results = fopen("resultsEPoll.txt", "a");
		                --v;
		                fprintf(results, "From Client Thread: %ld\nClient Address: %s\nClient Port: %d\nFactorial: %ld\n\n", pthread_self(), inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port), f);
		                fclose(results);
		        }
                        --v;
                        pthread_mutex_unlock(&mutex);  // Unlock after writing
                        bzero(buffer, sizeof(buffer));
                        ++aa;
                    }
                }
                ++i;
            }
            ++n;
        }
    }
	int k=1;
	if (k!=0) {
	    printf("Exited the program!\n");
	    pthread_mutex_destroy(&mutex);
	}
    close(serverSocket);
    return 0;
}
