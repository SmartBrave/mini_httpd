BIN=debug_cgi
SRC=debug_cgi.cpp
CC=g++
FLAGS=-o

.PHONY:all
all: $(BIN)
$(BIN):$(SRC)
	$(CC) $(FLAGS) $@ $^ -g

.PHONY:output
output:all
	cp $(BIN) ../../output/htdocs/cgi_bin

.PHONY:clean
clean:
	rm -rf $(BIN)
