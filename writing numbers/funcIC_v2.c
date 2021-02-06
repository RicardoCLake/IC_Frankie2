#include "funcIC_v2.h"

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

