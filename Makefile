CC=gcc

build: problema4.o
	$(CC) -Wall -g problema4.o -lm -o verifier

problema4.o: problema4.c functions.h
	$(CC) -c problema4.c

run: verifier
	./verifier
clean:
	rm -rf verifier
	rm -rf problema4.o
