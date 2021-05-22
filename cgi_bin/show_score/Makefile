LIB_PATH=/usr/lib/mysql
LIB=mysqlclient
INCLUDE=../../sql_connect

.PHONY:all
all:show_score
show_score:show_score.cpp
	#cd ../../sql_connect;\
	#make;\
	#cd ../cgi_bin/show_score/;
	g++ -o $@ $^ -L$(LIB_PATH) -L../../sql_connect -l$(LIB) -lsql  -g

.PHONY:output
output:all
	cp show_score ../../output/htdocs/cgi_bin

.PHONY:clean
clean:
	rm -rf show_score
