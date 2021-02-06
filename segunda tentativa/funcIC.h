#include <stdio.h>
#include <sys/socket.h>  //send()
//#include <netinet/in.h> 
#include <string.h> //strlen()
#include <unistd.h> //read() 
#include <time.h> //clock()
#include <sys/time.h> //gettimeofday()

#define MICRO_PER_SECOND 1000000

  //Delay clocks clocks
void delayClocks(int clocks);

  //Delay micros microseconds
void delayMicros(long micros);

int PotInt (int base, int expoente);

  //Count occurrences of each utimes' value. l1 = length of *utimes and l2 = length of *occurrences
void counting(int *utimes, int l1, int *occurrences, int l2);

  //{n, n+1, n+2, n+3, n+4} --> occurrence[n/5]++
void counting5(int *utimes, int l1, int *occurrences, int l2);

  //like couting5, but 5->x
void countingx (int *utimes, int l1, int *occurrences, int l2, int x);

  //Happened is the lenght of occurrences, and ratio is how many occurrences is iqual to a graphic's point
void printfGrafic(int *occurrences, int happened, int ratio);

  //The same as printGrafics(), but 5 by 5 in Ox
void printfGrafic5(int *occurrences, int happened, int ratio);

  //Like printfGrafic5, but 5 -> x
void printfGraphicx (int *occurrences, int happened, int ratio, int x);

  //returns an int from a string
int getNumber (char* buffer);

  //Simple slave in a ptp
long simpleSlave (int socket, int verboseMode);

  //Simple master in ptp
void simpleMaster (int socket, int verboseMode);
