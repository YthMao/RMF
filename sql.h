#ifndef __SQLITE_H__
#define __SQLITE_H__
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


extern int sql_ID;
extern char sql_value[20];
extern char strSql[100];
extern sqlite3 *db;

int myfunc(void *p , int argc , char **argv, char **argvv);
int db_init(sqlite3 *db_main);
int db_close(sqlite3 *db_main);
int db_store(sqlite3 *db_main , int data_ID , char data[]);





#endif     
