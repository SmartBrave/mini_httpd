LIB_PATH=/usr/lib/mysql
LIB=mysqlclient

.PHONY:all
all:show_score

show_score:show_score.cpp
	g++ -o $@ $^ -L$(LIB_PATH) -l$(LIB) -g

.PHONY:output
output:all
	cp show_score ../output/htdocs/cgi_bin 

.PHONY:clean
clean:
	rm -rf show_score
