.PHONY:all
all:add

add:add.cpp
	g++ -o $@ $^ -g

.PHONY:output
output:all
	cp add ../../output/htdocs/cgi_bin

.PHONY:clean
clean:
	rm -rf add
