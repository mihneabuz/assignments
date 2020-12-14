CFLAGS=-g
LDFLAGS=-lm
build: TArb.o test.o
	$(CC) $^ $(LDFLAGS) -o main
TArb.o: TArb.c
	$(CC) -c $^ -o $@

test.o: test.c
	        $(CC) -c $^ -o $@
clean:
	rm test.o TArb.o main
