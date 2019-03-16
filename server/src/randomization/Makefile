src = $(wildcard *.c)
obj = $(src:.c=.o)

CC = gcc
INCLUDE_DIRECTORY = -I/usr/include
LDFLAGS = -lcjson

randomize: $(obj)
	$(CC) $(INCLUDE_DIRECTORY) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) randomize
