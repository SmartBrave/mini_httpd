LIB_PATH=/usr/lib64/mysql
LIB=mysqlclient
INCLUDE=../../sql_connect

.PHONY:all
all:show_offer login_offer save register add create_user

show_offer:show_offer.cpp
	#cd ../../sql_connect;\
	#make;\
	#cd ../cgi_bin/offer/;
	g++ -o $@ $^ -L$(LIB_PATH) -L../../sql_connect -l$(LIB) -lsql  -g

login_offer:login.cpp
	g++ -o $@ $^

save:save.cpp
	g++ -o $@ $^ -L$(LIB_PATH) -L../../sql_connect -l$(LIB) -lsql  -g

register:register.cpp
	g++ -o $@ $^ -L$(LIB_PATH) -L../../sql_connect -l$(LIB) -lsql  -g

add:add.cpp
	g++ -o $@ $^ -L$(LIB_PATH) -L../../sql_connect -l$(LIB) -lsql  -g

create_user:create_user.cpp
	g++ -o $@ $^

.PHONY:outpu
output:all
	cp show_offer ../../output/htdocs/cgi_bin
	cp login_offer ../../output/htdocs/cgi_bin
	cp save ../../output/htdocs/cgi_bin
	cp register ../../output/htdocs/cgi_bin
	cp add ../../output/htdocs/cgi_bin
	cp create_user ../../output/htdocs/cgi_bin

.PHONY:clean
clean:
	rm -rf show_offer login_offer save register add create_user
