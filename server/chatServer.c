#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void broadcast(char* msg, int* clients, int numClients);

int main(int argc, char** argv)
{
    uint16_t port = 3000;
    char buf[200];
    int rMsg;
    struct sockaddr_in* server = malloc(sizeof(struct sockaddr_in));
    struct sockaddr_in* client = malloc(sizeof(struct sockaddr_in)); 
    int clientlength = sizeof(client);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        puts("Bad Socket!!!!");
    }
    
    server->sin_family = AF_INET;
    server->sin_addr.s_addr = inet_addr("127.0.0.1");
    server->sin_port = htons( port );
    memset(server->sin_zero, '\0', sizeof server->sin_zero);

    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}
    socklen_t serverSize = sizeof(server);
    
    int bindfd = bind(sockfd, (struct sockaddr *)server, sizeof(struct sockaddr));
    if(bindfd < 0)
    {
        puts("Problem with binding...\n");
    }
    int listenfd;
    int clientfd;
    int MAX_CLIENTS = 1000;
    int* clients = malloc(MAX_CLIENTS * sizeof(int));
    int numberOfConnectedClients = 0;
    
    while(1)
    {
        listenfd = listen(sockfd, 100);
    
        printf("Listening....\n");
        clientfd = accept(sockfd ,  (struct sockaddr *)server , &serverSize);
        printf("Client Connected.... %d\n", clientfd);
        
        //add this client to our array of clients
        clients[numberOfConnectedClients++] =  clientfd;
        char* message = "hello";
        broadcast(message, clients, numberOfConnectedClients);
        int newsockfd = accept(sockfd, (struct sockaddr *)client , &clientlength);
        char* client_reply = malloc(2000 * sizeof(char));
        int error = recv(sockfd, client_reply, sizeof(client_reply), 0);
        
        if(error < 0)
        {
            perror("Error");
        }
        puts(client_reply);
        rMsg = read(newsockfd, buf, strlen(buf));
        if(rMsg < 0)
        {
            perror("Error");
        }
        
        printf("%s", buf);
        break;
    }
    //write(clientfd, buffer, sizeof(buffer));
}

void broadcast(char* msg, int* clients, int numClients)
{
    int i;
    for(i = 0; i < numClients; i++)
    {
        send(*(clients + (i * sizeof(int))) , msg , strlen(msg) , 0);
        printf("Sending to client: %d", clients + (i * sizeof(int)));
    }
}