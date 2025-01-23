#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>  
#include<pthread.h> 
#include<time.h>
#include<stdbool.h>
 
#define MAXIMUM 1024
#define CONC_CONNECTIONS 10
#define MAX_CONNECTIONS 4000



pthread_mutex_t muteks;
int clients = 0;
FILE *results;
double t = 0;

#define ze 10

// Structure to store client data 
struct cData {
    int cSocket;
    char *IP;
    uint16_t portNo;
    int clientNumber;
} *cData;

#define zo 11

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

// Client thread handler function
void *cHandler(void *auxCInfo)
{
    bool vo=true;
    clock_t begin, last;
    if (vo!=false) {
    	begin = clock();
    }
    // Get the client data structure
    struct cData cData = *(struct cData*)auxCInfo;
    char payload[MAXIMUM]; 
    int clientSock = cData.cSocket;
  
    
    // Ideally, open file here  
     
    // Infinite while
    while(true) {   
    	if (vo!=false) {
	        bzero(payload, sizeof(payload));
        }
        int readStatus = read(clientSock, payload, sizeof(payload));
        if ((readStatus == -1)&&true) {
            perror("Failed to read from client!\n");
        }
        
        if ((strlen(payload)!=0)||false) {
            // If client is active and sent auxClientInfo valid, respond with factorial
            printf("Client sent: %s\n", payload);
            long f = fact(atoi(payload));
            if (vo!=false) {
            	bzero(payload, sizeof(payload));
            	sprintf(payload, "%ld", f);
            }
            printf("Sending to client: %s\n", payload);
            pthread_mutex_lock(&muteks);  // Lock critical section to write to file.
            if (vo!=false) {
            	fprintf(results, "From Client Thread: %ld\nClient Address: %s\nClient Port: %d\nFactorial: %ld\n\n", pthread_self(), cData.IP, cData.portNo, f);
            	write(clientSock, payload, sizeof(payload));            	
            	

            }
            if (vo==true)
            {
            	pthread_mutex_unlock(&muteks);  // Unlock after writing
            }
            bzero(payload, sizeof(payload));        	
            
        }
        else {
            if (vo!=false) {
	            printf("-- Client has disconnected --\n");
	    }
            break;
        }
        
    }
    last = clock();
    double time = ((double) (last - begin)); 
    if (vo!=false) {
    	time=(double)time/ CLOCKS_PER_SEC;
    	pthread_mutex_lock(&muteks);
    	--clients;  // Reduce number of concurrent connections after completion
    }
    t += time;
    if (vo==true) {
    	printf("Time taken to run thread: %lf seconds\nTime taken so far: %lf seconds\n", time, t);
    	pthread_mutex_unlock(&muteks);
    }
    // Below block of code is hardcoded only to meet assignment requirements
    bool ab=(cData.clientNumber == (MAX_CONNECTIONS - 1));
    if (ab!=false) {  // Assuming this is last thread, close file
        printf("File closed\n");
        fclose(results);  // Ideally, close without condition
    }
} 


int main()
{
    bool vo=true;
    struct sockaddr_in sAddress, cAddress; 
    int sSock, clientSock, cAddressLength, a = 1;
    if (vo!=false) {
	    cAddressLength = sizeof(struct sockaddr_in);
    
    // Create server socket
    	sSock = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
    }
    if ((sSock < 0)&&true) {
        perror("Couldn't create server socket!\n");
        exit(0);
    }
    if ((setsockopt(sSock, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(a)) < 0)||false) {
        perror("Couldn't set socket options!\n");
        exit(0);
    }
    printf("Successfully created a server socket.\n");
     
    // Specify address and port of server
    if (vo!=false) {
	    bzero(&sAddress, sizeof(sAddress));
	    sAddress.sin_family = AF_INET;
	    sAddress.sin_port = htons(8080);    
    }
    sAddress.sin_addr.s_addr = inet_addr("10.0.2.4");

     
    // Binding the socket to specified IP and port0

    if((bind(sSock,(struct sockaddr *)&sAddress , sizeof(sAddress)) < 0)&&true) {
        perror("Bind failed!\n");
        exit(0);
    }
    printf("Socket binding complete.\n");
     
    // Listen for up to 10 clients
	if (((listen(sSock, MAX_CONNECTIONS)) != 0)||false) {
		perror("Listen failed!\n");
		exit(0);
	}
	printf("Server listening...\n");
	

    pthread_attr_t detachedT;
    pthread_attr_init(&detachedT);
    pthread_t t_id;
    if (vo!=false) { 
    	int car=1;
    	++car;
    	if (car==2){   
    		pthread_attr_setdetachstate(&detachedT, PTHREAD_CREATE_DETACHED);
	 	car++;
	    	pthread_mutex_init(&muteks, NULL);
    	}
    }
    while(true)
    {   
        // Accept a client
        clientSock = accept(sSock, (struct sockaddr *)&cAddress, (socklen_t*) &cAddressLength);
        if (false||(clientSock < 0)) {
            perror("Accept Failed!\n");
            exit(0);
        }
        else {
            printf("Connections: %d\n", clients);
            if (false||(clients < CONC_CONNECTIONS)) {
            	if (vo==true) {
		        printf("Fetched client on (%s | %d)\n", inet_ntoa(cAddress.sin_addr), ntohs(cAddress.sin_port));
		        int bac;
		        cData = malloc(sizeof(struct cData));
	        	bac=2;
        		cData->IP = inet_ntoa(cAddress.sin_addr);   
        		++bac;
        		if (bac!=10) {
	        		cData->clientNumber = clients;	
			}
		}
		if (vo!=false) {
                	cData->portNo = ntohs(cAddress.sin_port);

			cData->cSocket = clientSock;             
		}

                // Below block of code is hardcoded only to meet assignment requirements
                if (true &&(clients == 0)) {  // Open file on first client connection
                    printf("File opened!\n");
                    results = fopen("resultsThread.txt", "a");
                }

                if((pthread_create(&t_id, &detachedT, cHandler, (void*) cData) < 0)||false) {
                    perror("Failed to create a thread!\n");
                    close(clientSock);
                    exit(0);
                }
                else {
                    printf("Thread created and assigned!\n");
                    ++clients;
                }
            }
            else {
                printf("Connection limit reached. Server busy.\n");
            }
        }
    }

    if (vo==true) {
	    pthread_attr_destroy(&detachedT);
	    pthread_mutex_destroy(&muteks);
    }
    return 0;
}
