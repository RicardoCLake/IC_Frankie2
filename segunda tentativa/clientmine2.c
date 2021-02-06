//Shows pings with a histogram
//Use with servermine1

#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/time.h>
#include "funcIC.h"

#define MICRO_PER_SECOND 1000000
#define PORT 7000 
#define IPS "192.168.6.2"
//#define IPS "169.254.27.21"
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

    struct timeval time1;
    struct timeval time2;
    char buffer[1024] = {0};
    long utime1, utime2, utime, sum = 0; 
    int utimes[TIMES], occurrences[LENGTH];
    char s_times[13];
    
    sprintf(s_times, "%d", TIMES);
    send(sock , s_times , 13 , 0);
    delayClocks(5000);

    //pinging
    for (int i = 0; i < TIMES; i++)
    {   
        send(sock , "hellohello" , strlen("hellohello") , 0 ); 
        gettimeofday (&time1, NULL);
        read (sock , buffer, 1024);
        gettimeofday(&time2, NULL);
        utime1 = time1.tv_sec * MICRO_PER_SECOND + time1.tv_usec; 
        utime2 = time2.tv_sec * MICRO_PER_SECOND + time2.tv_usec;
        utime = utime2 - utime1;
        printf("%d _ time2 - time1: %4ld microseconds\n", i + 1, utime);
        sum += utime;
        utimes[i] = utime;
    }

    //counting occurrences of each utimes' value
    countingx(utimes, TIMES, occurrences, LENGTH, INTERVAL);
    //counting5(utimes, TIMES, occurrences, LENGTH);

    //printing results
    int ratio = (TIMES*INTERVAL)/(1350);
    //(TIMES*INTERVAL)/(LENGTH*500)
    if (ratio <= 0) ratio = 1;
    printfGraphicx(occurrences, LENGTH, ratio, INTERVAL);
    //printfGrafic5(occurrences, LENGTH, TIMES/3000);
    printf("\nFINAL AVERAGE: %.3f usecs\n", (double)sum/(double)TIMES);
    return 0; 
} 

