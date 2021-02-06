#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NC 3000
/*mudar esse valor para regular a quantidade caracteres*/

int main()
{
  FILE *fp;
  char T[NC], fileName[200];
  //strcpy(fileName, "PegueiGuardei%d"NC);
  strcpy(fileName, "PegueiGuardei3000");
  if (!(fp = fopen(fileName, "a")))
  {
    //printf("Erro ao abrir o arquivo %s\n", fileName);
    printf("Erro ao abrir o arquivo %s\n", fileName);
    return 1;
  }
  printf("Qual o texto a ser guardado?\n");
  scanf("%[^\n]", T);
  for (int i = 0; i < NC && T[i] != 0; i++)
  {
    fprintf(fp, "%c", T[i]);
  }
  fprintf(fp, "\n");
  fclose(fp);
  return 0;
} 