#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include "globals.h"

#define MAX_MACRO_COUNT 100
#define MAX_MACRO_NAME_LENGTH 100

typedef struct {
    char name[MAX_MACRO_NAME_LENGTH];
    char value[MAX_LINE_LENGTH];
} Macro;

int loadMacros(const char* filename, Macro *macros);
void replaceMacros(const char* filename, Macro* macros, int macroCount, const char* outputFilename);
char* pre_assemble(const char* filename);
char* createOutputFilename(const char* filename);

#endif /* PRE_ASSEMBLER_H */