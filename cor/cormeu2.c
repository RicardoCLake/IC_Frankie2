#include<stdio.h>

//ANSI-Formatacao
#define NEGRITO        "\033[01m"
#define APAGADO        "\033[02m"
#define ITALICO        "\033[03m"
#define SUBLINHADO     "\033[04m"
#define PISCANTE       "\033[05m"
#define SEILA1            //06 não me surtiu efeito
#define MARCA_TEXTO    "\033[07m"
#define SEILA2           //08 sumiu o texto para mim
#define CORTADO        "\033[09m"

// ANSI-Cores-Fundo
#define F_VERMELHO     "\033[41m"
#define F_VERDE        "\033[42m"
#define F_LARANJA      "\033[43m"
#define F_AZUL         "\033[44m"
#define F_ROSA         "\033[45m"
#define F_CIANO        "\033[46m"
#define F_BRANCO       "\033[47m"
#define F_PRETO        "\033[40m"

//ANSI-Cores-Letra-Escuro
#define L_VERMELHO     "\033[31m"
#define L_VERDE        "\033[32m"
#define L_LARANJA      "\033[33m"
#define L_AZUL         "\033[34m"
#define L_ROSA         "\033[35m"
#define L_CIANO        "\033[36m"
#define L_BRANCO       "\033[37m"
#define L_PRETO        "\033[30m"

//ANSI-Cores-Letra-Claro
#define L_VERMELHO     "\033[91m"
#define L_VERDE        "\033[92m"
#define L_LARANJA      "\033[93m"
#define L_AZUL         "\033[94m"
#define L_ROSA         "\033[95m"
#define L_CIANO        "\033[96m"
#define L_BRANCO       "\033[97m"
#define L_PRETO        "\033[90m"

//ANSI-Padrao
#define RESTAURA       "\033[00m"


#define INICIO 0
#define LIMITE 99
int main (int argc, char *argv[])
{
  for(int i = INICIO; i < 10; i++)
  {
    printf("0%d --> \033[0%dmEssa é uma frase teste!%s\n", i, i, RESTAURA);
  }
  for(int i = 10; i <= LIMITE; i++)
  {
    printf("%d --> \033[%dmEssa é uma frase teste!%s\n", i, i, RESTAURA);
  }
}