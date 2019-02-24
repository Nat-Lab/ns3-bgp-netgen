CFLAGS=-O3 -Wall
OBJS=generator.o parser.o lexer.o netgen.o
CC=g++

netgen: $(OBJS)
	$(CC) $(CFLAGS) -o netgen $(OBJS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f netgen
	rm -f *.o