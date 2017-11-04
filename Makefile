CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Wwrite-strings -Wstack-usage=1024 -Wfloat-equal -Waggregate-return -Winline

all: server client

server: dispatcher.c
	$(CC) $(CFLAGS) dispatcher.c -o server

client: listener.c
	$(CC) $(CFLAGS) listener.c -o client

debug: CFLAGS += -g
debug: signaler sender

profile: CFLAGS += -pg
profile: signaler sender

clean:
	rm -f server  client *.o
