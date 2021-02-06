#include "funcIC.h"

void delayClocks(int clocks)
{
    int clock0;
    clock0 = clock() + clocks;
    while (clock() < clock0);
}

void delayMicros(long micros)
{
    struct timeval time0;
    struct timeval time1;
    long micros0, micros1; 

    gettimeofday(&time0, NULL);
    gettimeofday(&time1, NULL);
    micros0 = time0.tv_sec * MICRO_PER_SECOND + time0.tv_usec;
    micros1 = time1.tv_sec * MICRO_PER_SECOND + time1.tv_usec;

    while (micros1 - micros0 < micros) 
    {
        gettimeofday(&time1, NULL);
        micros1 = time1.tv_sec * MICRO_PER_SECOND + time1.tv_usec;
    }
}

int PotInt (int base, int expoente)
{
  int i, pot = 1;
  for (i = 0; i < expoente; i++)
  {
    pot *= base;
  }
  return pot;
}

void counting(int *utimes, int l1, int *occurrences, int l2)
{
    printf("Processing data...\n");
    for (int i = 0; i < l2; i++) occurrences[i] = 0;
    for (int i = 0; i < l1; i++)
    {
        if       (utimes[i] <= 0)      occurrences[0]++;
        else if  (utimes[i] >= l2 -1)  occurrences[l2 - 1]++;
        else                           occurrences[utimes[i]]++; 
    }
    printf("Counting done!\n");
}

void counting5(int *utimes, int l1, int *occurrences, int l2)
{
    printf("Processing data...\n");
    for (int i = 0; i < l2; i++) occurrences[i] = 0;
    for (int i = 0; i < l1; i++)
    {
        if       (utimes[i] <= 0)            occurrences[0]++;
        else if  (utimes[i] >= (l2 -1) * 5)  occurrences[l2 - 1]++;
        else                                 occurrences[utimes[i]/5]++; 
    }
    printf("Counting done!\n");
}

void countingx (int *utimes, int l1, int *occurrences, int l2, int x)
{
    printf("Processing data...\n");
    for (int i = 0; i < l2; i++) occurrences[i] = 0;
    for (int i = 0; i < l1; i++)
    {
        if       (utimes[i] <= 0)            occurrences[0]++;
        else if  (utimes[i] >= (l2 -1) * x)  occurrences[l2 - 1]++;
        else                                 occurrences[utimes[i]/x]++; 
    }
    printf("Counting done!\n");
}

void printfGrafic (int *occurrences, int happened, int ratio)
{   
    printf("############## EIS O GRAFICO ################\n");
    for (int i = 0; i < happened; i++)
    {
        printf("%4d|", i);
        for (int j = 0; j < occurrences[i]; j += ratio)
        {
            printf("-");
        }
        printf("\n");
    }
}

void printfGrafic5 (int *occurrences, int happened, int ratio)
{   
    printf("############## EIS O GRAFICO ################\n");
    for (int i = 0; i < happened; i++)
    {
        printf("%4d|", i * 5);
        for (int j = 0; j < occurrences[i]; j += ratio)
        {
            printf("-");
        }
        printf("\n");
    }
}

void printfGraphicx (int *occurrences, int happened, int ratio, int x)
{   
    printf("############## EIS O GRAFICO ################\n");
    for (int i = 0; i < happened; i++)
    {
        printf("%4d|", i * x);
        for (int j = 0; j < occurrences[i]; j += ratio)
        {
            printf("-");
        }
        printf("\n");
    }
}

int getNumber (char* buffer) 
{
    int end_index, valRead;
    for (end_index = 0; buffer[end_index] != 0; end_index++);

    for (int i = 0; i < end_index; i++)
    {
        valRead += PotInt(10, end_index - i - 1) * (buffer[i] - '0');
    }
    return valRead;
}

long simpleSlave (int socket, int verboseMode)
{
    //Initial Declarations
    char buffer[32];

    //Syncronization msg
    struct timeval time1;
    long sMicros1;

    if (verboseMode) 
    {
        printf ("Waiting for Sync msg...\n");
    }
    read (socket, buffer, strlen(buffer));
    gettimeofday(&time1, NULL);
    sMicros1 = time1.tv_sec * MICRO_PER_SECOND + time1.tv_usec;
    if (verboseMode) 
    {
        printf ("sMicros1 = %ld\n", sMicros1);
    }
    
    //Following up msg
    long mMicros1;

    if (verboseMode) 
    {
        printf ("Waiting for follow up msg...\n");
    }
    read (socket, buffer, strlen(buffer));
    mMicros1 = getNumber(buffer);
    if (verboseMode) 
    {
        printf ("mMicros1 = %ld\n", mMicros1);
    }

    //Delay request 
    struct timeval time2;
    long sMicros2;

    delayMicros(300);
    send (socket, "oi", strlen("oi"), 0);
    gettimeofday(&time2, NULL);
    sMicros2 = time2.tv_sec * MICRO_PER_SECOND + time2.tv_usec;
    if (verboseMode) 
    {
        printf ("Sent delay request msg!\n");
        printf ("sMicros2 = %ld\n", sMicros2);
    }
     
    //Delay response
    long mMicros2;

    if (verboseMode) 
    {
        printf ("Waiting for delay response msg...\n");
    }
    read (socket, buffer, strlen(buffer));
    mMicros2 = getNumber (buffer);
    if (verboseMode) 
    {
        printf ("mMicros2 = %ld\n", mMicros2);
    }

    //Final operation
    return (mMicros1 - sMicros1 + mMicros2 - sMicros1) / 2;
}

void simpleMaster (int socket, int verboseMode)
{
    //Initial Declarations
    char buffer[32];

    //Syncronization msg
    delayMicros(1000); //wait for server preparation in read()
    struct timeval time1;
    long micros1;

    send(socket, "syn", strlen("syn"), 0);
    gettimeofday(&time1, NULL);
    micros1 = time1.tv_sec * MICRO_PER_SECOND + time1.tv_usec;
    if (verboseMode) 
    {   
        printf ("Sent sync msg...\n");
        printf ("micros1 = %ld\n", micros1);
    }

    //Following up msg
    delayMicros(300);
    sprintf(buffer, "%ld", micros1);
    send(socket, buffer, strlen(buffer), 0);
    if (verboseMode) 
    {
        printf ("Sent follow up msg...\n");
    }

    //Delay request 
    struct timeval time2;
    long micros2;

    if (verboseMode) 
    {
        printf ("Waiting for delay request msg...\n");
    }
    read(socket, buffer, strlen(buffer));
    gettimeofday(&time2, NULL);
    micros2 = time2.tv_sec * MICRO_PER_SECOND + time2.tv_usec;
    if (verboseMode) 
    {
        printf ("micros1 = %ld\n", micros1);
    }

    //Delay response
    delayMicros(100);
    sprintf(buffer, "%ld", micros2);
    send(socket, buffer, strlen(buffer), 0);
    if (verboseMode) 
    {
        printf ("Sent delay response msg...\n");
    }
}




