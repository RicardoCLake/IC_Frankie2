#include <stdio.h>
#include <time.h>
#define VEZES 50
#define DELAY 9

int main()
{
  time_t seg;
  clock_t clock_i, clock_f, clock_dif;
  double soma = 0.0;

  for(int j = 0; j < VEZES; j++)
  { 
    seg = time(NULL) + 1;
    while(seg != time(NULL));
    clock_i = clock();
    while(time(NULL) != seg + DELAY);
    clock_f = clock();
    printf("%d segundos tiveram %ld clocks\n", DELAY, clock_dif = clock_f - clock_i);
    soma += (double) clock_dif;
  }

  printf ("A media foi: %f clocks por seg\n", soma/(VEZES*DELAY));
  
  return 0;
}
