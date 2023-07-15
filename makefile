CC=gcc
CFLAGS=-g -ansi -Wall -pedantic

all: assembler

assembler: main.o pre_assembler.o
	$(CC) $(CFLAGS) -o assembler main.o pre_assembler.o -lm

main.o: main.c pre_assembler.h
	$(CC) $(CFLAGS) -c main.c

pre_assembler.o: pre_assembler.c pre_assembler.h
	$(CC) $(CFLAGS) -c pre_assembler.c


