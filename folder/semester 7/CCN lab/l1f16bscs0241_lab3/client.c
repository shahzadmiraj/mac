// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define MAX 80 
#define PORT	 8080 
#define MAXLINE 1024 
 
// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from client"; 
	struct sockaddr_in	 servaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 



	// connect the client socket to server socket 
	    if (connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	    } 
	    else
		printf("connected to the server..\n"); 
	
	  
 	char buff[MAX]; 
   	 int n; 
   
	send( sockfd ,  hello , strlen( hello) , 0 ); 
   	 printf("Hello message sent\n"); 
         n = recv(sockfd , buff, MAX,0); 
	buff[n]='\0';
   	 printf("%s\n",buff ); 
	 // close the socket 
	    close(sockfd); 
	  
	   
	return 0; 
} 

