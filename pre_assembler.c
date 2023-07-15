#include <stdio.h>
#include <string.h>

#include "pre_assembler.h"

/**
 * @brief Loads macros from a file into an array of Macro structs.
 * @param filename The name of the file to load macros from.
 * @param macros The array of Macro structs to load macros into.
 * @return The number of macros loaded.
 */
int loadMacros(const char* filename, Macro* macros) {
    int macroCount = 0;
    char line[MAX_LINE_LENGTH];
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: could not open file %s\n", filename);
        return -1;
    }


    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (strncmp(line, "mcro ", 5) == 0) {
            char macroName[MAX_MACRO_NAME_LENGTH];
            char macroValue[MAX_LINE_LENGTH];
            sscanf(line, "%*s %s", macroName);
            strcpy(macros[macroCount].name, macroName);
            macroValue[0] = '\0';
            fgets(line, sizeof(line) , file); /* read the next line */
            while (strncmp(line, "endmcro", 7) != 0) {
                strcat(macroValue, line);
                fgets(line, sizeof(line) , file); /* read the next line */
            }
            strcpy(macros[macroCount].value, macroValue);

            macroCount++;
        }
    }

    fclose(file);
    return macroCount;
}

/**
 * @brief Replaces macros in a file with their values and remove the macro statement from the definition to endmcro.
 * @param filename The name of the file to replace macros in.
 * @param macros The array of Macro structs containing the macros to replace.
 * @param macroCount The number of macros in the macros array.
 */
void replaceMacros(const char* filename, Macro* macros, int macroCount, const char* outputFilename) {
    char line[MAX_LINE_LENGTH];
    int insideMacro = 0;  /* Flag to track if we are inside a macro paragraph */
    FILE* inputFile = fopen(filename, "r");
    FILE* outputFile = fopen(outputFilename, "w");

    if (inputFile == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    if (outputFile == NULL) {
        printf("Error creating file: test_macros.as\n");
        fclose(inputFile);
        return;
    }

    while (fgets(line, sizeof(line), inputFile)) {
        if (strncmp(line, "mcro", 4) == 0) {
            insideMacro = 1;
        }
        if (insideMacro) {
            /* Check if the current line is the end of a macro paragraph */
            if (strncmp(line, "endmcro", 7) == 0) {
                insideMacro = 0;
                continue;
            }
        } else {
            /* Iterate over all the macros and replace their occurrences in the line */
            int macroIndex;
            for (macroIndex = 0; macroIndex < macroCount; ++macroIndex) {
                char* macroName = macros[macroIndex].name;
                char* macroValue = macros[macroIndex].value;

                char* macroPosition;
                /* check if the macro name is in the line */
                if ((macroPosition = strstr(line, macroName)) != NULL) {
                    int macroValueLength = strlen(macroValue);
                    /* Replace macro with its value */
                    strncpy(macroPosition, macroValue, macroValueLength); /* Copy the macro value over the macro name */
                }
            }
        }

        if (!insideMacro) {
            fprintf(outputFile, "%s", line);
        }

    }

    fclose(inputFile);
    fclose(outputFile);
}

void pre_assemble(const char* filename, const char* outputFilename) {
    Macro macros[MAX_MACRO_COUNT];
    int macroCount = loadMacros(filename, macros);

    if (macroCount > 0) {
        replaceMacros(filename, macros, macroCount, outputFilename);
        printf("pre-assembled successfully.\n");
    } else {
        printf("No macros found in the file.\n");
    }
}