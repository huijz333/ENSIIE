CC=gcc -Wall -Wextra -ansi

all: list_insertion list_merge
list_insertion.o: list.h
list_merge.o: list.h

list_insertion: list_insertion.o main.c
	$(CC) $^ list.c -o $@

list_merge: list_merge.o main.c
	$(CC) $^ list.c -o $@

clean:
	rm -rf *.o
