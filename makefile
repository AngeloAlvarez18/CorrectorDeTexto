CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c99

main: main.o tablahash_d.o utils.o glist.o palabra.o
	$(CC) -g -o main main.o tablahash_d.o utils.o glist.o palabra.o $(FLAGS)

main.o: main.c tablahash.h glist.h palabra.h utils.h
	$(CC) -g -c main.c $(FLAGS)

tablahash_d.o: tablahash_d.c glist.h palabra.h utils.h
	$(CC) -g -c tablahash_d.c $(FLAGS)

utils.o: utils.c palabra.h tablahash.h
	$(CC) -g -c utils.c $(FLAGS)

glist.o: glist.c
	$(CC) -g -c glist.c $(FLAGS)

palabra.o: palabra.c tablahash.h
	$(CC) -g -c palabra.c $(FLAGS)

.PHONY: clean
clean:
	rm *.o
