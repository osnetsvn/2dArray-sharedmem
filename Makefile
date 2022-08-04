CFLAGS = -O3 -Wall -Wextra


all: 2d-shared-mem


2d-shared-mem: 2d-shared-mem.o sha-256.o

2d-shared-mem.o: sha-256.h 2d-shared-mem.c

sha-256.o: sha-256.h sha-256.c


.PHONY: clean
clean:
	rm -f 2d-shared-mem *.o
