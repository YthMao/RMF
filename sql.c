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
#include "sql.h"

int sql_ID = 0;
char sql_value[20] ;
char strSql[100] ;

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

int db_init(sqlite3 *db_main)
{
    /*Database Init*/
    char *err = 0;
    int ret = 0, empty = 1;
    sqlite3 *db;

    ret = sqlite3_open("db_TCP.db",&db);
    if(ret != SQLITE_OK)
    {
        fputs("\n",stderr);
        return 0;
    }
    db_main = db;
    ret = sqlite3_exec(db , "drop table table_data;", myfunc , NULL , &err);
     if(ret != SQLITE_OK)
    {
        fputs("\n",stderr);
        return 0;
    }
    printf("Database delete Over\n");
    ret = sqlite3_exec(db , "create table table_data(dev_ID INTERGER , dev_data TEXT, time TimeStamp NOT NULL DEFAULT(datetime('now','localtime') ));\0", myfunc , NULL , &err);
     if(ret != SQLITE_OK)
    {
        fputs("\n",stderr);
        return 0;
    }   
    printf("Database Init Over\n");

    return 1;
}

int db_close(sqlite3 *db_main)
{
    sqlite3_close(db_main);
}

int db_store(sqlite3 *db_main , int data_ID , char data[])
{
    sqlite3 *db = db_main ;
    char *err = 0;
    int ret = 0;
    sql_ID = data_ID;
    strcpy(sql_value , data);
    sprintf(strSql , "insert into table_data(dev_ID, dev_data) values(%d ,'%s'  );\0", sql_ID , sql_value );
    ret = sqlite3_exec(db , strSql, myfunc , NULL , &err);
    if(ret != SQLITE_OK)
    {
        fputs("\n",stderr);
        return 0;
    }
    printf("insert OK !\n");   
    return 1;
}
