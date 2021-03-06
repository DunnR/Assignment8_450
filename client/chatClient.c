#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    uint16_t port = 3000;
    int error;
    int wMsg;
    char buf[200] = "Message sent to the server";
    struct sockaddr_in* server = malloc(sizeof(struct sockaddr_in));
    server->sin_addr.s_addr = inet_addr("127.0.0.1");
    server->sin_family = AF_INET;
    server->sin_port = htons( port );
    //memset(server->sin_zero, '\0', sizeof server->sin_zero);
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    error = connect(sockfd, (struct sockaddr *)server, sizeof(struct sockaddr));
    if(error < 0)
    {
        puts("Problem with connecting...\n");
    }
    else
    {
        puts("We are Connected");
        //Receive a reply from the server
        
        char* server_reply = malloc(2000 * sizeof(char));
        while(1)
        {
            error = recv(sockfd, server_reply, sizeof(server_reply), 0);
            if(error < 0)
            {
                puts("recv failed");
            }
            else
            {
                puts("Reply received\n");
                puts(server_reply);
                send(sockfd, buf, strlen(buf), 0);
                if(wMsg < 0)
                {
                    perror("Error");
                }
                break;
            }
        }
    }
    free(server);
    return 0;
}