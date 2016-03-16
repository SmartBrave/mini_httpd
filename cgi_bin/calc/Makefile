all:calc
calc:calc.cpp
	g++ -o $@ $^ -g

.PHONY:output
output:all
	cp calc ../../output/htdocs/cgi_bin

.PHONY:clean
clean:
	rm -rf calc
