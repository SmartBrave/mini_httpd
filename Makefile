PWD=$(shell pwd)
CGI_PATH=$(PWD)/cgi_bin
SER_BIN=httpd
CLI_BIN=client
SER_SRC=httpd.c
CLI_SRC=client.c
INCLUDE=.
CC=gcc
FLAGS=-o
LDFLAGS=-lpthread#-static
LIB=

.PHONY:all
all:$(SER_BIN) $(CLI_BIN) cgi

$(SER_BIN):$(SER_SRC)
	$(CC) $(FLAGS) $@ $^ $(LDFLAGS) -D_DEBUG_ -g
$(CLI_BIN):$(CLI_SRC)
	$(CC) $(FLAGS) $@ $^ $(LDFLAGS) -D_DEBUG_ -g

.PHONY:cgi
cgi:
	cd cgi_bin;\
	for name in `ls -F | grep '/$$'`;\
	do\
		cd $$name;\
		make;\
		cd ..;\
	done

.PHONY:output
output: all
	mkdir -p output/htdocs/cgi_bin
	cp httpd output
	cp client output
	cp -rf conf output
	cp -rf log output
	cp -rf htdocs/* output/htdocs
	for name in `ls -F cgi_bin/| grep '/$$'`;\
	do\
		echo `pwd`;\
		echo $$name;\
		cd cgi_bin;\
		cd $$name;\
		make output;\
		cd ../../;\
	done;\
	cd sql_connect;\
	make output;\
	cd ..;\

.PHONY:clean
clean:
	rm -rf $(SER_BIN) $(CLI_BIN) output
	cd cgi_bin;\
	for name in `ls -F | grep '/$$'`;\
	do\
		cd $$name;\
		make clean;\
		cd ..;\
	done
	cd sql_connect;\
	make clean;\
	cd ..;\


.PHONY:git
git:clean
	git add *
	git commit -m "`date +%y/%m/%d/%R`" 
	git push -u origin master
