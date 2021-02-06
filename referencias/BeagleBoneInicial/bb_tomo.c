#include "bb_tomo.h"

void bb_setPattern(int sock, int skip, int curr)
{
    char buffer[8];
	sprintf(buffer, "C%02d%02d;", curr, (curr+skip) % 32 + 1);
	send(sock, buffer , 6 , 0 ); 
	printf("Mensagem Enviada (C%02d%02d;)\n", curr, (curr+skip) % 32 + 1);
}

void bb_init(int sock)
{
    char bb_hostname[] = "beaglebone.local";
    char bb_ip[32];

    struct hostent *he;
	struct in_addr **addr_list;
	int x;

    struct sockaddr_in serv_addr; 

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("Socket creation error\n"); 
        return; 
    }
    printf("Socket created\n");

		
	if((he = gethostbyname(bb_hostname)) == NULL) 
	{
		printf("TCP Error: gethostbyname\n");
		return;   
	}
	addr_list = (struct in_addr **) he->h_addr_list;
	for(x = 0; addr_list[x] != NULL; x++) 
	{
		strcpy(bb_ip , inet_ntoa(*addr_list[x]));
	}
	printf("IP %s\n", bb_ip);
    printf("BeagleBone Found\n");
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, bb_ip, &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return; 
    } 
    printf("BeagleBone IP Found\n");

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return; 
    }
    printf("Socket Connected\n");
}
