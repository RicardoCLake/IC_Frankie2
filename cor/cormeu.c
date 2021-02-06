#include<stdio.h>

// ANSI-Cores-Fundo
#define F_VERMELHO     "\033[41m"
#define F_VERDE        "\033[42m"
#define F_LARANJA      "\033[43m"
#define F_AZUL         "\033[44m"
#define F_ROSA         "\033[45m"
#define F_CIANO        "\033[46m"
#define F_BRANCO       "\033[47m"
#define F_PRETO        "\033[40m"

//ANSI-Cores-Letra
#define L_VERMELHO     "\033[31m"
#define L_VERDE        "\033[32m"
#define L_LARANJA      "\033[33m"
#define L_AZUL         "\033[34m"
#define L_ROSA         "\033[35m"
#define L_CIANO        "\033[36m"
#define L_BRANCO       "\033[37m"
#define L_PRETO        "\033[30m"

//ANSDI-Formatacao
#define NEGRITO        "\033[01m"
#define APAGADO        "\033[02m"
#define ITALICO        "\033[03m"
#define SUBLINHADO     "\033[04m"
#define PISCANTE       "\033[05m"
//06 não me surtiu efeito
#define MARCA_TEXTO    "\033[07m"
//08 sumiu o texto para mim
#define CORTADO        "\033[09m"

//ANSI-Outros
#define DUPLO_SUBLINHADO "\033[21m"
#define SOBRE_SUBLINHADO "\033[53m"

//ANSI-Padrao
#define RESTAURA       "\033[00m"

int main (int argc, char *argv[])
{
  int r1, r2, r3, k;
  char rc;

  printf("Opcoes de local: Fundo, Letra\n                  [4]    [3]\nOpcoes de cores: Preto, Vermelho, Verde, Laranja, Azul, Rosa, Ciano e Branco\n                  [0]     [1]      [2]     [3]    [4]    [5]   [6]     [0]\nFaca sua combinacao tipo 42 (fundo verde)\n");

  printf("ESCOLHA 1: ");
  scanf("%d",&r1);
  do 
  {
    printf("Voce deseja fazer mais alguma escolha? (s/n): ");
    scanf("\n%c", &rc);
  } while (rc != 's' && rc != 'n');
  if(rc == 's')
  {
    printf("ESCOLHA 2: ");
    scanf("\n%d",&r2);
    do 
    {
      printf("Voce deseja fazer mais alguma escolha? (s/n): ");
      scanf("%c", &rc);
    } while (rc != 's' && rc != 'n');
    if(rc == 's')
    {
      printf("ESCOLHA 3: ");
      scanf("%d",&r3);
      k = 3; 
    }
    else
    {
      k = 2;
    }
  }
  else
  {
    k = 1;
  }
  
  if(k == 1)
  {
    printf("\n\033[%dmEssa é uma frase teste para ver como fica a cor!%s\n", r1, RESTAURA);
  }
  if(k == 2)
  {
    printf("\n\033[%dm\033[%dmEssa é uma frase teste para ver como fica a cor!%s\n", r1, r2, RESTAURA);
  }
  if(k == 3)
  {
    printf("\n\033[%dm\033[%dm\033[%dmEssa é uma frase teste para ver como fica a cor!%s\n", r1, r2, r3, RESTAURA);
  }
  return 0;
}