#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include<netdb.h>


#include<sys/time.h>

int main(int argc, char *argv[])
{
    if(argc!=3){
        printf("Missing host name and port name\n");
        exit(0);
    }

	char *serverName = argv[1];
	int port  = atoi(argv[2]);

    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    int server_struct_length = sizeof(server_addr);
	struct hostent* host = gethostbyname(serverName);
    
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    
    // Create socket:
    int socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = **(struct in_addr **)host->h_addr_list;;
    
	while(1){

		// Get input from the user:
		printf("Enter message: ");
		gets(client_message);
		
		// Send the message to server:
		if(sendto(socket_desc, client_message, strlen(client_message), 0,
			(struct sockaddr*)&server_addr, server_struct_length) < 0){
			printf("Unable to send message\n");
			return -1;
		}
		
		// Receive the server's response:
		if(recvfrom(socket_desc, server_message, sizeof(server_message), 0,
			(struct sockaddr*)&server_addr, &server_struct_length) < 0){
			printf("Error while receiving server's msg\n");
			return -1;
		}
		
		printf("Server's response: %s\n", server_message);
		
	}

    
    
    // Close the socket:
    close(socket_desc);
    
    return 0;
}

