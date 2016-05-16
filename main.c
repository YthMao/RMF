#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h>
#include <pthread.h>
#include "sql.h"
#include "tcp.h"
#include "uart.h"
#include "data_process.h"

#define PLC_test
#define Uart_test
#define Can_test
#define Data_test
#define Rmfsend_test
 
pthread_t thread[5];
pthread_mutex_t mut;
int number=0, i;
int i = 0;
int rxlen = 0;
int fd_uart;
unsigned char data_packed[20];
char data_raw[20];
char rxbuf_can[2] ; // just a test
sqlite3 *db;        //for the main sql

void *thread1()
{
    int k = 0;
    printf ("thread1 : I'm thread 1, TCP\n");
    sleep(2);
    while(1){
        //for(i=0 ; i<5 ; i++){
        printf("T1 :");
        rxlen = get_tcp_data(&sock_plc , sendbuf_plc , rxbuf_plc); 
        printf("T1 :Receive: %s , len = %d \n",rxbuf_plc,rxlen);
        sleep(3);
    }	 
    pthread_exit(NULL);

}

void *thread2()
{
    int k = 0;
    int len_uart;
    printf("thread2 : I'm thread 2, UART \n");
    sleep(2);
    //for (k = 0 ; k < 6 ; k++)
    while(1)
    {
        strcpy(sendbuf_uart, "Give me Uart\r\n");
        len_uart = UART0_Send(fd_uart,sendbuf_uart,20);
        if(len_uart > 0)
            printf("T2 :send data successful\n");
        else
            printf("T2 :send data failed!\n");
        
        //strcpy(rxbuf_uart , "bb");
        //printf("T2 :Receive Uart Data : %s  , len = %d \n",rxbuf_uart, len_uart);
        len_uart = UART0_Recv(fd_uart , rxbuf_uart , 2);
        if(len_uart > 0){
            rxbuf_uart[len_uart] = '\0';
            printf("T2 :Receive Uart Data : %s  , len = %d \n",rxbuf_uart, len_uart);
        } 
        else 
            printf("Uart data receive fail!");
        //tcflush(fd_uart , TCIFLUSH);
        sleep(2);
    }
    pthread_exit(NULL);

}
void *thread3()
{
    printf("thread3 : I'm thread 3, Test\n");
    sleep(2);
    strcpy(rxbuf_can , "cc");
    for (i = 0; i < 6; i++)
    {
        printf("T3 :test number = %d\n",number);
       // pthread_mutex_lock(&mut);
        number++;
       // pthread_mutex_unlock(&mut);
        sleep(3);
    }
    pthread_exit(NULL);
}
void *thread4()
{
    int k,j,len_data = 0;
    printf("thread4 : I'm thread 4, Pack and Store!!\n");
    sleep(2);
    //for( k=0 ; k<10 ; k++  ){
    while(1){
        pack_data(data_packed , rxbuf_plc , rxbuf_uart ,rxbuf_can , 3);
        db_store(db , 2 ,(char *)data_packed);// need to unpack and store dispersedly
        printf("T4 : Pack and store data:  ");
        
        len_data = strlen(data_packed);
        for(j=0;j<10;j++){
            printf("%02X ",data_packed[j]);
        }
        printf("\n");
        sleep(3);
    }
    
    pthread_exit(NULL);
}
void *thread5()
{
    printf("thread5 : I'm thread 5, Send data to the remote system!!\n");
    sleep(2);
    while(1){
        printf("T5 : Sending To Remote: \n");
	    strcpy(sendbuf_rmf,"Hello RMF!\n");
        send(sock_rmf , sendbuf_rmf , sizeof(sendbuf_rmf) ,0);

        sleep(3);
    }
    pthread_exit(NULL);
}

void thread_create(void)
{
    int temp;
    memset(&thread, 0, sizeof(thread)); //comment1
    //创建线程G
#ifdef PLC_TCP
    if((temp = pthread_create(&thread[0], NULL, thread1, NULL)) != 0) //comment2
        printf("线程1创建失败!\n");
    else
        printf("线程1被创建\n");
#endif
#ifdef Uart_test
    if((temp = pthread_create(&thread[1], NULL, thread2, NULL)) != 0) //comment3
        printf("线程2创建失败\n");
    else
        printf("线程2被创建\n");
#endif
#ifdef Can_test
    if((temp = pthread_create(&thread[2], NULL, thread3, NULL)) != 0) //comment3
        printf("线程3创建失败\n");
    else
        printf("线程3被创建\n");
#endif
#ifdef Data_test
    if((temp = pthread_create(&thread[3], NULL, thread4, NULL)) != 0) //comment3
        printf("线程4创建失败\n");
    else
        printf("线程4被创建\n");
#endif
#ifdef Rmfsend_test
    if((temp = pthread_create(&thread[4], NULL, thread5, NULL)) != 0) //comment3
        printf("线程5创建失败\n");
    else
        printf("线程5被创建\n");
#endif
}

void thread_wait(void)
{
    //等待线程结束
    if(thread[0] !=0) { //comment4
        pthread_join(thread[0],NULL);
        printf("线程1已经结束\n");
    }
    if(thread[1] !=0) { //comment5
        pthread_join(thread[1],NULL);
        printf("线程2已经结束\n");
    }
    if(thread[2] !=0) { //comment5
        pthread_join(thread[2],NULL);
        printf("线程3已经结束\n");
    }
    if(thread[3] !=0) { //comment5
        pthread_join(thread[3],NULL);
        printf("线程4已经结束\n");
    }
    if(thread[4] !=0) { //comment5
        pthread_join(thread[4],NULL);
        printf("线程5已经结束\n");
    }
   
}

int main(void)
{ 
    int err;  
    /*Time Init*/
    time_t   now;         //实例化time_t结构
    struct   tm     *timenow;         //实例化tm结构指针
    time(&now);
    timenow = localtime(&now);
    printf("Local   time   is   %s\n",asctime(timenow));

    /*Database Init*/
    db_init(db);
    
#ifdef PLC_test
    /*PLC_TCP Init*/
    plc_tcp_init();
#endif

#ifdef Rmfsend_test
    /*TCP Init*/
    rmf_tcp_init(); 
    send(sock_rmf , "Hello rmf! I'm ready to send data!\n" ,12 ,0);
#endif

#ifdef Uart_test
    /*UART Init*/
    //fd_uart = UART0_Open(fd_uart,"/dev/ttySAC0"); //打开串口，返回文件描述符
    fd_uart = UART0_Open(fd_uart,"/dev/ttyUSB0"); //打开串口，返回文件描述符
    do{
        err = UART0_Init(fd_uart,9600,0,8,1,'N');
        printf("Set Port Exactly!\n");
    }while(FALSE == err || FALSE == fd_uart);
#endif
    /*Start Thread!*/
    pthread_mutex_init(&mut,NULL);
    printf("Start Threads!\n");
    thread_create();
    thread_wait();
    
    
    /*Close All */ 
    db_close(db);
    close(sock_rmf);	
    close(sock_plc);
#ifdef Uart_test
    UART0_Close(fd_uart);
#endif
    printf("Goodbye!\n");
    return 0 ;
}
