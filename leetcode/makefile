CC=g++
CFLAGS=-I.
DEPS = 500lc.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: 500lc.o
	$(CC) -o 500lc 500lc.o