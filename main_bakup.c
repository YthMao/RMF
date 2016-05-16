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

#define Sqlite3_test
//#define Tcp_test
struct sockaddr_in pin;
struct hostent *nlp_host;
int port;
int sd; 
char host_name[256];
char sendbuf[100];
char rxbuf[100];
// read from the tcp.log
int myfunc(void *p , int argc , char **argv, char **argvv)
{
    int i;
    *(int*)p = 0;
    for(i = 0 ; i < argc ; i++)
    {
        printf("%s = %s ",argvv[i],argv[i]?argv[i]:"NULL");
    }
    putchar('\n');
    return 0;
}
int main(void)
{ 
    int i = 0;
    int rxlen = 0;

    /*Time Init*/
    time_t   now;         //实例化time_t结构
    struct   tm     *timenow;         //实例化tm结构指针
    time(&now);
    timenow = localtime(&now);
    printf("Local   time   is   %s\n",asctime(timenow));

    
#ifdef Sqlite3_test
    /*Database Init*/
    sqlite3 *db;
    char *err = 0;
    int ret = 0, empty = 1;
    char strSql[100] ;
    int sql_ID = 0;
    int sql_value = 0;

    ret = sqlite3_open("db_TCP.db",&db);
    if(ret != SQLITE_OK)
    {
        fputs("\n",stderr);
        exit(1);
    }
    ret = sqlite3_exec(db , "drop table table_data;", myfunc , NULL , &err);
     if(ret != SQLITE_OK)
    {
        fputs("\n",stderr);
        exit(1);
    }
    printf("Database delete Over\n");
    ret = sqlite3_exec(db , "create table table_data(dev_ID INTERGER , degree_data INTERGER, time TimeStamp NOT NULL DEFAULT(datetime('now','localtime') ));\0", myfunc , NULL , &err);
     if(ret != SQLITE_OK)
    {
        fputs("\n",stderr);
        exit(1);
    }   
    printf("Database Init Over\n");
 #endif

#ifdef Tcp_test
    /*TCP Init*/
    FILE *logfile = fopen("tcp.log","r");
    fscanf(logfile,"%s",host_name);// read logfile for host_name and port
    printf("host_name: %s\n",host_name);	
    //	strcpy( host_name , "192.168.1.106" ); 
    fscanf(logfile,"%d",&port);
    printf("port: %d\n",port);
    while ((nlp_host=gethostbyname(host_name))==0){
        printf("Connecting to %s Error!\n", host_name);
    } // check the host_name
    bzero(&pin,sizeof(pin));
    pin.sin_family=AF_INET;                 
    pin.sin_addr.s_addr=htonl(INADDR_ANY);  
    pin.sin_addr.s_addr=((struct in_addr *)(nlp_host->h_addr))->s_addr;
    pin.sin_port=htons(port);
    sd=socket(AF_INET,SOCK_STREAM,0);
    printf("TCP Init Over\n"); 
    /*start main*/
    while (connect(sd,(struct sockaddr*)&pin,sizeof(pin))==-1){
        printf("Connect Error!\n");
    }
    printf("Coneected to %s \n", host_name);
    for(i=0 ; i<3 ; i++){
        strcpy(sendbuf,"Hello man!\n");
        send(sd , sendbuf , 12 , 0);
        printf("Send Over , waiting for data\n");
        memset(rxbuf , 0 ,strlen(rxbuf));
        rxlen = recv(sd , rxbuf, 10 , 0 );	
        printf("Receive: %s , len = %d \n",rxbuf,rxlen);
        sleep(1);
    }	 
    #endif
    for(i=0 ; i<3 ; i++){

        sql_ID = i + 100;
        sql_value = 90;
        sprintf(strSql , "insert into table_data(dev_ID, degree_data) values(%d , %d  );\0", sql_ID , sql_value );

//        strcpy( strSql , "insert into table_data values(0002 , 200 , '2013-05-05');\0" );
        ret = sqlite3_exec(db , strSql, myfunc , NULL , &err);
        printf("insert OK !\n");
        sleep(1);
    }
// 		ret = sqlite3_exec(db , "select * from user;" , myfunc , &empty , &err);
//
//    	if(ret != SQLITE_OK)
//    	{	
//    		fputs(err,stderr);
//    		fputs("\n",stderr);
//    		sqlite3_close(db);
//    	//	exit(1);
//    	}
//
//	    if(empty)
//    	{
//    		fputs("table test is empyt\n",stderr);
//    	//	exit(1);
//    	}
sqlite3_close(db);
close(sd);	
//	fclose(logfile);
printf("Goodbye!\n");
return 0 ;
}
