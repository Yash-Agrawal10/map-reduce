CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lpthread

all: main

main: main.o mapreduce.o vector.o map.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

mapreduce.o: mapreduce.c mapreduce.h map.h vector.h
	$(CC) $(CFLAGS) -c $< -o $@

map.o: map.c map.h vector.h
	$(CC) $(CFLAGS) -c $< -o $@

vector.o: vector.c vector.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f main *.o
