#include<stdio.h>

// ANSI-Cores
#define F_VERMELHO     "\033[41m\033[37m"
#define F_VERDE        "\033[42m\033[30m"
#define F_LARANJA      "\033[43m\033[30m"
#define F_AZUL         "\033[44m\033[37m"
#define F_ROSA         "\033[45m\033[37m"
#define F_CIANO        "\033[46m\033[30m"
#define F_BRANCO       "\033[47m\033[30m"
#define F_PRETO        "\033[40m\033[37m"

#define L_VERMELHO     "\033[31m\033[47m"
#define L_VERDE        "\033[32m\033[40m"
#define L_LARANJA      "\033[33m\033[40m"
#define L_AZUL         "\033[34m\033[47m"
#define L_ROSA         "\033[35m\033[47m"
#define L_CIANO        "\033[36m\033[40m"
#define L_BRANCO       "\033[37m\033[40m"
#define L_PRETO        "\033[30m\033[47m"

#define S_VERMELHO     "\033[31m"
#define S_VERDE        "\033[32m"
#define S_LARANJA      "\033[33m"
#define S_AZUL         "\033[34m"
#define S_ROSA         "\033[35m"
#define S_CIANO        "\033[36m"
#define S_BRANCO       "\033[37m"
#define S_PRETO        "\033[30m"

#define RESTAURA       "\033[00m"


// Funcoes de cores
void f_vermelho (char string[]) 
{printf("%s%s%s\n",F_VERMELHO,string,RESTAURA);}

void f_verde (char string[]) 
{printf("%s%s%s\n",F_VERDE   ,string,RESTAURA);}

void f_laranja (char string[])
{printf("%s%s%s\n",F_LARANJA ,string,RESTAURA);}

void f_azul(char string[]) 
{printf("%s%s%s\n",F_AZUL    ,string,RESTAURA);}

void f_rosa(char string[])
{ printf("%s%s%s\n",F_ROSA    ,string,RESTAURA);}

void f_ciano    (char string[]) 
{printf("%s%s%s\n",F_CIANO   ,string,RESTAURA);}

void f_branco (char string[])
{ printf("%s%s%s\n",F_BRANCO  ,string,RESTAURA);}

void f_preto (char string[])
{printf("%s%s%s\n",F_PRETO   ,string,RESTAURA);}

void l_vermelho (char string[]) 
{printf("%s%s%s\n",L_VERMELHO,string,RESTAURA);}

void l_verde (char string[]) 
{printf("%s%s%s\n",L_VERDE   ,string,RESTAURA);}

void l_laranja (char string[])
{printf("%s%s%s\n",L_LARANJA ,string,RESTAURA);}

void l_azul(char string[]) 
{printf("%s%s%s\n",L_AZUL    ,string,RESTAURA);}

void l_rosa(char string[])
{ printf("%s%s%s\n",L_ROSA    ,string,RESTAURA);}

void l_ciano    (char string[]) 
{printf("%s%s%s\n",L_CIANO   ,string,RESTAURA);}

void l_branco (char string[])
{ printf("%s%s%s\n",L_BRANCO  ,string,RESTAURA);}

void l_preto (char string[])
{printf("%s%s%s\n",L_PRETO   ,string,RESTAURA);}

void s_vermelho (char string[]) 
{printf("%s%s%s\n",S_VERMELHO,string,RESTAURA);}

void s_verde (char string[]) 
{printf("%s%s%s\n",S_VERDE   ,string,RESTAURA);}

void s_laranja (char string[])
{printf("%s%s%s\n",S_LARANJA ,string,RESTAURA);}

void s_azul(char string[]) 
{printf("%s%s%s\n",S_AZUL    ,string,RESTAURA);}

void s_rosa(char string[])
{ printf("%s%s%s\n",S_ROSA    ,string,RESTAURA);}

void s_ciano    (char string[]) 
{printf("%s%s%s\n",S_CIANO   ,string,RESTAURA);}

void s_branco (char string[])
{ printf("%s%s%s\n",S_BRANCO  ,string,RESTAURA);}

void s_preto (char string[])
{printf("%s%s%s\n",S_PRETO   ,string,RESTAURA);}

// Principal
int main()
{
   f_vermelho("Vermelho");
   f_verde   ("Verde");
   f_laranja ("Laranja");
   f_azul    ("Azul");
   f_rosa    ("ROSA");
   f_ciano   ("Ciano");
   f_branco  ("Branco");
   f_preto   ("Preto");
   printf("\n");
   l_vermelho("Vermelho");
   l_verde   ("Verde");
   l_laranja ("Laranja");
   l_azul    ("Azul");
   l_rosa    ("ROSA");
   l_ciano   ("Ciano");
   l_branco  ("Branco");
   l_preto   ("Preto");
   printf("\n");
   s_vermelho("Vermelho");
   s_verde   ("Verde");
   s_laranja ("Laranja");
   s_azul    ("Azul");
   s_rosa    ("ROSA");
   s_ciano   ("Ciano");
   s_branco  ("Branco");
   s_preto   ("Preto");
}