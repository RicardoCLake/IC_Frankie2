#include <stdio.h>
#include <sys/time.h>

#define MICRO_PER_SECOND 1000000 
// Os 1000000 e so porque quero que os milisegundos aparecam como decimal dos segundos
// o membro tv_use da struct timeval vem em microsegundos

void funcao();

int main(int argc,char* argv[])
{
    struct timeval start_time;
    struct timeval stop_time;
    float time_diff;

    gettimeofday( &start_time, NULL );
    funcao(); // qualquer coisa que queiram medir
    gettimeofday( &stop_time, NULL );

     time_diff = (float)(stop_time.tv_sec - start_time.tv_sec);
     time_diff += (stop_time.tv_usec - start_time.tv_usec)/(float)MICRO_PER_SECOND; 
     printf("funcao executada em %f s ou %f us\n",time_diff, time_diff*MICRO_PER_SECOND);
     printf("start %ld-%ld\nstop  %ld-%ld\n", start_time.tv_sec, start_time.tv_usec, stop_time.tv_sec, stop_time.tv_usec);

    return 0;
}

void funcao(){
    for (int i = 0; i<1234567; i++);
}