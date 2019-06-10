CC = gcc
CFLAGS = -lcjson -lm
src = $(wildcard *.c)
obj = $(src:.c=.o)

simulate: compile.o vs.o node.o error.o
	$(CC) -o simulate simulator.c compile.o vs.o node.o error.o $(CFLAGS)

compile.o: compile.c compile.h
	$(CC) -c compile.c $(CFLAGS)

vs.o: vs.c vs.h
	$(CC) -c vs.c $(CFLAGS)

node.o: node.c node.h
	$(CC) -c node.c

error.o: error.c error.h
	$(CC) -c error.c

.PHONY: clean
clean:
	rm -f $(obj) simulate
