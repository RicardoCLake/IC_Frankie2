#include "funcIC_v2.h"

//changes the shape of numbers with 32 bits

int main(int argc, char const *argv[])
{   
    int opcao = 0;
    unsigned int entrada = 0;

    printf("Qual o tipo de entrada?\n\t1-Binario\n\t2-Decimal\n\t3-Hexadecimal\nResposta: ");
    scanf("%d", &opcao);

    if (opcao == 1)
    {   
        char string[33] = {0};
        int casas = 0;
        printf("\n[Entrada binaria]\nDigite um numero de ate 32 bits (sem o 0B): ");
        scanf("%c", &string[0]); //limpando o lixo de '\n' q fica
        for (int i = 0; i < 32; i++)
        {   
            scanf("%c", &string[i]);
            if (string[i] != '1' && string[i] != '0') break;
            casas++;
        }
        string [casas] = 0;
        for (int i = 0; i < casas; i++)
        {   
            if(string[i] == '1')
            {
                entrada += PotInt(2, casas - i - 1);
            }
        }
    } 
    
    else if (opcao == 2)
    {
        printf("\n[Entrada decimal]\nDigite um numero de ate 32 bits: ");
        scanf("%d", &entrada);
    } 
    
    else if (opcao == 3)
    {  
        printf("\n[Entrada decimal]\nDigite um numero de ate 32 bits: ");
        scanf("%x", &entrada);
    } 

    else
    {
        printf("NAO ENTENDI!\n");
        return 0;
    }

    printf("\nQual o tipo de saida?\n\t1-Binario\n\t2-Decimal\n\t3-Hexadecimal\nResposta: ");
    scanf("%d", &opcao);

    if (opcao == 1)
    {   
        printf("\n[Saida binaria]\n0B");
        for (int i = 0; i < 32; i++)
        {   
            if(! (i%4) )
            {
                printf(" ");
            } 
            printf("%d", ( entrada/PotInt(2, 32 -i -1) ) % 2);
        }
        printf("\n");
    } 
    
    else if (opcao == 2)
    {
        printf("\n[Saida decimal]\n%d\n", entrada);
    } 
    
    else if (opcao == 3)
    {   
        printf("\n[Saida hexadecimal]\n%x\n", entrada);
    } 

    else
    {
        printf("NAO ENTENDI!\n");
        return 0;
    }
    
    return 0;
}
