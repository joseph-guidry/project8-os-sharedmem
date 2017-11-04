CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Wwrite-strings -Wstack-usage=1024 -Wfloat-equal -Waggregate-return -Winline

all: dispatcher listener

dispatcher: dispatcher.c
	$(CC) $(CFLAGS) dispatcher.c -o dispatcher

listener: listener.c
	$(CC) $(CFLAGS) listener.c -o listener

debug: CFLAGS += -g
debug: server client

profile: CFLAGS += -pg
profile: server client

clean:
	rm -f listener dispatcher *.o
