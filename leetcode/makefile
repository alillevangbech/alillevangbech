CC=g++
CFLAGS=-I.
DEPS = template.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: template.o
	$(CC) -o template template.o 