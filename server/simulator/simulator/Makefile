CC = gcc
CFLAGS = -lcjson
src = $(wildcard *.c)
obj = $(src:.c=.o)

simulate: compile.o vs.o node.o
	$(CC) -o simulate simulator.c compile.o vs.o node.o $(CFLAGS)

compile.o: compile.c compile.h
	$(CC) -c compile.c $(CFLAGS)

vs.o: vs.c vs.h
	$(CC) -c vs.c

node.o: node.c node.h
	$(CC) -c node.c

.PHONY: clean
clean:
	rm -f $(obj) simulate
