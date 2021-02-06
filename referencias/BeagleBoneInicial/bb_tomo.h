// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <netdb.h>
#define PORT 8081 

void bb_init(int socket);
void bb_setPattern(int socket, int skip, int curr);
