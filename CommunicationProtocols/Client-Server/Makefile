CC = g++
CFLAGS = -Wall -Wextra

build: server subscriber

server: server.cpp structs.o
	$(CC) $(CFLAGS) server.cpp structs.o -o $@

subscriber: subscriber.cpp structs.o
	$(CC) $(CFLAGS) subscriber.cpp structs.o -o $@

structs: strucs.cpp
	$(CC) $(CFLAGS) structs.cpp -o structs.o

clean:
	rm -f server subscriber structs.o
