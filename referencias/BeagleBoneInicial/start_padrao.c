#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

#include "gpio_map.h"


#define GPIO1_BITS 0b00000000000011111111000011111111
#define GPIO2_BITS 0b00000000000000011111111111111110

static long count = 0;
int padrao = 0;
unsigned long interval = 1;
unsigned int ele_inicial = 0;
 
#define INTERVAL 500000                    /* microseconds */
 

static void cycle(int eleInp);
void init(int fd, int argc, char *argv[]);

void signalhandler(int signo)
{
    /* Do something every INTERVAL microseconds */
    /* For example, write a capital A to stdout */
    cycle(ele_inicial);
    ele_inicial = (ele_inicial + 1) % 32;
    count++;
}
 
 
int main(int argc, char *argv[])
{
    struct sigaction action;
    struct itimerval timer;
    struct timespec ts, rem;
    struct timeval end, now;
    int fd = open("/dev/mem", O_RDWR);

    init(fd, argc, argv);

    /* Catch SIGALRM */
    action.sa_handler = signalhandler;
    sigemptyset (&action.sa_mask);
    action.sa_flags = 0;
    sigaction (SIGALRM, &action, NULL);

    /* Send SIGALRM every INTERVAL microseconds */
    timer.it_value.tv_sec = timer.it_interval.tv_sec = 5;
    timer.it_value.tv_usec = timer.it_interval.tv_usec = INTERVAL;
    setitimer(ITIMER_REAL, &timer, NULL);

    /* Define sleep parameters */ 
    ts.tv_sec = 200;
    ts.tv_nsec = 0;
    rem = ts;

    /* Calculate end time of program run */
    gettimeofday(&end, NULL);
    end.tv_sec += 200;
 
    while (rem.tv_sec || rem.tv_nsec) {
        /* Continue sleep, if woken up by our own signal */
        if (nanosleep(&ts, &rem) == -1 && errno == EINTR)
            ts = rem;
        else
            break;
        /* DURATION elapsed? */
        gettimeofday(&now, NULL);
        if (now.tv_sec >= end.tv_sec && now.tv_usec >= end.tv_usec)
            break;
    }
    close(fd);
    return 0;
}



volatile void *gpio_addr = NULL;

volatile unsigned int *gpio1_oe_addr = NULL;
volatile unsigned int *gpio1_setdataout_addr = NULL;
volatile unsigned int *gpio1_cleardataout_addr = NULL;

volatile unsigned int *gpio2_oe_addr = NULL;
volatile unsigned int *gpio2_setdataout_addr = NULL;
volatile unsigned int *gpio2_cleardataout_addr = NULL;

volatile unsigned int *gpio3_oe_addr = NULL;
volatile unsigned int *gpio3_setdataout_addr = NULL;
volatile unsigned int *gpio3_cleardataout_addr = NULL;

void init(int fd, int argc, char *argv[]) {
    
    unsigned int reg;

    printf("Mapping %X - %X (size: %X)\n", GPIO_START_ADDR, GPIO_END_ADDR, GPIO_SIZE);
    gpio_addr = mmap(0, GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_START_ADDR);
    if(gpio_addr == MAP_FAILED) {
        printf("Unable to map GPIO\n");
        exit(1);
    }

    gpio1_oe_addr = gpio_addr + GPIO1_OFFSET + GPIO_OE;
    gpio1_setdataout_addr = gpio_addr + GPIO1_OFFSET + GPIO_SETDATAOUT;
    gpio1_cleardataout_addr = gpio_addr + GPIO1_OFFSET + GPIO_CLEARDATAOUT;

    gpio2_oe_addr = gpio_addr + GPIO2_OFFSET + GPIO_OE;
    gpio2_setdataout_addr = gpio_addr + GPIO2_OFFSET + GPIO_SETDATAOUT;
    gpio2_cleardataout_addr = gpio_addr + GPIO2_OFFSET + GPIO_CLEARDATAOUT;

    gpio3_oe_addr = gpio_addr + GPIO3_OFFSET + GPIO_OE;
    gpio3_setdataout_addr = gpio_addr + GPIO3_OFFSET + GPIO_SETDATAOUT;
    gpio3_cleardataout_addr = gpio_addr + GPIO3_OFFSET + GPIO_CLEARDATAOUT;

    printf("GPIO mapped to %p\n", gpio_addr);

    reg = *gpio1_oe_addr;
    printf("GPIO1 configuration: %X\n", reg);
    reg = reg & (0xFFFFFFFF - (GPIO1_BITS + 0b11110000000000000000000000000000));
    *gpio1_oe_addr = reg;

    reg = *gpio2_oe_addr;
    printf("GPIO2 configuration: %X\n", reg);
    reg = reg & (0xFFFFFFFF - (GPIO2_BITS + 0b00000011110000000000000000000000));
    *gpio2_oe_addr = reg;

    reg = *gpio3_oe_addr;
    printf("GPIO3 configuration: %X\n", reg);
    reg = reg & (0xFFFFFFFF - (             0b00000000000000111100000000000000));
    *gpio3_oe_addr = reg;

}

