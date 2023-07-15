CC=gcc
CFLAGS=-g -ansi -Wall -pedantic

all: test

test: test.o pre_assembler.o
	$(CC) $(CFLAGS) -o test test.o pre_assembler.o -lm

test.o: test.c ../pre_assembler.h
	$(CC) $(CFLAGS) -c test.c

pre_assembler.o: ../pre_assembler.c ../pre_assembler.h
	$(CC) $(CFLAGS) -c ../pre_assembler.c


