# rotor-fault-detection-c
CC=gcc
CFLAGS=-O3 -std=c11
LDFLAGS=-lm

all: rotor_fd

rotor_fd: main.c
	$(CC) $(CFLAGS) main.c -o rotor_fd $(LDFLAGS)

clean:
	rm -f rotor_fd diagnostic.json
