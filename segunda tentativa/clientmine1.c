//Use with servermine1
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <time.h>
#define PORT 7000 
//#define IPS "192.168.6.2"
#define IPS "169.254.230.51"
#define VEZES 1000000
#include "funcIC.h"

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

    clock_t clock1, clock2, clock3;
    char buffer[1024] = {0};
    unsigned long soma = 0;
    int vezes = VEZES, tempo;  
    int tempos[VEZES];        //deve ter o tamanho de "vezes"
    int ocorrencias[300];
    char svezes[13];
    
    sprintf(svezes, "%d", vezes);
    send(sock , svezes , 13 , 0 );
    delayClocks(1000);
    for (int i = 0; i < vezes; i++)
    {   
        clock1 = clock();
        send(sock , "hellohello" , strlen("hellohello") , 0 ); 
        clock2 = clock();
        read (sock , buffer, 1024);
        clock3 = clock(); 
        //printf("buffer: %s\n", buffer); 
        //printf("valread: %d\n", valread);
        //printf("clock1: %ld\nclock2: %ld\nclock3: %ld\n", clock1, clock2, clock3);
        tempo = clock3 - clock2;
        printf("%d - clock3 - clock2: %ld\n", i + 1, tempo);
        soma += tempo;
        tempos[i] = tempo;
    }
    printf("Processando dados...\n");
    for (int i = 0; i < 300; i++)
    {
        ocorrencias[i] = 0;
    }
    for (int i = 0; i < vezes; i++)
    {
        if (tempos[i] <= 0)
        {
            ocorrencias[0]++;
        }
        else if (tempos[i] >= 299)
        {
            ocorrencias[299]++;
        }
        else
        {
            ocorrencias[tempos[i]]++;
        }   
    }
    printf("Feita a contagem!\n");
    printfGrafic(ocorrencias, 300, 1200);
    printf("\nMEDIA FINAL: %.3f clocks\n", (double)soma/(double)vezes);
    return 0; 
} 
