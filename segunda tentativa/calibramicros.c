#include <stdio.h>
#include <time.h>
#define VEZES 2
#define DELAY 10

int main()
{
  long seg;
  int i;
  double soma = 0.0;

  for(int j = 0; j < VEZES; j++)
  { 
    time(&seg);
    for (i = 0; time(NULL) != seg + DELAY; i++);
    printf("%d segundos tiveram %d clocks\n",DELAY, i);
    soma += (double)i;
  }

  printf ("A media foi: %f\n", soma/(VEZES*DELAY));
  
  return 0;
}
