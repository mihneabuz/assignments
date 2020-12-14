CC = gcc
CFLAGS = -Wall -g

build-liste: test-graf.c graf-L.c
	$(CC) $(CFLAGS) $^ -o $@

build-matrice: test-graf.c graf-M.c
	$(CC) $(CFLAGS) $^ -o $@

run-liste:
	./build-liste

run-matrice:
	./build-matrice

.PHONY: clean
clean:
	rm -f *.o *~ build-liste build-matrice
