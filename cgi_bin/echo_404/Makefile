.PHONY:all
all:echo_404

add:echo_404.cpp
	g++ -o $@ $^ -g

.PHONY:output
output:all
	cp echo_404 ../../output/htdocs/cgi_bin

.PHONY:clean
clean:
	rm -rf echo_404