/*
 Controle do Conjunto de Multiplexadores
 A placa contem 4 multiplexadores identificados por U4, U5, U6 e U7.
 Os multiplexadores U4 e U7 recebem o mesmo sinal de entrada. O mesmo ocorre para U5 e U6.
 As saídas dos multiplexadores U4 e U5 são direcionadas aos mesmos bornes de saída. O mesmo ocorre para U6 e U7.
 Sao utilizados 12 sinais (identificados de S00 a S11) da seguinte maneira:
 - Sinais S00 a S03 habilitam a saídas de um dos 4 multiplexadores. S00->U7, S01->U6, S02->U5 e S03->U4.
 - Os sinais S00 e S03 atuam sobre multiplexadores que recebem o mesmo sinal de entrada e, portanto,
   nao devem ser atuados simultaneamente. O mesmo ocorre para os sinais S01 e S02.
 - Os sinais S04 a S07 determinam para qual das 16 saídas dos multiplexadores U5 e U6 deve ser direcionada a respectiva entrada.
 - O mesmo ocorre para as saida S08 a S12 em relacao aos multiplexadores U4 e U7.

 Considerando a conexao das saidas dos multiplexadores U4 e U5 nos eletrodos 1 a 16 e os demais eletrodos em U6 e U7, e
 tomando as duas entradas por INPUT1 (U4 e U7) e INPUT2 (U5 e U6), e atribuindo o nome CODE1 ao valor decimal dado pelos
 sinais S04 (LSB) a S07 (MSB) e CODE2 ao valor dedimal dados pelos sinais S08 (LSB) a S12 (MSB), temos:
 
 Se S03==1 entao INPUT1 será direcionada para eletrodo CODE2;
 Se S00==1 entao INPUT1 será direcionada para eletrodo CODE2;
 Se S00==0 AND S03==0 entao INPUT1 não será direcionada para nenhuma saída;
 Acionamento simultaneo de S00 e S03 é uma condição inválida.

 Se S02==1 entao INPUT2 será direcionada para eletrodo CODE1;
 Se S01==1 entao INPUT2 será direcionada para eletrodo CODE1;
 Se S01==0 AND S02==0 entao INPUT2 não será direcionada para nenhuma saída;
 Acionamento simultaneo de S01 e S02 é uma condição inválida.

 A implementacao usa GPIO1.28-31 como CODE1, GPIO2.22-25 como CODE2, GPIO3.14-17 como S00-03

 
 */


static void cycle(int eleInp) {
   
    
    unsigned int eleOut = (eleInp + padrao + 1) % 32;

    printf("Turning all off\n");
    *gpio2_cleardataout_addr = GPIO2_BITS + 0b00000011110000000000000000000000;
    *gpio1_cleardataout_addr = GPIO1_BITS + 0b11110000000000000000000000000000;
    *gpio3_cleardataout_addr = 0b00000000000000111100000000000000;

    printf("Setting %d and %d\n", eleInp, eleOut);
    if (eleInp>15) {   
        *gpio1_setdataout_addr = ((eleInp>23)? (1 << (eleInp-12)) : (1 << (eleInp-16)));
        *gpio2_setdataout_addr = ((eleInp % 16) << 22);
        *gpio3_setdataout_addr = (1 << 17);
    }
    else {
        *gpio2_setdataout_addr = (1 << (eleInp+1)) + ((eleInp % 16) << 22);
        *gpio3_setdataout_addr = (1 << 14);
    }

    if (eleOut>15) {
        *gpio1_setdataout_addr = ((eleOut>23)? (1 << (eleOut-12)) : (1 << (eleOut-16))) + ((eleOut % 16) << 28);
        *gpio3_setdataout_addr = (1 << 16);
    }
    else {
        *gpio2_setdataout_addr = (1 << (eleOut+1));
        *gpio1_setdataout_addr = ((eleOut % 16) << 28);
        *gpio3_setdataout_addr = (1 << 15);
    }
    


}
