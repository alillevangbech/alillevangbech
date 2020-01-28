CC=g++
CFLAGS=-I.
DEPS = main.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o main_fun.o 
	$(CC) -o main main.o main_fun.o 