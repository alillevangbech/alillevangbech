CC=g++
CFLAGS=-I.
DEPS = lc.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: 101lc.o
	$(CC) -o 101lc 101lc.o