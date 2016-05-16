#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "data_process.h"

int pack_data(unsigned char data_after[] , char data1[] , char data2[] , char data3[] , int num_data)
{
    /*make up a data pack!*/
    memset(data_after , 0 ,strlen(data_after));
    data_after[0] = PackHead1 ;      //PackHead  2 bytes
    data_after[1] = PackHead2 ; 
    data_after[2] = num_data>>8;     //num of the datas 2 bytes
    data_after[3] = num_data&0xff;  
    data_after[4] = data1[0];        //each data takes 2 bytes
    data_after[5] = data1[1];
    data_after[6] = data2[0];
    data_after[7] = data2[1];
    data_after[8] = data3[0];
    data_after[9] = data3[1];
    data_after[10]= '\0';            // end of a pack without check
    return 1;
}

int unpack_data(char data_before[] , char data_split[] , int num_split)
{;}


