# Makefile for assignment 5
#

CC = g++
CPPFLAGS = -g -Wall
PROG = TwoPipesTwoChildren TwoPipesThreeChildren dynpipe

all : $(PROG)

TwoPipesTwoChildren : TwoPipesTwoChildren.o
	$(CC) $(CPPFLAGS) -o TwoPipesTwoChildren TwoPipesTwoChildren.o

TwoPipesTwoChildren.o :
	$(CC) $(CPPFLAGS) -c TwoPipesTwoChildren.cpp

TwoPipesThreeChildren : TwoPipesThreeChildren.o
	$(CC) $(CPPFLAGS) -o TwoPipesThreeChildren TwoPipesThreeChildren.o

TwoPipesThreeChildren.o :
	$(CC) $(CPPFLAGS) -c TwoPipesThreeChildren.cpp

dynpipe : DynPipe.o
	$(CC) $(CPPFLAGS) -o dynpipe DynPipe.o

DynPipe.o :
	$(CC) $(CPPFLAGS) -c DynPipe.cpp

clean:
	rm -f core $(PROG) *.o
