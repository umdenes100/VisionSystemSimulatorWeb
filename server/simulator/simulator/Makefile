CC = gcc
CFLAGS = -lcjson
src = $(wildcard *.c)
obj = $(src:.c=.o)

simulate: compile.o
	$(CC) -o simulate simulator.c compile.o $(CFLAGS)

compile.o: compile.c compile.h
	$(CC) -c compile.c $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) simulate