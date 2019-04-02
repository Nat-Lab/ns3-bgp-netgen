CFLAGS=-std=c++11 -O3 -Wall
OBJS_NETGEN=generator.o parser.o lexer.o netgen.o preprocessor.o
OBJS_BGPTREE=generator.o bgp-tree.o
CC=g++

.PHONY: all
all: netgen bgp-tree

netgen: $(OBJS_NETGEN)
	$(CC) $(CFLAGS) -o netgen $(OBJS_NETGEN)

bgp-tree: $(OBJS_BGPTREE)
	$(CC) $(CFLAGS) -o bgp-tree $(OBJS_BGPTREE)

%.o: %.cc
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f netgen bgp-tree *.o
