CC = g++

BUILDDIR = ./build
SRCDIR = ./src
BINDIR = ./bin

.PHONY: build clean

build:
	# compile and assemble
	$(CC) -c $(SRCDIR)/main.cpp -o $(BUILDDIR)/main.o
	# link to an executable
	$(CC) $(BUILDDIR)/main.o -o $(BINDIR)/main

clean:
	rm $(BUILDDIR)/*
	rm $(BINDIR)/*
