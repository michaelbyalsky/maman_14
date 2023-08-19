#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include "globals.h"

#define MAX_MACRO_NAME_LENGTH 100

typedef struct Macro {
    char name[MAX_MACRO_NAME_LENGTH];
    char value[MAX_LINE_LENGTH];
    struct Macro *next;
} Macro;

/**
 * @brief load macros from file to macros table
 * @param filename
 * @param macrosHead
 * @return
 */
void loadMacros(const char *filename, Macro **macrosHead);

/**
 * @brief replace macros in file and write the file
 * @param filename
 * @param macrosHead
 * @param macroCount
 * @param outputFilename
 */
void replaceMacros(const char *filename, Macro **macrosHead, const char *outputFilename);

/**
 * @brief pre assemble the file
 * @param filename
 * @return
 */
char *pre_assemble(const char *filename);

/**
 * @brief create macro instance
 * @param name
 * @param value
 * @return Macro
 */
Macro *createMacro(const char *name, const char *value);

/**
 * @brief insert macro to the macros table
 * @param head
 * @param newMacro
 */
void insertMacro(Macro **head, Macro *newMacro);

/**
 * @brief iterate over the macros table and free memory
 * @param head
 */
void freeMacroList(Macro **head);

#endif /* PRE_ASSEMBLER_H */