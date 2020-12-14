CC = gcc
CFLAGS = -Wall -g

build: Stiva_Test Coada_Test

Coada_Test: Coada_Test.o TCoada.o TStiva.o
	$(CC) $(CFLAGS) $^ -o $@

Coada_Test.o: Coada_Test.c
	$(CC) $(CFLAGS) $^ -c -o $@

TCoada.o: TCoada.c
	$(CC) $(CFLAGS) $^ -c -o $@

Stiva_Test: Stiva_Test.o TStiva.o TCoada.o
	$(CC) $(CFLAGS) $^ -o $@

Stiva_Test.o: Stiva_Test.c
	$(CC) $(CFLAGS) $^ -c -o $@

TStiva.o: TStiva.c
	$(CC) $(CFLAGS) $^ -c -o $@

.PHONY: clean
clean:
	rm -f *.o *~ Stiva_Test Coada_Test