CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -g

all: proj2

proj2: main.o childcare.o
	$(CC) $(CFLAGS) -pthread main.o childcare.o -o proj2

main.o: main.c childcare.h
	$(CC) $(CFLAGS) -pthread -c main.c -o main.o 

childcare.o : childcare.c childcare.h
	$(CC) $(CFLAGS) -pthread -c childcare.c -o childcare.o 

clean:
	rm -f proj2
	rm -f main.o childcare.o
	rm -f proj2.out	
