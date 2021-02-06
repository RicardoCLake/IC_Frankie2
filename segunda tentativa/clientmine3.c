//Master from a ptp simple
//Use with servermine3

#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/time.h>
#include "funcIC.h"

#define MICRO_PER_SECOND 1000000
#define PORT 7000 
//#define IPS "192.168.6.2"
#define IPS "169.254.27.21"
#define TIMES 1000000
#define LENGTH 600       //of the graphic
#define INTERVAL 2    //of each collum of the graphic 
    //(LENGTH*INTERVAL=MaxTime in Graphic, in microseconds)

int main(int argc, char const *argv[]) 
{ 
    int sock = 0; 
    struct sockaddr_in serv_addr; 

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    }
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, IPS, &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
    printf("Defined the server address: %s/%d\n", IPS, PORT);
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    printf("Yeah man, your conection was accepted by the server!\n");

    simpleMaster (sock, 1);

    return 0; 
} 

