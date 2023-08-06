#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include "globals.h"

#define MAX_MACRO_NAME_LENGTH 100

typedef struct Macro {
    char name[MAX_MACRO_NAME_LENGTH];
    char value[MAX_LINE_LENGTH];
    struct Macro *next;
} Macro;

int loadMacros(const char *filename, Macro **macrosHead);

void replaceMacros(const char *filename, Macro **macrosHead, int macroCount, const char *outputFilename);

char *pre_assemble(const char *filename);

char *createOutputFilename(const char *filename);

Macro *createMacro(const char *name, const char *value);

void insertMacro(Macro **head, Macro *newMacro);

Macro *findMacro(Macro **head, const char *name);

void freeMacroList(Macro **head);

#endif /* PRE_ASSEMBLER_H */