.PHONY:all
all:libsql.a

libsql.a:sql.cpp
	g++ -c sql.cpp -L/usr/lib/mysql -lmysqlclient -g
	ar -crv libsql.a sql.o

.PHONY:output
output:all
	#cp libsql.a /usr/lib
	#cp sql.h /usr/include
	#cp libsql.a ../output/htdocs/cgi_bin
	#cp sql.h ../output/htdocs/include

.PHONY:clean
clean:
	rm -rf libsql.a sql.o
