// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT	 8080 

#define MAX 80 

// Function designed for chat between client and server. 



// Driver code 
int main() { 
	int sockfd; 
	char *hello = "Hello from server"; 
	struct sockaddr_in servaddr, cliaddr; 
	
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	// Now server is ready to listen and verification 
	    if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	    } 
	    else
		printf("Server listening..\n"); 

		int len,connfd; 


		  len = sizeof(cliaddr); 
	  
	 




	char buff[MAX]; 
    	int n ;
	
	while(1)
	{


	// Accept the data packet from client and verification 
	    connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len); 
	    if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	    } 
	    else
		printf("server acccept the client...\n"); 


         n = recv(connfd ,(char *) buff, MAX,0);
	buff[n]='\0'; 
   	 printf("%s\n",buff ); 
    	send( connfd ,  hello , strlen( hello) , 0 ); 
   	 printf("Hello message sent\n"); 
	// After chatting close the socket 
	    close(connfd); 

   	}
	    



	
	return 0; 
} 

