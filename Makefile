PWD=$(shell pwd)
CGI_BIN=$(PWD)/cgi_bin
SER_BIN=httpd
SER_SRC=httpd.c
CLI_SRC=client.c
CLI_BIN=client
FLAGS=-o
LDFLAGS=-lpthread -g -DEPOLL -DLOG -DDEBUG #-DTHREADPOLL  -DMULTITHREAD 
CC=gcc

.PHONY:all
all:$(SER_BIN) $(CLI_BIN) sql cgi 

$(SER_BIN):$(SER_SRC)
	$(CC) $(FLAGS) $@ $^ $(LDFLAGS)
$(CLI_BIN):$(CLI_SRC)
	$(CC) $(FLAGS) $@ $^ $(LDFLAGS)

.PHONY:cgi
cgi:
	cd cgi_bin;\
	for name in `ls -F | grep '/$$'`;\
	do\
		cd $$name;\
		make;\
		cd ..;\
	done

.PHONY:sql
sql:
	cd sql_connect;\
	make;

.PHONY:output
output:all
	cd sql_connect;\
	make output;\
	cd ..;\
	mkdir -p output/htdocs/cgi_bin
	#mkdir -p output/htdocs/include
	cp $(SER_BIN) output
	cp $(CLI_BIN) output
	cp autostart.sh output
	cp -rf conf output
	cp -rf log output
	cp -rf htdocs/* output/htdocs
	for name in `ls -F cgi_bin/|grep '/$$'`;\
	do\
		cd cgi_bin;\
		cd $$name;\
		make output;\
		cd ../../;\
	done;\


.PHONY:clean
clean:
	rm -rf $(SER_BIN) $(CLI_BIN) output
	for name in `ls -F cgi_bin/| grep '/$$'`;\
	do\
		cd cgi_bin/$$name;\
		make clean;\
		cd ../..;\
	done
	cd sql_connect;\
	make clean;\
	cd ..;\

.PHONY:git
git:clean
	git add *
	git commit -m "`date +%Y-%m-%d-%H-%M-%S`"
	git push -u origin master
