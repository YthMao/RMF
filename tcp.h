#ifndef __TCP_H__
#define __TCP_H__
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//extern int port;
extern int sock_rmf; 
extern int sock_plc;
extern char sendbuf_plc[100];
extern char rxbuf_plc[100];
extern char sendbuf_rmf[100]; 
extern char rxbuf_rmf[100];



int tcp_init(int *p_sockid , char logfilename[]);
int get_tcp_data(int *p_sock ,char sendbuf[], char rxbuf_tcp[] );
int rmf_tcp_init();
int plc_tcp_init();

#endif
