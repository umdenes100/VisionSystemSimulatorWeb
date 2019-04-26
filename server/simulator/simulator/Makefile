CC = gcc
CFLAGS = -lcjson
src = $(wildcard *.c)
obj = $(src:.c=.o)

simulate: compile.o vs.o
	$(CC) -o simulate simulator.c compile.o vs.o $(CFLAGS)

compile.o: compile.c compile.h
	$(CC) -c compile.c $(CFLAGS)

vs.o: vs.c vs.h
	$(CC) -c vs.c $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) simulate
