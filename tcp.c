#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tcp.h"

int sock_rmf;
int sock_plc;
char sendbuf_plc[100];   //tcp buf for plc data
char rxbuf_plc[100];;
char sendbuf_rmf[100];   //tcp buf for sending to rmf system 
char rxbuf_rmf[100];
 

int tcp_init(int *p_sockid , char logfilename[])
{
 
    int port;
    struct sockaddr_in pin;
    struct hostent *nlp_host;
    char host_name[256];
   /*TCP Init*/
    FILE *logfile = fopen(logfilename,"r");       //the dir should has more detail   
	fscanf(logfile,"%s",host_name);             // read logfile for host_name and port
	printf("host_name: %s\n",host_name);	
//	strcpy( host_name , "192.168.1.106" ); 
	fscanf(logfile,"%d",&port);
	printf("port: %d\n",port);
	fclose(logfile);

    while ((nlp_host=gethostbyname(host_name))==0){
   		printf("Connecting to %s Error!\n", host_name);
	} // check the host_name
 
	bzero(&pin,sizeof(pin));
	pin.sin_family=AF_INET;                 
	pin.sin_addr.s_addr=htonl(INADDR_ANY);  
	pin.sin_addr.s_addr=((struct in_addr *)(nlp_host->h_addr))->s_addr;
	pin.sin_port=htons(port);
 
	*p_sockid = socket(AF_INET,SOCK_STREAM,0);
	printf("TCP Init Over\n"); 

    while (connect(*p_sockid,(struct sockaddr*)&pin,sizeof(pin))==-1){
  		printf("Connect Error!\n");
        return 0;
	}
	printf("Connected to %s \n", host_name);
    return 1;
}

int rmf_tcp_init()
{
    tcp_init(&sock_rmf , "tcp.log"); 
    printf("RMF_TCP_INIT OVER!\n");
    return 1;
}

int plc_tcp_init()
{
    tcp_init(&sock_plc , "plc.log"); 
    printf("PLC_TCP_INIT OVER!\n");
    return 1;
}

int get_tcp_data(int *p_sock ,char sendbuf[], char rxbuf_tcp[] )
{ 
	int i = 0;
    int rxlen = 0;
	strcpy(sendbuf,"Hello man!\n");
	send(*p_sock , sendbuf , 12 , 0);
	
	printf("Send Over , waiting for data\n");
//	memset(rxbuf , 0 ,strlen(rxbuf));
	rxlen = recv(*p_sock , rxbuf_tcp, 2 , 0 );	
	//printf("Receive: %s , len = %d \n",rxbuf,rxlen);
    return rxlen;
}

    
