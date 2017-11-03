CC=gcc
CFLAGS+=-Wall -Wextra -Wpedantic
CFLAGS+=-Wwrite-strings -Wfloat-equal
CFLAGS+=-Waggregate-return -Winline

all: server client

server: dispatcher.c
	gcc $(CFLAGS) -o server dispatcher.c

client: listener.c
	gcc $(CFLAGS) -o client listener.c

clean:
	$(RM) server client *.o 
