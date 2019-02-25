CFLAGS=-std=c++11 -O3 -Wall
OBJS=generator.o parser.o lexer.o netgen.o preprocessor.o
CC=g++

netgen: $(OBJS)
	$(CC) $(CFLAGS) -o netgen $(OBJS)

%.o: %.cc
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f netgen
	rm -f *.o
