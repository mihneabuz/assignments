CC=gcc
CFLAGS=-std=c99 -Wall -Wextra
EXE=ip

all: build

build: ip.c
	$(CC) $^ $(CFLAGS) -o $(EXE)

run: ip
	./ip

clean:
	rm -f ip

.PHONY: clean run
