#include <stdio.h>

int main()
{
  //teste
  char texto[500], continua = 's';
  printf("Digite um texto de até 500 caracteres:\n-->");
  scanf("%[^\n]", texto);
  printf("Eis o texto em decimal:\n-->");
  for(int i = 0; texto[i] != 0; i++)
  {
    printf("%d ", texto[i]);
  }
  printf("\n");
  printf("Eis o texto em hexadecimal:\n-->");
  for(int i = 0; texto[i] != 0; i++)
  {
    printf("%x ", texto[i]);
  }
  printf("\n");
  printf("\nContinua? (s/n)\n");
  scanf("\n%c", &continua);
  while (continua == 's')
  {
    printf("\n");
    printf("Digite um texto de até 500 caracteres:\n-->");
    scanf("\n%[^\n]", texto);
    printf("Eis o texto em decimal:\n-->");
    for(int i = 0; texto[i] != 0; i++)
    {
      printf("%d ", texto[i]);
    }
    printf("\n");
    printf("Eis o texto em hexadecimal:\n-->");
    for(int i = 0; texto[i] != 0; i++)
    {
      printf("%x ", texto[i]);
    }
    printf("\n");
    printf("\nContinua? (s/n)\n");
    scanf("\n%c", &continua);
  }
  return 0;
} 