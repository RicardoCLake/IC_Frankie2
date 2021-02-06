/*
 * PROTOCOLO SERVIDOR x BEAGLEBONE
 * STX + XX YY + CR
 * XX e YY sao numeros entre 01 e 32 indicando
 * os eletrodos de entrada e saida de corrente
 */

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

//#include <unistd.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h> 
//#include <string.h>
#include <sys/select.h>

/* According to earlier standards */
//#include <sys/time.h>
//#include <unistd.h>
 

#include "gpio_map.h"


#define GPIO1_BITS 0b00000000000011111111000011111111
#define GPIO2_BITS 0b00000000000000011111111111111110

#define INTERVAL 500                    /* microseconds */
#define TCP_PORT 8081

static void cycle(int eleInp, int eleOut);
void gpio_init(int fd);
 
int main(int argc, char *argv[])
{
    int sockfd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    time_t next_update = 0;
    unsigned char buffer[10], snd_buffer[10] = "OK00;";
    int n = 0;
	int e1, e2;

    /* SOCKET INITIALIZATION */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Forcefully attaching socket to the port 8080 
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( TCP_PORT ); 
       
    // Forcefully attaching socket to the port 22001
    if (bind(sockfd, (struct sockaddr *)&address, sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    struct timeval t;    
    t.tv_sec = 0;
    t.tv_usec = 10000;

    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const void *) &t, (socklen_t) sizeof(t));


    /* GPIO INITIALIZATION */
    int fd = open("/dev/mem", O_RDWR);
    gpio_init(fd);

    if (listen(sockfd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
    while (1) {
        
        while ((new_socket = accept(sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
        { 
            //printf("TCP Error: accept %d\n",  new_socket);
            if (new_socket != -1) {
                close(fd);
                exit(EXIT_FAILURE);
            }
            //printf("Aguardando conexão\n");
            //sleep(1);
        }
        while (1) {
            
            n = read( new_socket , buffer, 6);
			if ((n == 6) && (buffer[0] == 'C') && (buffer[5] == ';')) {
				e1 = (buffer[1] - 0x30) * 10 + (buffer[2] - 0x30);
				e2 = (buffer[3] - 0x30) * 10 + (buffer[4] - 0x30);
				if (e1 > 32 || e1 < 1) {
					e1 = 1;
				}
				if (e2 > 32 || e2 < 1 || e1 == e2) {
					e2 = (e1 + 1);
				}
				//printf(">>%02d <<%02d\n", e1, e2);
				cycle(e1 - 1, e2 - 1);
                snd_buffer[2] = buffer[1];
                snd_buffer[3] = buffer[2];
                send(new_socket, snd_buffer, 5, 0);
	            next_update = time(NULL) + 3;
	            n = 0;
			}
            if (time(NULL) > next_update) {
                printf("TCP Timeout\n");
                close(new_socket);
                break;
            }
        }
    }    
    /* TCP CLOSING */
    

    

    /* GPIO CLOSING */
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

void gpio_init(int fd) {
    
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


static void cycle(int eleInp, int eleOut) {
   
    //printf("Turning all off\n");
    *gpio2_cleardataout_addr = GPIO2_BITS + 0b00000011110000000000000000000000;
    *gpio1_cleardataout_addr = GPIO1_BITS + 0b11110000000000000000000000000000;
    *gpio3_cleardataout_addr = 0b00000000000000111100000000000000;

    //printf("Setting %d and %d\n", eleInp, eleOut);
    if (eleInp>15) {   
        *gpio1_setdataout_addr = ((eleInp>23)? (1 << (eleInp-12)) : (1 << (eleInp-16)));
        *gpio2_setdataout_addr = ((eleInp % 16) << 22);
        *gpio3_setdataout_addr = (1 << 14);
    }
    else {
        *gpio2_setdataout_addr = (1 << (eleInp+1)) + ((eleInp % 16) << 22);
        *gpio3_setdataout_addr = (1 << 17);
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
