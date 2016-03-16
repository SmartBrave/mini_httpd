LIB_PATH=/usr/lib/mysql
LIB=mysqlclient
.PHONY:all
all:register

register:register.cpp
	cd ../../sql_connect;\
	make;\
	cd ../cgi_bin/register/;\
	g++ -o $@ $^ -L$(LIB_PATH) -l$(LIB) -D_DEBUG_ -g

.PHONY:output
output:all
	cp register ../../output/htdocs/cgi_bin

.PHONY:clean
clean:
	rm -rf register
