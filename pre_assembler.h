#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#define MAX_LINE_LENGTH 1000
#define MAX_MACRO_COUNT 100
#define MAX_MACRO_NAME_LENGTH 10

typedef struct {
    char name[MAX_MACRO_NAME_LENGTH];
    char value[MAX_LINE_LENGTH];
} Macro;

int loadMacros(const char* filename, Macro* macros);
void replaceMacros(const char* filename, Macro* macros, int macroCount, const char* outputFilename);
void pre_assemble(const char* filename, const char* outputFilename);

#endif /* PRE_ASSEMBLER_H */