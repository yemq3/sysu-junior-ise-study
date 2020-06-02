CC=gcc
CFLAGS=-Wall
PTHREADS=-lpthread
LRT=-lrt
MATH=-lm

all: dph prod cons mycall

dph: dph.c
	$(CC) $(CFLAGS) -o dph dph.c $(PTHREADS)

prod: prod.c
	$(CC) $(CFLAGS) -o prod prod.c $(PTHREADS) $(LRT) $(MATH)

cons: cons.c
	$(CC) $(CFLAGS) -o cons cons.c $(PTHREADS) $(LRT) $(MATH)

mycall: mycall.c
	$(CC) $(CFLAGS) -o mycall mycall.c $(PTHREADS)

clean:
	rm -rf dph
	rm -rf prod
	rm -rf cons
	rm -rf mycall