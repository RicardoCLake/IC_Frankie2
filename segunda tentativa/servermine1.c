// Server that answers ping
// Use with clientmine 1 and 2

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/time.h>
#include "funcIC.h"
#define PORT 7000 

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket; 
    struct sockaddr_in local_addr; // Thats an strct that contains the IP and the Port of our destiny 
    int opt = 1; 
    int addrlen = sizeof(local_addr);   
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port  
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons( PORT ); 
    local_addr.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY reffered for the localhost
    
    printf("Defined the local address on port %d\n", PORT); 

    // Forcefully attaching socket to the port 
    if (bind(server_fd, (struct sockaddr *)&local_addr, sizeof(local_addr))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    printf("Server is listening...\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&local_addr, (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    printf("The connection is done with a client!\n");
    //At this time, the connection is done.

    char buffer[1024] = {0};
    int valread, times = 0, end_point;


    read (new_socket, buffer, 1024);
    printf("buffer = %s\n", buffer );
    for (end_point = 0; buffer[end_point] != 0; end_point++);
    for (int i = 0; i < end_point; i++)
    {
        times += PotInt(10, end_point - i - 1) * (buffer[i] - '0');
    }
    printf("times = %d\n", times);

    for (int i = 0; i < times; i++)
    {
        valread = read (new_socket ,buffer, 1024);
        send(new_socket , "hello" , strlen("hello") , 0); 
        //printf("%d - valread: %d\n", i + 1, valread); 
    }
    return 0; 
} 

