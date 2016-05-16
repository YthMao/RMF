#sq_test:test.o
#	gcc test.o -o sq_test -lsqlite3

whole_test:main.o sql.o tcp.o uart.o data_process.o sql.h tcp.h uart.h data_process.h
	gcc -o whole_test main.o sql.o tcp.o uart.o data_process.o -lsqlite3

main.o:main.c sql.h tcp.h
	gcc -c main.c -o main.o -lsqlite3

sql.o:sql.c sql.h  sqlite3.h 
	gcc -c -o sql.o sql.c -lsqlite3

tcp.o:tcp.c tcp.h
	gcc -c -o tcp.o tcp.c 

uart.o:uart.c uart.h
	gcc -c -o uart.o uart.c

data_process.o:data_process.c data_process.h
	gcc -c -o data_process.o data_process.c

clean:
	rm -f *.o
