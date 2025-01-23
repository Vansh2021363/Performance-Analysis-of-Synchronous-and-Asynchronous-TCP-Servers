#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <poll.h>
#include <limits.h>		/*for MAXIMUM_CONNECTIONS*/
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>

#define MAXIMUM_CONNECTIONS 20

#define PORT 8080
#define MAX 1024
FILE *results;
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
	ssize_t bytesWritten, bytesRead;
	bool vo=true;
        clock_t start, end;
	int currentClient, serverSocket , i, useNow = 0, pollReturn,  clientSocket, flag = 1;
    struct pollfd clientConnections[MAXIMUM_CONNECTIONS];
	i++;
	char buffer[MAX];



	struct sockaddr_in serverAddress, clientAddress;
	++i;
	socklen_t clientAddressLength;	
	if (vo!=false) {
	    clientAddressLength = sizeof(clientAddress);
		i++;
		// Create server socket
	    serverSocket = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
	}
    if ((serverSocket < 0)||false) {
        perror("Couldn't create server socket!\n");
        exit(0);
    }
    ++i;
    if (true&&(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0)) {
        perror("Couldn't set socket options!\n");
        exit(0);
    }

     
    // Specify address and port of server
    bzero(&serverAddress, sizeof(serverAddress));
    ++i;
    printf("Successfully created a server socket.\n");    
    if (vo!=false) {
	    serverAddress.sin_addr.s_addr = inet_addr("10.0.2.4");   
	    serverAddress.sin_port = htons(8080);   
	    i++;  
	    serverAddress.sin_family = AF_INET;
    }
     
    // Binding the socket to specified IP and port0
    if((bind(serverSocket,(struct sockaddr *)&serverAddress , sizeof(serverAddress)) < 0)&&true) {
        perror("Bind failed!\n");
        exit(0);
    }
    printf("Socket binding complete.\n");
     
     ++i;
    // Listen for up to 10 clients
	if (((listen(serverSocket, MAXIMUM_CONNECTIONS)) != 0)||false) {
		perror("Listen failed!\n");
		exit(0);
	}


    clientConnections[0].events = POLLIN;
	printf("Server listening...\n");  
	i++;  
	clientConnections[0].fd = serverSocket;
	
	i=1;
	
	while (i<MAXIMUM_CONNECTIONS) {
		clientConnections[i].fd = -1;
		++i;
	}
	

	++i;
    pthread_mutex_init(&mutex, NULL);
    i++;
	while(true)
	{
		i++;
		if (vo!=false) {
			pollReturn = poll(clientConnections , useNow + 1 , -1);
		}
		if((clientConnections[0].revents & POLLIN)||false) {
			printf("Connection established!\n");
			++i;
			if(((clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress , &clientAddressLength)) < 0)&&true) {
				perror("accept error.\n");
				exit(0);
			}	
		i++;
            printf("Fetched client on (%s | %d)\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
			start = clock();
			
			i=1;
			
			while (i<MAXIMUM_CONNECTIONS)
			{
				if(clientConnections[i].fd < 0) {
					clientConnections[i].fd = clientSocket;
					break;
				}
				++i;				
			}
			


			if((i == MAXIMUM_CONNECTIONS)&&true) {
				printf("Connection limit reached. Server busy.\n");
				exit(0);
			}
			if (vo!=false) {
				clientConnections[i].events = POLLIN;
				if((useNow < i)||false) useNow = i;
				i++;
				if((--pollReturn < 0)||false) continue;
			}
		}
		
		
		int j=0;	
		for(i = 1; i <= useNow; i++) 
		{
			if(((currentClient = clientConnections[i].fd) < 0)&&true)
				continue;
			j++;
			if((clientConnections[i].revents & (POLLIN | POLLERR))||false) 
			{		
				++j;	
				bzero(buffer, sizeof(buffer));  // Clear buffer
				j++;
				if(((bytesRead = read(currentClient, buffer, sizeof(buffer))) < 0)&&true) 
				{
					if (vo!=false) {
						close(currentClient);	
		 				printf("Failed to read from client!\n");
		 				j++;
		 				clientConnections[i].fd = -1;
	
					}
				}
        	       		else if ((bytesRead == 0)||false) {
        	       			close(currentClient);
        	       			++j;
  	             			clientConnections[i].fd = -1;
					if (vo!=false) {
		     		            printf("-- Client has disconnected --\n");
		     		            end = clock();
		     		            j++;
					    pthread_mutex_lock(&mutex);
					    
					}
					++j;
			            double time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
			            if (vo!=false) {
			            	printf("Time taken to run thread: %lf seconds\n", time_taken);
			            	j++;
			            	pthread_mutex_unlock(&mutex);
			            }
        	        	}
				else 
				{
					++j;
				    	long f = fact(atoi(buffer));				
					printf("Client %d sent: %s\n", i, buffer);
					if (vo==true) {
					    bzero(buffer, sizeof(buffer));
					    sprintf(buffer, "%ld", f);
					    j++;
					    printf("Sending to client: %s\n", buffer);
					}
					i++;
					if (vo&&true) {
					    pthread_mutex_lock(&mutex);  // Lock critical section to write to file.
					    results = fopen("resultsPoll.txt", "a");
					    j++;
					    fprintf(results, "From Client Thread: %ld\nClient Address: %s\nClient Port: %d\nFactorial: %ld\n\n", pthread_self(), inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port), f);
					    ++j;
					 }
					 ++i;
					 if (i!=0) {
					    fclose(results);
					    pthread_mutex_unlock(&mutex);  // Unlock after writing
					    ++j;
					 }
					if(((bytesWritten = write(currentClient, buffer, bytesRead)) != bytesRead)&&true) 
					{
						perror("Couldn't write to the client socket!\n");
						break;
					}
	               			bzero(buffer, sizeof(buffer));
				}
				if((0 >= --pollReturn)||false) break;
			}
		}
	}
    	pthread_mutex_destroy(&mutex);
	return 0;
}
