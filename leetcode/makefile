CC=g++
CFLAGS=-I.
DEPS = lc.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: lc.o
	$(CC) -o lc lc.o 