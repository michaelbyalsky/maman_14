CC=gcc
CFLAGS=-g -ansi -Wall -pedantic
GLOBALS_DIR=globals.h
OUTPUT_FILES=assembler.o pre_assembler.o first_run.o helpers.o tables.o code.o second_run.o file_helpers.o

all: assembler

assembler: $(OUTPUT_FILES) $(GLOBALS_DIR)
	$(CC) $(CFLAGS) -o assembler $(OUTPUT_FILES)

assembler.o: assembler.c $(GLOBALS_DIR)
	$(CC) $(CFLAGS) -c assembler.c

pre_assembler.o: pre_assembler.c pre_assembler.h $(GLOBALS_DIR)
	$(CC) $(CFLAGS) -c pre_assembler.c

first_run.o: first_run.c first_run.h $(GLOBALS_DIR)
	$(CC) $(CFLAGS) -c first_run.c

second_run.o: second_run.c second_run.h $(GLOBALS_DIR)
	$(CC) $(CFLAGS) -c second_run.c

helpers.o: helpers.c helpers.h $(GLOBALS_DIR)
	$(CC) $(CFLAGS) -c helpers.c

tables.o: tables.c tables.h $(GLOBALS_DIR)
	$(CC) $(CFLAGS) -c tables.c

file_helpers.o: file_helpers.c file_helpers.h $(GLOBALS_DIR)
	$(CC) $(CFLAGS) -c file_helpers.c

code.o: code.c code.h $(GLOBALS_DIR)
	$(CC) $(CFLAGS) -c code.c

clean:
	rm -f assembler *.o