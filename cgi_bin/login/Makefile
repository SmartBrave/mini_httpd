.PHONY:all
all:login

login:login.cpp
	g++ -o $@ $^

.PHONY:output
output:all
	cp login ../../output/htdocs/cgi_bin/

.PHONY:clean
clean:
	rm -rf login
